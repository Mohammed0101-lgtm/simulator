#ifndef __PCB_H__
#define __PCB_H__

#include <iostream>


namespace kernel {
namespace proc {

struct PID
{
    int value;

    // Constructors
    PID(int v) :
        value(v) {}

    // Equality operators
    bool operator==(const PID& other) const { return value == other.value; }
    bool operator!=(const PID& other) const { return value != other.value; }
};

typedef enum: int {
    NEW,        // Process is being created
    READY,      // Process is ready to run
    RUNNING,    // Process is currently executing
    WAITING,    // Process is waiting for an event to occur
    TERMINATED  // Process has finished execution
} ProcessState;

class PCB
{
   protected:
    PID          _pid;                             // Process ID
    int          _priority = 0;                    // Process priority
    ProcessState _state    = ProcessState::READY;  // Process state (e.g., running, waiting, etc.)
    int          _pc;                              // Program counter
    int          _sp;                              // Stack pointer
    int          _base;                            // Base address of the process's memory segment
    int          _limit;                           // Limit address of the process's memory segment
    int*         _stack;                           // Pointer to the process's stack
    std::size_t  _stackSize;                       // Size of the process's stack

   public:
    PCB(int pid, int priority, int pc, int sp, int base, int limit, std::size_t stackSize);

    ~PCB() {
        delete[] _stack;  // Free the allocated memory for the stack
    }

    // Getters
    ProcessState getState() const;
    PID          getPID() const;
    int          getPriority() const;
    int          getPC() const;
    int          getSP() const;
    int          getBase() const;
    int          getLimit() const;
    int*         getStack() const;
    std::size_t  getStackSize() const;

    // Setters
    void setPID(const PID pid);
    void setStackSize(const std::size_t stackSize);
    void setState(const ProcessState state);
    void setPriority(const int priority);
    void setPC(const int pc);
    void setSP(const int sp);
    void setBase(const int base);
    void setLimit(const int limit);
};

}  // namespace proc
}  // namespace kernel


#endif  // __PCB_H__