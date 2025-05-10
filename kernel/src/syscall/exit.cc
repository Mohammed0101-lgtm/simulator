#include "exit.h"
#include <stdlib.h>


namespace kernel {
namespace syscall {

void __exit__(int status) { exit(status); }

}  // namespace syscall
}  // namespace kernel