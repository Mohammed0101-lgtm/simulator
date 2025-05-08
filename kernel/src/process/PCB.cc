#include "PCB.h"


namespace kernel {
namespace proc {

// Getters
ProcessState PCB::getState() const { return _state; }
int          PCB::getPID() const { return _pid; }
int          PCB::getPriority() const { return _priority; }
int          PCB::getPC() const { return _pc; }
int          PCB::getSP() const { return _sp; }
int          PCB::getBase() const { return _base; }
int          PCB::getLimit() const { return _limit; }
int*         PCB::getStack() const { return _stack; }
std::size_t  PCB::getStackSize() const { return _stackSize; }

// setters
void PCB::setStackSize(std::size_t stackSize) {
    delete[] _stack;  // Free the old stack memory
    _stackSize = stackSize;
    _stack     = new int[stackSize];  // Allocate new memory for the stack
}

void PCB::setPID(PID pid) { _pid = pid; }
void PCB::setState(ProcessState state) { _state = state; }
void PCB::setPriority(int priority) { _priority = priority; }
void PCB::setPC(int pc) { _pc = pc; }
void PCB::setSP(int sp) { _sp = sp; }
void PCB::setBase(int base) { _base = base; }
void PCB::setLimit(int limit) { _limit = limit; }

};  // namespace proc
};  // namespace kernel