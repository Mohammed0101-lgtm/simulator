#ifndef __EXECUTE_H__
#define __EXECUTE_H__

#include "../memory/memory.h"
#include "register.h"
#include <inttypes.h>


namespace kernel {
namespace cpu {


class SimulatedCPU
{
   public:
    Register _regs[8];  // R0–R7
    uint32_t _pc;       // Program Counter
    uint32_t _sp;       // Stack Pointer
    bool     _halted = false;

    SimulatedCPU();

    void tick();             // Execute one instruction
    void run_until_halt();   // Run until HALT
    void syscall_handler();  // Handle R0 syscall ID
};


}  // namespace cpu
}  // namespace kernel


#endif  // __EXECUTE_H__