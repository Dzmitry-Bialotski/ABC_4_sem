
#ifndef RISCV_SIM_CPU_H
#define RISCV_SIM_CPU_H

#include "Memory.h"
#include "Decoder.h"
#include "RegisterFile.h"
#include "CsrFile.h"
#include "Executor.h"
#include <optional>
class Cpu
{
public:
    Cpu(IMem& mem)//static constexpr size_t memSize = 4*1024*1024; // memory size in 4-byte words
            : _mem(mem)
    {
        request_done = false;
        instruction_executed = false;
    }
    void Clock()
    {
        _csrf.Clock();
        if(request_done==false)
        {
            _mem.Request(_ip);//запрашиваем инструкцию из памяти
            request_done = true;
        }
        else if(instruction_executed == false)
        {
            instr=_mem.Response();
            if(instr.has_value()==false)
                return;
            //когда дождались инструкцию из памяти:
            instr_ptr=_decoder.Decode(instr.value());
            _rf.Read(instr_ptr);
            _csrf.Read(instr_ptr);
            _exe.Execute(instr_ptr,_ip);
            instruction_executed = true;
            _mem.Request(instr_ptr);
        }
        else
        {
            if(!_mem.Response(instr_ptr))//ждем еще один ответ если это интрукции LD и ST
                return;
            _rf.Write(instr_ptr);
            _csrf.Write(instr_ptr);
            _csrf.InstructionExecuted();
            _ip=instr_ptr->_nextIp;
            //интрукция завершена
            request_done = false;
            instruction_executed = false;
        }
    }
    void Reset(Word ip)
    {
        _csrf.Reset();
        _ip = ip;
    }
    std::optional<CpuToHostData> GetMessage()
    {
        return _csrf.GetMessage();
    }
private:
    Reg32 _ip;
    Decoder _decoder;
    RegisterFile _rf;
    CsrFile _csrf;
    Executor _exe;
    IMem& _mem;
    // Add your code here, if needed
    InstructionPtr instr_ptr;
    std::optional<Word> instr;
    bool request_done;
    bool instruction_executed;
};
#endif //RISCV_SIM_CPU_H