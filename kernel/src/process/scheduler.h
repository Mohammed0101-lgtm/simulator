#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "process.h"
#include <memory>
#include <queue>
#include <vector>


namespace kernel {
namespace proc {

using Queue      = std::queue<std::unique_ptr<Process>>;
using BlockQueue = std::vector<std::unique_ptr<Process>>;


class Scheduler
{
   private:
    Queue      _ready_queue;
    BlockQueue _blocked_queue;

   public:
    void add_process(std::unique_ptr<Process> process);
    void block_current_process();
    void unblock_process(PID pid);
    void run(int time_slices);  // main simulation loop
};

};
};


#endif  // __SCHEDULER_H__