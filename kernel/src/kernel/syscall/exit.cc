#include "syscall/exit.hh"
#include <cstdlib>


namespace kernel {
namespace syscall {

void sys_exit(int status) { std::exit(status); }

}  // namespace syscall
}  // namespace kernel