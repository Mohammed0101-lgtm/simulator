#include "syscall/malloc.hh"


namespace kernel {
namespace syscall {

template<typename _Tp>
std::unique_ptr<_Tp> sys_malloc(const std::size_t size) noexcept {
    return std::unique_ptr<_Tp>(new (std::nothrow) _Tp[size]);
}

}  // namespace syscall
}  // namespace kernel