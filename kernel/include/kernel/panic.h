#ifndef __PANIC_H__
#define __PANIC_H__

#include "cpu/cpu.h"
#include <string>


namespace kernel {

void panic(const std::string& msg, kernel::cpu::CPU& cpu) {
    std::cerr << "PC: " << std::hex << cpu._pc << "\n";
    std::cerr << "SP: " << std::hex << cpu._sp << "\n";
    std::cerr << "Registers:\n";

    for (int i = 0; i < 8; ++i)
        std::cerr << "R" << i << ": " << std::hex << cpu._registers[i].getInstruction()._value << "\n";

    throw kernel::error::Panic_kernel(msg);
    // Optionally, you can also print the instruction that caused the panic
    // std::cerr << "Instruction: " << cpu._regs[cpu._pc].getInstruction()._value << "\n";
}

}  // namespace kernel


#endif  // __PANIC_H__