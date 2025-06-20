#include "cpu/register.hh"


namespace kernel {
namespace cpu {


kernel::cpu::Register::Register(const RegLabel label) :
    _label(label),
    _instruction(kernel::cpu::instr::VALUE::HALT) {}

kernel::cpu::Register::Register(const Register& other) :
    _label(other.getLabel()),
    _instruction(other.getInstruction()) {}

kernel::cpu::RegLabel kernel::cpu::Register::getLabel() const { return this->_label; }

void kernel::cpu::Register::setInstruction(const kernel::cpu::instr::Instruction instruction) noexcept {
    this->_instruction = instruction;
}

kernel::cpu::instr::Instruction kernel::cpu::Register::getInstruction() const { return this->_instruction; }


}  // namespace cpu
}  // namespace kernel