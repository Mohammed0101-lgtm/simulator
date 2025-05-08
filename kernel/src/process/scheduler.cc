#include "scheduler.h"


namespace kernel {
namespace proc {


void Scheduler::add_process(std::unique_ptr<Process> process) {}

void Scheduler::block_current_process() {}

void Scheduler::unblock_process(PID pid) {}

void Scheduler::run(int time_slices) {  // main simulation loop
    for (int tick = 0; tick < time_slices; ++tick)
    {
        if (this->_ready_queue.empty())
        {
            std::cout << "[Scheduler] No process ready to run!\n";
            break;
        }

        std::unique_ptr<Process> process = std::move(this->_ready_queue.front());
        this->_ready_queue.pop();

        PCB& pcb = *process->process_control_block;
        pcb.setState(ProcessState::RUNNING);

        std::cout << "[Scheduler] Running process of PID : " << pcb.getPID() << "\n";

        // Simulate process executing
        // For now, just assume it completes after one slice
        pcb.setState(ProcessState::TERMINATED);

        std::cout << "[Scheduler] Process PID : " << pcb.getPID() << " terminated.\n";
        // If you want to support yielding or time slicing, requeue instead of terminating
    }
}


}  // namepace proc
}  // namespace kernel
