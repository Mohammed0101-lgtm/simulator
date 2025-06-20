#include "cpu/cpu.hh"
#include "init.h"  // Declare `init()` in a header if needed
#include "memory/memory.hh"

int main() {
    kernel::memo::MemoryManager mm;
    kernel::cpu::CPU            cpu;

    kernel::core::init(mm, cpu);

    return 0;
}
