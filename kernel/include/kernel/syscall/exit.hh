#ifndef __EXIT_H__
#define __EXIT_H__


namespace kernel {
namespace syscall {

void sys_exit(int status);

}  // namespace syscall
}  // namespace kernel

#endif  // __EXIT_H__