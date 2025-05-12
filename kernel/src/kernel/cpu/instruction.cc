#include "cpu/instruction.h"


namespace kernel {
namespace cpu {
namespace instr {


kernel::cpu::instr::Instruction::Instruction(int val) {
    if (val < kernel::cpu::instr::VALUE::LOAD
        || (val > kernel::cpu::instr::VALUE::SYSCALL && val != kernel::cpu::instr::VALUE::HALT))
        throw kernel::error::Unknown_instruction("Error: Unknown instruction is not allowed.");
    _value = static_cast<VALUE>(val);
}

bool kernel::cpu::instr::Instruction::operator==(const kernel::cpu::instr::Instruction& other) const {
    return this->_value == other._value;
}


}  // namespace instr
}  // namespace cpu
}  // namespace kernel