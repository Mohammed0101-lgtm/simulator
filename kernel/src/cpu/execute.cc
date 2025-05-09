#include "execute.h"
#include "instruction.h"


namespace kernel {
namespace cpu {


SimulatedCPU::SimulatedCPU() :
    _regs{Register(RegLabel::R0), Register(RegLabel::R1), Register(RegLabel::R2), Register(RegLabel::R3),
          Register(RegLabel::R4), Register(RegLabel::R5), Register(RegLabel::R6), Register(RegLabel::R7)},
    _sp(0x00),
    _pc(0) {}

void SimulatedCPU::tick() {
    uint32_t instr_val = kernel::memo::memory_manager.read_u32(_pc, 0);  // Read the instruction at PC with offset 0
    uint8_t  opcode    = (instr_val >> 24) & 0xFF;
    uint8_t  rd        = (instr_val >> 20) & 0x0F;
    uint8_t  rs1       = (instr_val >> 16) & 0x0F;
    uint16_t imm       = instr_val & 0xFFFF;

    switch (opcode)
    {
    case kernel::cpu::instr::VALUE::LOAD :  // LOAD
        // Read the value from memory at the address (rs1 + imm), with offset 0
        _regs[rd].setInstruction(
          instr::Instruction(kernel::memo::memory_manager.read_u32(_regs[rs1].getInstruction()._value + imm, 0)));
        break;
    case kernel::cpu::instr::VALUE::STORE :  // STORE
        // Write the value of rd to memory at address (rs1 + imm), with offset 0
        kernel::memo::memory_manager.write_u32(_regs[rs1].getInstruction()._value + imm,
                                               _regs[rd].getInstruction()._value, 0);
        break;
    case kernel::cpu::instr::VALUE::MOV :  // MOV
        _regs[rd].setInstruction(_regs[rs1].getInstruction());
        break;
    case kernel::cpu::instr::VALUE::ADDI :  // ADDI
        _regs[rd].setInstruction(instr::Instruction(_regs[rs1].getInstruction()._value + imm));
        break;
    case kernel::cpu::instr::VALUE::ADD :  // ADD
        _regs[rd].setInstruction(
          instr::Instruction(_regs[rs1].getInstruction()._value + _regs[imm & 0x0F].getInstruction()._value));
        break;
    case kernel::cpu::instr::VALUE::SUB :  // SUB
        _regs[rd].setInstruction(
          instr::Instruction(_regs[rs1].getInstruction()._value - _regs[imm & 0x0F].getInstruction()._value));
        break;
    case kernel::cpu::instr::VALUE::JMP :  // JMP
        _pc = imm;
        return;
    case kernel::cpu::instr::VALUE::BEQ :  // BEQ
        if (_regs[rd].getInstruction() == _regs[rs1].getInstruction())
            _pc = imm;
        else
            _pc += 4;
        return;
    case kernel::cpu::instr::VALUE::SYSCALL :  // SYSCALL
        syscall_handler();
        break;
    case kernel::cpu::instr::VALUE::HALT :  // HALT
        _halted = true;
        return;
    default :
        throw kernel::error::unknown_instruction("Invalid opcode");
    }

    _pc += 4;  // Move PC forward
}

void SimulatedCPU::syscall_handler() {
    kernel::cpu::instr::VALUE id = _regs[0].getInstruction()._value;

    switch (id)
    {
    case kernel::cpu::instr::VALUE::STORE : {
        uint32_t addr = _regs[1].getInstruction()._value;
        uint32_t len  = _regs[2].getInstruction()._value;
        for (uint32_t i = 0; i < len; ++i)
            std::cout << kernel::memo::memory_manager.read_u8(addr, i);
        break;
    }
    case kernel::cpu::instr::VALUE::MOV : {
        _halted = true;
        break;
    }
    default :
        std::cerr << "Unknown syscall ID: " << id << "\n";
        break;
    }
}


}  // namespace cpu
}  // namespace kernel
