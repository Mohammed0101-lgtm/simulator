#ifndef __EXECUTE_H__
#define __EXECUTE_H__

#include "../../../cpu/cpu.h"
#include "../../../memory/memory.h"
#include "../../process.h"


namespace kernel {
namespace core {
namespace elf {


void setup_stack(kernel::proc::Process& process) {
    const uint32_t STACK_SIZE  = 0x1000 * 4;  // 4 pages (16KB)
    const uint32_t STACK_START = memo::STACK_START_ADDRESS;

    for (uint32_t offset = 0; offset < STACK_SIZE; offset += memo::PAGE_SIZE)
    {
        // Allocate a new page for the stack
        kernel::memo::PageID page_id = kernel::memo::memory_manager.allocate_physical_page();

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

void setup_stack(kernel::proc::Process& process, const std::vector<std::string>& args) {
    constexpr uint32_t    STACK_BASE = kernel::memo::STACK_START_ADDRESS;
    uint32_t              sp         = STACK_BASE;
    std::vector<uint32_t> argv_ptrs;

    // Step 1: Copy argument strings onto the stack (in reverse)
    for (auto it = args.rbegin(); it != args.rend(); ++it)
    {
        const std::string& arg = *it;
        sp -= arg.size() + 1;  // null-terminated

        for (size_t i = 0; i < arg.size(); ++i)
            process.write_to_virtual_page(sp, i, arg[i]);

        process.write_to_virtual_page(sp, arg.size(), '\0');
        argv_ptrs.push_back(sp);
    }

    // Step 2: Align the stack (optional but common: 4- or 8-byte alignment)
    sp &= ~0x3;

    // Step 3: Push argv pointers
    sp -= sizeof(uint32_t) * (argv_ptrs.size() + 1);  // +1 for null
    uint32_t argv_addr = sp;

    for (size_t i = 0; i < argv_ptrs.size(); ++i)
    {
        uint32_t ptr = argv_ptrs[argv_ptrs.size() - 1 - i];
        for (int b = 0; b < 4; ++b)
            process.write_to_virtual_page(sp + i * 4, b, (ptr >> ((3 - b) * 8)) & 0xFF);
    }

    // Null-terminate argv
    for (int b = 0; b < 4; ++b)
        process.write_to_virtual_page(sp + argv_ptrs.size() * 4, b, 0);

    // Step 4: Push argc
    sp -= 4;
    uint32_t argc = static_cast<uint32_t>(args.size());

    for (int b = 0; b < 4; ++b)
        process.write_to_virtual_page(sp, b, (argc >> ((3 - b) * 8)) & 0xFF);

    // Step 5: Set stack pointer
    process.process_control_block->setSP(sp);
}


}  // namespace elf
}  // namespace core
}  // namespace kernel


#endif  // __EXECUTE_H__