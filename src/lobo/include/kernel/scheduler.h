#pragma once

#include <common/single_linked_list.h>
#include <kernel/task.h>
namespace kernel {
namespace scheduling {

class Scheduler {
   public:
    bool                schedulingEnabled = false;
    ThreadControlBlock  idleThread;
    ThreadControlBlock* currentThread;

    // TODO: Implement Round-Robin scheduling in a better way.
    // TODO: Add process support.
    common::single_linked_list<ThreadControlBlock*> threads;
    size_t                                          currentThreadIndex = 0;

   public:
    static Scheduler&   Get();
    void                Init();
    void                OnTick();
    ThreadControlBlock* CreateThread(const char* name, void* function);
    void                SwitchThread(ThreadControlBlock* newThread);
    void                PrintTaskInfo(ThreadControlBlock* tcb);
    void                EnableScheduling();
    void                DisableScheduling();
    void                Schedule();  ///< Switch to the next thread.
    void                Yield();     ///< Yields the rest of this threads time
};
}  // namespace scheduling
}  // namespace kernel