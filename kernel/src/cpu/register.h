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
    Register(const RegLabel label);
    Register(const Register& other);

    RegLabel                        getLabel() const;
    void                            setInstruction(const kernel::cpu::instr::Instruction instruction) noexcept;
    kernel::cpu::instr::Instruction getInstruction() const;
};


}  // namespace cpu
}  // namespace kernel

#endif  // __REGISTER_H__