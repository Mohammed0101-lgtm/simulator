#ifndef __EXIT_H__
#define __EXIT_H__


namespace kernel {
namespace syscall {

void __exit__(int status);

}  // namespace syscall
}  // namespace kernel

#endif  // __EXIT_H__