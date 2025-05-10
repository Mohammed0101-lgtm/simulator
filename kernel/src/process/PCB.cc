#include "PCB.h"


namespace kernel {
namespace proc {

// Constructor
kernel::proc::PCB::PCB(int pid, int priority, int pc, int sp, int base, int limit, std::size_t stackSize) :
    _pid(pid),
    _priority(priority),
    _state(kernel::proc::ProcessState::NEW),
    _pc(pc),
    _sp(sp),
    _base(base),
    _limit(limit),
    _stackSize(stackSize) {
    _stack = new int[stackSize];  // Allocate memory for the stack
}

// Getters
ProcessState kernel::proc::PCB::getState() const { return _state; }
PID          kernel::proc::PCB::getPID() const { return _pid; }
int          kernel::proc::PCB::getPriority() const { return _priority; }
int          kernel::proc::PCB::getPC() const { return _pc; }
int          kernel::proc::PCB::getSP() const { return _sp; }
int          kernel::proc::PCB::getBase() const { return _base; }
int          kernel::proc::PCB::getLimit() const { return _limit; }
int*         kernel::proc::PCB::getStack() const { return _stack; }
std::size_t  kernel::proc::PCB::getStackSize() const { return _stackSize; }

// setters
void kernel::proc::PCB::setPID(const kernel::proc::PID pid) { _pid = pid; }
void kernel::proc::PCB::setState(const kernel::proc::ProcessState state) { _state = state; }
void kernel::proc::PCB::setPriority(const int priority) { _priority = priority; }
void kernel::proc::PCB::setPC(const int pc) { _pc = pc; }
void kernel::proc::PCB::setSP(const int sp) { _sp = sp; }
void kernel::proc::PCB::setBase(const int base) { _base = base; }
void kernel::proc::PCB::setLimit(const int limit) { _limit = limit; }
void kernel::proc::PCB::setStackSize(std::size_t stackSize) {
    delete[] _stack;  // Free the old stack memory
    _stackSize = stackSize;
    _stack     = new int[stackSize];  // Allocate new memory for the stack
}

}  // namespace proc
}  // namespace kernel