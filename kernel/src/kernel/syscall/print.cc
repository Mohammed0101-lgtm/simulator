#include "print.h"
#include <iostream>

namespace kernel {
namespace syscall {

void sys_print(const std::string string_literal) { std::cout << string_literal; }

}  // syscall
}  // kernel