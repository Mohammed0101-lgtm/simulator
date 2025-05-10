#ifndef __OPEN_H__
#define __OPEN_H__

#include "../process/process.h"


namespace kernel {
namespace syscall {

int sys_open(kernel::proc::Process& proc, const std::string& path, int flags);

}  // namespace syscall
}  // namespace kernel


#endif  // __OPEN_H__