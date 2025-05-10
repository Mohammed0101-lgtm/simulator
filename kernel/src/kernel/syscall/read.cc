#include "read.h"
#include <cstring>


namespace kernel {
namespace syscall {

template<typename _Tp>
_Tp sys_read(const std::size_t addr) {
    _Tp value;
    std::memcpy(&value, &simulated_memory[addr], sizeof(_Tp));
    return value;
}

// Explicit template instantiations
template uint8_t  read<uint8_t>(std::size_t);
template uint16_t read<uint16_t>(std::size_t);
template uint32_t read<uint32_t>(std::size_t);

}  // namespace syscall
}  // namespace kernel