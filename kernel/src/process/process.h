#ifndef __PROCESS_H__
#define __PROCESS_H__


#include "PCB.h"
#include "memory.h"
#include <memory>


namespace kernel {
namespace proc {

class Process
{
   public:
    std::unique_ptr<proc::PCB> process_control_block;
    memo::PageTable            page_table;

    Process() = default;

    explicit Process(int pid, int priority, int pc, int sp, int base, int limit, std::size_t stackSize) {
        process_control_block = std::make_unique<PCB>(PCB(pid, priority, pc, sp, base, limit, stackSize));
        page_table.entries.clear();  // Initialize the page table
    }

    explicit Process(Process&& process) :
        process_control_block(std::move(process.process_control_block)),
        page_table(std::move(process.page_table)) {}

    explicit Process(const Process& process) {
        if (process.process_control_block)
            process_control_block = std::make_unique<proc::PCB>(*process.process_control_block);

        page_table = process.page_table;  // Assumes PageTable and its entries support deep copy
    }

    Process& operator=(const Process& other) {
        if (this == &other)
            return *this;

        // Deep copy PCB
        if (other.process_control_block)
            process_control_block = std::make_unique<proc::PCB>(*other.process_control_block);
        else
            process_control_block.reset();

        // Deep copy page table
        page_table = other.page_table;

        return *this;
    }

    void write_to_virtual_page(memo::MemoryManager& mm, memo::PageID vpage, int offset, char value);
    char read_from_virtual_page(memo::MemoryManager& mm, memo::PageID vpage, int offset);

    std::unique_ptr<Process> Process::fork_process(memo::MemoryManager& mm, PID child_pid);
};

};  // namespace proc
};  // namespace proc


#endif  // __PROCESS_H__