#include "cpu.h"
#include "instruction.h"


namespace kernel {
namespace cpu {


kernel::cpu::CPU::CPU() :
    _registers{Register(RegLabel::R0), Register(RegLabel::R1), Register(RegLabel::R2), Register(RegLabel::R3),
               Register(RegLabel::R4), Register(RegLabel::R5), Register(RegLabel::R6), Register(RegLabel::R7)},
    _sp(0x00),
    _pc(0) {}

void kernel::cpu::CPU::tick() {
    uint32_t instr_val = kernel::memo::memory_manager.read_u32(_pc, 0);  // Read the instruction at PC with offset 0
    uint8_t  opcode    = (instr_val >> 24) & 0xFF;
    uint8_t  rd        = (instr_val >> 20) & 0x0F;
    uint8_t  rs1       = (instr_val >> 16) & 0x0F;
    uint16_t imm       = instr_val & 0xFFFF;

    switch (opcode)
    {
    case kernel::cpu::instr::VALUE::LOAD :  // LOAD
        // Read the value from memory at the address (rs1 + imm), with offset 0
        _registers[rd].setInstruction(
          instr::Instruction(kernel::memo::memory_manager.read_u32(_registers[rs1].getInstruction()._value + imm, 0)));
        break;
    case kernel::cpu::instr::VALUE::STORE :  // STORE
        // Write the value of rd to memory at address (rs1 + imm), with offset 0
        kernel::memo::memory_manager.write_u32(_registers[rs1].getInstruction()._value + imm,
                                               _registers[rd].getInstruction()._value, 0);
        break;
    case kernel::cpu::instr::VALUE::MOV :  // MOV
        _registers[rd].setInstruction(_registers[rs1].getInstruction());
        break;
    case kernel::cpu::instr::VALUE::ADDI :  // ADDI
        _registers[rd].setInstruction(instr::Instruction(_registers[rs1].getInstruction()._value + imm));
        break;
    case kernel::cpu::instr::VALUE::ADD :  // ADD
        _registers[rd].setInstruction(
          instr::Instruction(_registers[rs1].getInstruction()._value + _registers[imm & 0x0F].getInstruction()._value));
        break;
    case kernel::cpu::instr::VALUE::SUB :  // SUB
        _registers[rd].setInstruction(
          instr::Instruction(_registers[rs1].getInstruction()._value - _registers[imm & 0x0F].getInstruction()._value));
        break;
    case kernel::cpu::instr::VALUE::JMP :  // JMP
        _pc = imm;
        return;
    case kernel::cpu::instr::VALUE::BEQ :  // BEQ
        if (_registers[rd].getInstruction() == _registers[rs1].getInstruction())
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
        throw kernel::error::Unknown_instruction("Invalid opcode");
    }

    _pc += 4;  // Move PC forward
}

void kernel::cpu::CPU::syscall_handler() {
    // Read syscall ID from R0
    kernel::cpu::instr::VALUE id = _registers[0].getInstruction()._value;

    // Instructions that involve printing memory content
    auto print_memory = [this]() {
        // Read address and length from R1 and R2
        uint32_t addr = _registers[1].getInstruction()._value;
        uint32_t len  = _registers[2].getInstruction()._value;
        // Print memory content
        for (uint32_t i = 0; i < len; ++i)
            std::cout << kernel::memo::memory_manager.read_u8(addr, i);
    };

    switch (id)
    {
    case kernel::cpu::instr::VALUE::STORE :
    case kernel::cpu::instr::VALUE::ADDI :
    case kernel::cpu::instr::VALUE::ADD :
    case kernel::cpu::instr::VALUE::SUB :
    case kernel::cpu::instr::VALUE::JMP :
    case kernel::cpu::instr::VALUE::BEQ :
    case kernel::cpu::instr::VALUE::LOAD :
        print_memory();
        break;

    case kernel::cpu::instr::VALUE::MOV :
    case kernel::cpu::instr::VALUE::HALT :
        _halted = true;
        break;

    default :
        std::cerr << "Unknown syscall ID: " << static_cast<int>(id) << "\n";
        break;
    }
}

void kernel::cpu::CPU::run_until_halt() {
    while (!this->_halted)
        this->tick();

    std::cout << "Program halted.\n";
}


}  // namespace cpu
}  // namespace kernel
