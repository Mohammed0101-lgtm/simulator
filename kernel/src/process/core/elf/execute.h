#ifndef __EXECUTE_H__
#define __EXECUTE_H__

#include "../../../cpu/cpu.h"
#include "../../../memory/memory.h"
#include "../../process.h"


namespace kernel {
namespace core {
namespace elf {

void setup_stack(kernel::proc::Process& process, kernel::memo::MemoryManager& mm) {
    const uint32_t STACK_SIZE  = 0x1000 * 4;  // 4 pages (16KB)
    const uint32_t STACK_START = memo::STACK_START_ADDRESS;

    for (uint32_t offset = 0; offset < STACK_SIZE; offset += memo::PAGE_SIZE)
    {
        // Allocate a new page for the stack
        kernel::memo::PageID page_id = mm.allocate_physical_page();

        // Map it into the process's virtual address space
        uint32_t vaddr = STACK_START - offset;
        process.map_virtual_page(vaddr, page_id);
    }

    // Set the initial stack pointer
    process.process_control_block->setSP(STACK_START);
}

void initialize_registers(kernel::proc::Process& process, uint32_t entry_point, kernel::cpu::CPU& cpu) {
    // Zero all general-purpose CPU registers
    for (int i = 0; i < cpu._registers.size(); ++i)
        cpu._registers[i].setInstruction(kernel::cpu::instr::VALUE::NOP);

    // Set Program Counter (PC) in the process control block to entry point
    process.process_control_block->setPC(entry_point);

    // Stack Pointer (SP) will be set separately in setup_stack()
}


}  // namespace elf
}  // namespace core
}  // namespace kernel


#endif  // __EXECUTE_H__