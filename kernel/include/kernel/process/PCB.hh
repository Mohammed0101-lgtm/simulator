#ifndef __PCB_H__
#define __PCB_H__

#include "io/file.hh"

#include <iostream>


namespace kernel {
namespace proc {

struct PID
{
    int _value;

    // Constructors
    PID(int v) :
        _value(v) {}

    // Equality operators
    bool operator==(const PID& other) const { return this->_value == other._value; }
    bool operator!=(const PID& other) const { return this->_value != other._value; }
};

enum class ProcessState : int {
    NEW,        // Process is being created
    READY,      // Process is ready to run
    RUNNING,    // Process is currently executing
    WAITING,    // Process is waiting for an event to occur
    TERMINATED  // Process has finished execution
};

enum class FileType {
    REGULAR,
    DIRECTORY,
    CHAR,
    BLOCK,
    FIFO,
    SOCKET
};

enum class FileMode {
    READ,
    WRITE,
    APPEND,
    READ_WRITE
};

enum class FilePermission {
    READ,
    WRITE,
    EXECUTE
};

enum class FileStatus {
    OPEN,
    CLOSED,
    ERROR
};

enum class FileFlag {
    O_RDONLY,
    O_WRONLY,
    O_RDWR,
    O_APPEND,
    O_CREAT,
    O_TRUNC,
    O_EXCL
};

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
    mutable std::unordered_map<int, std::shared_ptr<kernel::io::File>> _fd_table;
    int _next_fd = 3;  // File descriptor table, starting from 3 (0, 1, 2 are reserved for stdin, stdout, stderr)

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