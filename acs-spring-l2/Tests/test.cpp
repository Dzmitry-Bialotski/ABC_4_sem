//
// Created by dimasiandro on 16.04.20.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/Cpu.h"
#include "../src/Memory.h"
#include "../src/BaseTypes.h"
#include <optional>

using testing::Eq;

namespace {
    class decoder_test : public testing::Test {
    public:
        Word instraction;
        Decoder decoder;
        InstructionPtr instructionPtr;
        decoder_test(){
            instraction = 0b00000000000000000000000001101111;
            instructionPtr = decoder.Decode(instraction);
        }
    };
    class executor_test : public testing::Test {
    public:
        Word instraction;
        Decoder decoder;
        InstructionPtr instructionPtr;
        Executor executor;
        executor_test(){
            instraction = 0b00000000000000000000000001101111;
            instructionPtr = decoder.Decode(instraction);
            executor.Execute(instructionPtr, 0);
        }
    };
}
TEST_F(decoder_test, test1){
    ASSERT_EQ(instructionPtr ->_type,IType::J);
    ASSERT_EQ(instructionPtr ->_imm, 0);
    ASSERT_EQ(instructionPtr ->_brFunc,BrFunc::AT);
    ASSERT_EQ(instructionPtr ->_aluFunc,AluFunc::Add);
}

TEST_F(executor_test, test1){
    //в instructionPtr ->_addr лежит мусор
    ASSERT_EQ(instructionPtr ->_nextIp,4);
    ASSERT_EQ(instructionPtr ->_data,4);
}