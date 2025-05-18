#include "process/scheduler.hh"


namespace kernel {
namespace proc {


void kernel::proc::Scheduler::add_process(std::unique_ptr<kernel::proc::Process> process) {
    // TODO : implement this
}

void kernel::proc::Scheduler::block_current_process() {
    // TODO : implement this
}

void kernel::proc::Scheduler::unblock_process(kernel::proc::PID pid) {
    // TODO : implement this
}

void kernel::proc::Scheduler::run() {  // main simulation loop
    for (;;)
    {
        if (this->_ready_queue.empty())
        {
            std::cout << "[Scheduler] No process ready to run!\n";
            break;
        }

        std::unique_ptr<kernel::proc::Process> process = std::move(this->_ready_queue.front());
        this->_ready_queue.pop();

        kernel::proc::PCB& pcb = *process->_process_control_block;
        pcb.setState(kernel::proc::ProcessState::RUNNING);

        std::cout << "[Scheduler] Running process of PID : " << pcb.getPID()._value << "\n";

        // Simulate process executing
        // For now, just assume it completes after one slice (one clock tick)
        pcb.setState(kernel::proc::ProcessState::TERMINATED);

        std::cout << "[Scheduler] Process PID : " << pcb.getPID()._value << " terminated.\n";
        // If you want to support yielding or time slicing, requeue instead of terminating
    }
}


}  // namepace proc
}  // namespace kernel
