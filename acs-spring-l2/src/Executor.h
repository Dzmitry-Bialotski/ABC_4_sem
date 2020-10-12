
#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"
#include <map>
#include <functional>
class Executor
{
public:
    void Execute(InstructionPtr& instr, Word ip)
    {
        // Add your code here.
        if(instr->_src1.has_value() and ( instr->_imm.has_value() or instr->_src2.has_value() ))
        {
            Word first_arg = instr->_src1Val;
            Word second_arg = instr->_imm.value_or(instr->_src2Val);
            Word aluFunc;
            switch (instr->_aluFunc) {
                case AluFunc::Add:
                {
                    aluFunc = Add(first_arg, second_arg);
                    break;
                }
                case AluFunc::Sub:
                {
                    aluFunc = Sub(first_arg, second_arg);
                    break;
                }
                case AluFunc::And:
                {
                    aluFunc = And(first_arg, second_arg);
                    break;
                }
                case AluFunc::Or:
                {
                    aluFunc = Or(first_arg, second_arg);
                    break;
                }
                case AluFunc::Xor:
                {
                    aluFunc = Xor(first_arg, second_arg);
                    break;
                }
                case AluFunc::Slt:
                {
                    aluFunc = Slt(first_arg, second_arg);
                    break;
                }
                case AluFunc::Sltu:
                {
                    aluFunc = Sltu(first_arg, second_arg);
                    break;
                }
                case AluFunc::Sll:
                {
                    aluFunc = Sll(first_arg, second_arg);
                    break;
                }
                case AluFunc::Srl:
                {
                    aluFunc = Srl(first_arg, second_arg);
                    break;
                }
                case AluFunc::Sra:
                {
                    aluFunc = Sra(first_arg, second_arg);
                    break;
                }
            }
            switch (instr->_type) {
                case IType::Alu:
                    instr->_data = aluFunc;
                    break;
                case IType::St:
                    instr->_data = instr->_src2Val;
                    instr->_addr = aluFunc;
                    break;
                case IType::Ld:
                    instr->_addr = aluFunc;
                    break;
            }
        }
        switch (instr->_type) {
            case IType::Csrr:
                instr->_data = instr->_csrVal;
                break;
            case IType::Csrw:
                instr->_data = instr->_src1Val;
                break;
            case IType::J:
                instr->_data = ip + 4;
                break;
            case IType::Jr:
                instr->_data = ip + 4;
                break;
            case IType::Auipc:
                instr->_data = ip + instr->_imm.value();
                break;
        }
        bool brFunc = false;
        Word next_ip;
        if(instr->_src1.has_value() && instr->_src2.has_value()){
            switch (instr->_type) {
                case IType::Br:
                    next_ip = ip + instr->_imm.value();
                    break;
                case IType::J:
                    next_ip = ip + instr->_imm.value();
                    break;
                case IType::Jr:
                    next_ip = instr->_src1Val + instr->_imm.value();
                    break;
            }
            switch(instr->_brFunc)
            {
                case BrFunc::Eq:
                {
                    brFunc = Eq(instr->_src1Val, instr->_src2Val);
                }
                case BrFunc::Neq:
                {
                    brFunc = Neq(instr->_src1Val, instr->_src2Val);
                }
                case BrFunc::Lt:
                {
                    brFunc = Lt(instr->_src1Val, instr->_src2Val);
                }
                case BrFunc::Ltu:
                {
                    brFunc = Ltu(instr->_src1Val, instr->_src2Val);
                }
                case BrFunc::Ge:
                {
                    brFunc = Ge(instr->_src1Val, instr->_src2Val);
                }
                case BrFunc::Geu:
                {
                    brFunc = Geu(instr->_src1Val, instr->_src2Val);
                }
                case BrFunc::AT:
                {
                    brFunc = AT(instr->_src1Val, instr->_src2Val);
                }
                case BrFunc::NT:
                {
                    brFunc = NT(instr->_src1Val, instr->_src2Val);
                }
            }
        }
        if(brFunc == true)
        {
            instr->_nextIp = next_ip;
        }
        else
        {
            instr->_nextIp = ip + 4;
        }
    }

private:
    // Add helper functions here
    //AluFunc
    Word Add(Word op1, Word op2)
    {
        return op1 + op2;
    }

    Word Sub(Word op1, Word op2)
    {
        return op1 - op2;
    }

    Word And(Word op1, Word op2)
    {
        return op1 & op2;
    }

    Word Or(Word op1, Word op2)
    {
        return op1 | op2;
    }

    Word Xor(Word op1, Word op2)
    {
        return op1 ^ op2;
    }

    Word Slt(Word op1, Word op2)
    {
        return (Word)((int32_t)op1 < (int32_t)op2);
    }

    Word Sltu(Word op1, Word op2)
    {
        return (Word)(op1 < op2);
    }

    Word Sll(Word op1, Word op2)
    {
        return op1 << (op2 % 32);
    }

    Word Srl(Word op1, Word op2)
    {
        return op1 >> (op2 % 32);
    }

    Word Sra(Word op1, Word op2)
    {
        return (Word)((int32_t)op1 >> (op2 % 32));
    }
    // BrFunc
    bool Eq(Word op1, Word op2)
    {
        return op1 == op2;
    }

    bool Neq(Word op1, Word op2)
    {
        return op1 != op2;
    }

    bool Lt(Word op1, Word op2)
    {
        return (int32_t)op1 < (int32_t)op2;
    }

    bool Ltu(Word op1, Word op2)
    {
        return op1 < op2;
    }

    bool Ge(Word op1, Word op2)
    {
        return (int32_t)op1 >= (int32_t)op2;
    }

    bool Geu(Word op1, Word op2)
    {
        return op1 >= op2;
    }

    bool AT(Word op1, Word op2)
    {
        return true;
    }

    bool NT(Word op1, Word op2)
    {
        return false;
    }
};

#endif // RISCV_SIM_EXECUTOR_H
