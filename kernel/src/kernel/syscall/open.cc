#include "syscall/open.h"


namespace kernel {
namespace syscall {

int sys_open(kernel::proc::Process& proc, const std::string& path, int flags) {
    // Stub: Always return a memory file or console file
    int fd                 = proc._process_control_block->_next_fd++;
    proc._process_control_block->_fd_table[fd] = std::make_shared<kernel::proc::ConsoleFile>();
    return fd;
}

}  // namespace syscall
}  // namespace kernel