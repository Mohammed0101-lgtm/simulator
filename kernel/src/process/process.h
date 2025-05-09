#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "../memory/memory.h"
#include "PCB.h"
#include <memory>


namespace kernel {
namespace proc {

class Process
{
   public:
    std::unique_ptr<proc::PCB> process_control_block;
    kernel::memo::PageTable    page_table;

    Process() = default;

    explicit Process(int pid, int priority, int pc, int sp, int base, int limit, std::size_t stackSize);
    explicit Process(Process&& process);
    explicit Process(const Process& process);

    Process& operator=(const Process& other);

    void                     write_to_virtual_page(kernel::memo::PageID vpage, int offset, char value);
    char                     read_from_virtual_page(kernel::w::PageID vpage, int offset);
    std::unique_ptr<Process> fork_process(PID child_pid);
};

}  // namespace proc
}  // namespace proc


#endif  // __PROCESS_H__