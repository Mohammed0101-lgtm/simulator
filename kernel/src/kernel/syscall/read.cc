#include "syscall/read.h"
#include <cstring>


namespace kernel {
namespace syscall {

template<typename _Tp>
_Tp sys_read(const std::size_t addr) {
    _Tp value;
    std::memcpy(&value, &simulated_memory[addr], sizeof(_Tp));
    return value;
}

}  // namespace syscall
}  // namespace kernel