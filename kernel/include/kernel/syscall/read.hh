#ifndef __READ_H__
#define __READ_H__

#include <memory>


namespace kernel {
namespace syscall {

template<typename _Tp>
_Tp sys_read(const std::size_t addr);

// Explicit template instantiations
template uint8_t  sys_read<uint8_t>(std::size_t);
template uint16_t sys_read<uint16_t>(std::size_t);
template uint32_t sys_read<uint32_t>(std::size_t);

}  // namespace syscall
}  // namespace kernel


#endif  // __READ_H__