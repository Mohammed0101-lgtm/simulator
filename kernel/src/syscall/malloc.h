#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <memory>


namespace kernel {
namespace syscall {

template<typename _Tp>
std::unique_ptr<_Tp> _malloc(const std::size_t size) noexcept;

}  // namespace syscall
}  // namespace kernel

#endif  // __MALLOC_H__