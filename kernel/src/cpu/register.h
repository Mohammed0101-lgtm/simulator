#ifndef __REGISTER_H__
#define __REGISTER_H__

#include "instruction.h"
#include <cstdint>


namespace kernel {
namespace cpu {


typedef enum: uint32_t {
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7
} RegLabel;

struct Register
{
   private:
    const RegLabel                  _label;
    kernel::cpu::instr::Instruction _instruction;

   public:
    Register() = delete;  // Can't use default constructor

    Register(const RegLabel label) :
        _label(label),
        _instruction(kernel::cpu::instr::VALUE::HALT) {}

    Register(const Register& other) :
        _label(other.getLabel()),
        _instruction(other.getInstruction()) {}

    RegLabel getLabel() const { return _label; }
    void     setInstruction(const kernel::cpu::instr::Instruction instruction) noexcept { _instruction = instruction; }
    kernel::cpu::instr::Instruction getInstruction() const { return _instruction; }
};


}  // namespace cpu
}  // namespace kernel

#endif  // __REGISTER_H__