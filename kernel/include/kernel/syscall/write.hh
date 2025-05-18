#ifndef __WRITE_H__
#define __WRITE_H__

#include "../process/process.hh"
#include <cstddef>
#include <cstdint>


namespace kernel {
namespace syscall {

ssize_t sys_write(kernel::proc::Process& proc, int fd, const char* buffer, std::size_t size);

}  // namespace syscall
}  // namespace kernel


#endif  // __WRITE_H__