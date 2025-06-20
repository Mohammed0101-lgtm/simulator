#include "process/PCB.hh"


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

    this->_fd_table[0] = std::make_shared<kernel::io::ConsoleFile>();  // stdin
    this->_fd_table[1] = std::make_shared<kernel::io::ConsoleFile>();  // stdout
    this->_fd_table[2] = std::make_shared<kernel::io::ConsoleFile>();  // stderr
}

// Getters
ProcessState kernel::proc::PCB::getState() const { return this->_state; }
PID          kernel::proc::PCB::getPID() const { return this->_pid; }
int          kernel::proc::PCB::getPriority() const { return this->_priority; }
int          kernel::proc::PCB::getPC() const { return this->_pc; }
int          kernel::proc::PCB::getSP() const { return this->_sp; }
int          kernel::proc::PCB::getBase() const { return this->_base; }
int          kernel::proc::PCB::getLimit() const { return this->_limit; }
int*         kernel::proc::PCB::getStack() const { return this->_stack; }
std::size_t  kernel::proc::PCB::getStackSize() const { return this->_stackSize; }

// setters
void kernel::proc::PCB::setPID(const kernel::proc::PID pid) { this->_pid = pid; }
void kernel::proc::PCB::setState(const kernel::proc::ProcessState state) { this->_state = state; }
void kernel::proc::PCB::setPriority(const int priority) { this->_priority = priority; }
void kernel::proc::PCB::setPC(const int pc) { this->_pc = pc; }
void kernel::proc::PCB::setSP(const int sp) { this->_sp = sp; }
void kernel::proc::PCB::setBase(const int base) { this->_base = base; }
void kernel::proc::PCB::setLimit(const int limit) { this->_limit = limit; }
void kernel::proc::PCB::setStackSize(std::size_t stackSize) {
    delete[] _stack;  // Free the old stack memory
    this->_stackSize = stackSize;
    this->_stack     = new int[stackSize];  // Allocate new memory for the stack
}

}  // namespace proc
}  // namespace kernel