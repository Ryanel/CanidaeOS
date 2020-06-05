#pragma once

#include "kernel/task.h"
#include <common/linked_list.h>
namespace kernel {
namespace scheduling {

class Scheduler {
   public:
    bool schedulingEnabled = false;
    ThreadControlBlock  idleThread;
    ThreadControlBlock* currentThread;

    // TODO: Implement Round-Robin scheduling in a better way.
    // TODO: Add process support.
    common::linked_list<ThreadControlBlock*> threads;
    int currentThreadIndex = 0;

   public:
    static Scheduler&   Get();
    void                Init();
    void                OnTick();
    ThreadControlBlock* CreateThread(const char* name, void* function);
    void                SwitchThread(ThreadControlBlock* newThread);
    void                PrintTaskInfo(ThreadControlBlock* tcb);
    void                EnableScheduling();
    void                DisableScheduling();
    void                Schedule();  // Switch to the next thread.
};
}  // namespace Scheduling
}  // namespace Kernel