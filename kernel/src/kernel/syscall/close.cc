#include "syscall/close.hh"


namespace kernel {
namespace syscall {

void sys_close(kernel::proc::Process& proc, int fd) {
    auto it = proc._process_control_block->_fd_table.find(fd);
    if (it != proc._process_control_block->_fd_table.end())
    {
        it->second->close();
        proc._process_control_block->_fd_table.erase(fd);
    }
}

}  // namespace syscall
}  // namespace kernel