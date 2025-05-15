#ifndef __CLOSE_H__
#define __CLOSE_H__

#include "../process/process.hh"


namespace kernel {
namespace syscall {

void sys_close(kernel::proc::Process& proc, int fd);

}  // namespace syscall
}  // namespace kernel


#endif  // __CLOSE_H__