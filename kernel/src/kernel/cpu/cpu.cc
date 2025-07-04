#include "cpu/cpu.hh"
#include "cpu/instruction.hh"


namespace kernel {
namespace cpu {


kernel::cpu::CPU::CPU() :
    _registers{kernel::cpu::Register(kernel::cpu::RegLabel::R0), kernel::cpu::Register(kernel::cpu::RegLabel::R1),
               kernel::cpu::Register(kernel::cpu::RegLabel::R2), kernel::cpu::Register(kernel::cpu::RegLabel::R3),
               kernel::cpu::Register(kernel::cpu::RegLabel::R4), kernel::cpu::Register(kernel::cpu::RegLabel::R5),
               kernel::cpu::Register(kernel::cpu::RegLabel::R6), kernel::cpu::Register(kernel::cpu::RegLabel::R7)},
    _sp(0x00),
    _pc(0) {}

void kernel::cpu::CPU::tick() {
    uint32_t instr_val =
      kernel::memo::memory_manager.read_u32(this->_pc, 0);  // Read the instruction at PC with offset 0
    uint8_t  opcode = (instr_val >> 24) & 0xFF;
    uint8_t  rd     = (instr_val >> 20) & 0x0F;
    uint8_t  rs1    = (instr_val >> 16) & 0x0F;
    uint16_t imm    = instr_val & 0xFFFF;

    switch (opcode)
    {
    case kernel::cpu::instr::VALUE::LOAD :  // LOAD
        // Read the value from memory at the address (rs1 + imm), with offset 0
        this->_registers[rd].setInstruction(instr::Instruction(
          kernel::memo::memory_manager.read_u32(this->_registers[rs1].getInstruction()._value + imm, 0)));
        break;
    case kernel::cpu::instr::VALUE::STORE :  // STORE
        // Write the value of rd to memory at address (rs1 + imm), with offset 0
        kernel::memo::memory_manager.write_u32(this->_registers[rs1].getInstruction()._value + imm,
                                               this->_registers[rd].getInstruction()._value, 0);
        break;
    case kernel::cpu::instr::VALUE::MOV :  // MOV
        this->_registers[rd].setInstruction(this->_registers[rs1].getInstruction());
        break;
    case kernel::cpu::instr::VALUE::ADDI :  // ADDI
        this->_registers[rd].setInstruction(instr::Instruction(this->_registers[rs1].getInstruction()._value + imm));
        break;
    case kernel::cpu::instr::VALUE::ADD :  // ADD
        this->_registers[rd].setInstruction(instr::Instruction(this->_registers[rs1].getInstruction()._value
                                                               + this->_registers[imm & 0x0F].getInstruction()._value));
        break;
    case kernel::cpu::instr::VALUE::SUB :  // SUB
        this->_registers[rd].setInstruction(instr::Instruction(this->_registers[rs1].getInstruction()._value
                                                               - this->_registers[imm & 0x0F].getInstruction()._value));
        break;
    case kernel::cpu::instr::VALUE::JMP :  // JMP
        this->_pc = imm;
        return;
    case kernel::cpu::instr::VALUE::BEQ :  // BEQ
        if (this->_registers[rd].getInstruction() == this->_registers[rs1].getInstruction())
            this->_pc = imm;
        else
            this->_pc += 4;
        return;
    case kernel::cpu::instr::VALUE::SYSCALL :  // SYSCALL
        this->syscall_handler();
        break;
    case kernel::cpu::instr::VALUE::AND :  // AND
        this->_registers[rd].setInstruction(instr::Instruction(this->_registers[rs1].getInstruction()._value
                                                               & this->_registers[imm & 0x0F].getInstruction()._value));
        break;
    case kernel::cpu::instr::VALUE::OR :  // OR
        this->_registers[rd].setInstruction(instr::Instruction(this->_registers[rs1].getInstruction()._value
                                                               | this->_registers[imm & 0x0F].getInstruction()._value));
        break;
    case kernel::cpu::instr::VALUE::XOR :  // XOR
        this->_registers[rd].setInstruction(instr::Instruction(this->_registers[rs1].getInstruction()._value
                                                               ^ this->_registers[imm & 0x0F].getInstruction()._value));
        break;
    case kernel::cpu::instr::VALUE::NOT :  // NOT
        this->_registers[rd].setInstruction(
          instr::Instruction(~this->_registers[rs1].getInstruction()._value & 0xFFFFFFFF));
        break;
    case kernel::cpu::instr::VALUE::SHL :  // SHL
        this->_registers[rd].setInstruction(
          instr::Instruction(this->_registers[rs1].getInstruction()._value << (imm & 0x1F)));
        break;
    case kernel::cpu::instr::VALUE::SHR :  // SHR
        this->_registers[rd].setInstruction(
          instr::Instruction(this->_registers[rs1].getInstruction()._value >> (imm & 0x1F)));
        break;
    case kernel::cpu::instr::VALUE::HALT :  // HALT
        this->_halted = true;
        return;
    default :
        throw kernel::error::Unknown_instruction("Invalid opcode");
    }

    this->_pc += 4;  // Move PC forward
}

void kernel::cpu::CPU::syscall_handler() {
    // Read syscall ID from R0
    kernel::cpu::instr::VALUE id = this->_registers[0].getInstruction()._value;

    // Instructions that involve printing memory content
    auto print_memory = [this]() {
        // Read address and length from R1 and R2
        uint32_t addr = this->_registers[1].getInstruction()._value;
        uint32_t len  = this->_registers[2].getInstruction()._value;
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
        this->_halted = true;
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
