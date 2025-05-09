#ifndef __READ_H__
#define __READ_H__

#include <memory>


namespace kernel {
namespace syscall {

template<typename _Tp>
_Tp read(const std::size_t addr);

}  // namespace syscall
}  // namespace kernel


#endif  // __READ_H__