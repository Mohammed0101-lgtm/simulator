#include "syscall/write.h"


namespace kernel {
namespace syscall {

ssize_t sys_write(kernel::proc::Process& proc, int fd, const char* buffer, size_t size) {
    auto it = proc.process_control_block->_fd_table.find(fd);
    if (it == proc.process_control_block->_fd_table.end())
        return -1;
        
    return it->second->write(buffer, size);
}

}  // namespace syscall
}  // namespace kernel