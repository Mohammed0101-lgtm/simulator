#ifndef __PRINT_H__
#define __PRINT_H__

#include <string>


namespace kernel {
namespace syscall {

void print(const std::string string_literal);

}  // namespace kernel
}  // namespace syscall

#endif  // __PRINT_H__