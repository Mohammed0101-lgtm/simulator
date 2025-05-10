#include "instruction.h"


namespace kernel {
namespace cpu {
namespace instr {


kernel::cpu::instr::Instruction::Instruction(int val) {
    if (val < LOAD || (val > SYSCALL && val != HALT))
        throw kernel::error::Unknown_instruction("Error: Unknown instruction is not allowed.");
    _value = static_cast<VALUE>(val);
}

bool kernel::cpu::instr::Instruction::operator==(const Instruction& other) const {
    return this->_value == other._value;
}


}  // namespace instr
}  // namespace cpu
}  // namespace kernel