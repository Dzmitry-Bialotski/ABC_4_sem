
#ifndef RISCV_SIM_CPU_H
#define RISCV_SIM_CPU_H

#include "Memory.h"
#include "Decoder.h"
#include "RegisterFile.h"
#include "CsrFile.h"
#include "Executor.h"

class Cpu
{
public:
    Cpu(Memory& mem)
        : _mem(mem)
    {

    }

    void ProcessInstruction()
    {
        Word raw_instr = _mem.Request(_ip);
        InstructionPtr decoded_instr = _decoder.Decode(raw_instr);
        _rf.Read(decoded_instr);
        _csrf.Read(decoded_instr);
        _exe.Execute(decoded_instr, _ip);
        _mem.Request(decoded_instr);
        _rf.Write(decoded_instr);
        _csrf.Write(decoded_instr);
        _csrf.InstructionExecuted();
        _ip = decoded_instr->_nextIp;
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
    Memory& _mem;
};


#endif //RISCV_SIM_CPU_H
