#include "execute.h"


namespace kernel {
namespace cpu {


SimulatedCPU::SimulatedCPU(memo::MemoryManager& mm) :
    _memory(mm),
    _regs{Register(RegLabel::R0), Register(RegLabel::R1), Register(RegLabel::R2), Register(RegLabel::R3),
          Register(RegLabel::R4), Register(RegLabel::R5), Register(RegLabel::R6), Register(RegLabel::R7)},
    _sp(0x00),
    _pc(0) {}

void SimulatedCPU::tick() {
    uint32_t instr  = _memory.read_u32(_pc, 0);  // Read the instruction at PC with offset 0
    uint8_t  opcode = (instr >> 24) & 0xFF;
    uint8_t  rd     = (instr >> 20) & 0x0F;
    uint8_t  rs1    = (instr >> 16) & 0x0F;
    uint16_t imm    = instr & 0xFFFF;

    switch (opcode)
    {
    case 0x01 :  // LOAD
        // Read the value from memory at the address (rs1 + imm), with offset 0
        _regs[rd].setInstruction(
          static_cast<const Instruction>(_memory.read_u32(_regs[rs1].getInstruction() + imm, 0)));
        break;
    case 0x02 :  // STORE
        // Write the value of rd to memory at address (rs1 + imm), with offset 0
        _memory.write_u32(_regs[rs1].getInstruction() + imm, _regs[rd].getInstruction(), 0);
        break;
    case 0x03 :  // MOV
        _regs[rd].setInstruction(_regs[rs1].getInstruction());
        break;
    case 0x04 :  // ADDI
        _regs[rd].setInstruction(static_cast<const Instruction>(_regs[rs1].getInstruction() + imm));
        break;
    case 0x05 :  // ADD
        _regs[rd].setInstruction(
          static_cast<const Instruction>(_regs[rs1].getInstruction() + _regs[imm & 0x0F].getInstruction()));
        break;
    case 0x06 :  // SUB
        _regs[rd].setInstruction(
          static_cast<const Instruction>(_regs[rs1].getInstruction() - _regs[imm & 0x0F].getInstruction()));
        break;
    case 0x07 :  // JMP
        _pc = imm;
        return;
    case 0x08 :  // BEQ
        if (_regs[rd].getInstruction() == _regs[rs1].getInstruction())
            _pc = imm;
        else
            _pc += 4;
        return;
    case 0x09 :  // SYSCALL
        syscall_handler();
        break;
    case 0xFF :  // HALT
        _halted = true;
        return;
    default :
        throw std::runtime_error("Invalid opcode");
    }

    _pc += 4;  // Move PC forward
}

void SimulatedCPU::syscall_handler() {
    uint32_t id = _regs[0].getInstruction();
    switch (id)
    {
    case 1 : {
        uint32_t addr = _regs[1].getInstruction();
        uint32_t len  = _regs[2].getInstruction();
        for (uint32_t i = 0; i < len; ++i)
            std::cout << _memory.read_u8(addr, i);
        break;
    }
    case 2 : {
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
