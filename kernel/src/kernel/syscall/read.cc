#include "syscall/read.hh"
#include "memory/memory.hh"
#include <cstring>


namespace kernel {
namespace syscall {

template<typename _Tp>
_Tp sys_read(const std::size_t addr) {
    _Tp value;
    std::memcpy(&value, &kernel::memo::memory_manager._simulated_memory[addr], sizeof(_Tp));
    return value;
}

}  // namespace syscall
}  // namespace kernel