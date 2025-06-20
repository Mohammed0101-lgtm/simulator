#include "cpu/cpu.hh"
#include "cpu/instruction.hh"
#include "cpu/register.hh"
#include "error.h"

#include <gtest/gtest.h>


TEST(CPU, DefaultConstructor) {
    kernel::cpu::CPU cpu;
    EXPECT_EQ(cpu._pc, 0);
    EXPECT_EQ(cpu._sp, 0);
    EXPECT_FALSE(cpu._halted);
    for (const auto& reg : cpu._registers)
        EXPECT_EQ(static_cast<int>(reg.getInstruction()._value), 0);
}

TEST(CPU, InstructionEquality) {
    kernel::cpu::instr::Instruction instr1(0x01);
    kernel::cpu::instr::Instruction instr2(0x01);
    kernel::cpu::instr::Instruction instr3(0x02);

    EXPECT_TRUE(instr1 == instr2);
    EXPECT_FALSE(instr1 == instr3);
}

TEST(CPU, RegisterConstructor) {
    kernel::cpu::Register reg(kernel::cpu::RegLabel::R0);
    EXPECT_EQ(reg.getLabel(), kernel::cpu::RegLabel::R0);
    EXPECT_EQ(reg.getInstruction()._value, 0);  // Assuming default value is 0
}

TEST(CPU, RegisterSetInstruction) {
    kernel::cpu::Register           reg(kernel::cpu::RegLabel::R1);
    kernel::cpu::instr::Instruction instruction(0x03);
    reg.setInstruction(instruction);
    EXPECT_EQ(reg.getInstruction()._value, 0x03);
}

TEST(CPU, RegisterCopyConstructor) {
    kernel::cpu::Register           reg1(kernel::cpu::RegLabel::R2);
    kernel::cpu::instr::Instruction instruction(0x04);
    reg1.setInstruction(instruction);

    kernel::cpu::Register reg2(reg1);
    EXPECT_EQ(reg2.getLabel(), kernel::cpu::RegLabel::R2);
    EXPECT_EQ(reg2.getInstruction()._value, 0x04);
}

TEST(CPU, CPUInstructionExecution) {
    kernel::cpu::CPU                cpu;
    kernel::cpu::instr::Instruction instruction(0x01);  // LOAD instruction
    cpu._registers[0].setInstruction(instruction);
    cpu.tick();
    EXPECT_EQ(cpu._registers[0].getInstruction()._value, 0x01);
}

TEST(CPU, CPUHaltedState) {
    kernel::cpu::CPU cpu;
    cpu._halted = true;
    EXPECT_TRUE(cpu._halted);
}

TEST(CPU, CPUHaltInstruction) {
    kernel::cpu::CPU                cpu;
    kernel::cpu::instr::Instruction instruction(0xFF);  // HALT instruction
    cpu._registers[0].setInstruction(instruction);
    cpu.tick();
    EXPECT_TRUE(cpu._halted);
}

TEST(CPU, UnknownInstructionException) {
    kernel::cpu::CPU                cpu;
    kernel::cpu::instr::Instruction instruction(0xFF);  // HALT instruction
    cpu._registers[0].setInstruction(instruction);
    EXPECT_THROW(cpu.tick(), kernel::error::Unknown_instruction);
}
