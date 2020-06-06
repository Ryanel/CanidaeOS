#pragma once

#include <common/single_linked_list.h>
#include <kernel/task.h>
namespace kernel {
namespace scheduling {

class Scheduler {
   public:
    /// Threads --------------------------------------------------
    ThreadControlBlock* CreateThread(const char* name, void* function);
    void                SwitchThread(ThreadControlBlock* newThread);
    void                PrintThreadInfo(ThreadControlBlock* tcb);

    /// Tasks ----------------------------------------------------
    void SwitchTask(TaskControlBlock* task);

    // Scheduling ------------------------------------------------
    static Scheduler& Get();

    void Init();
    void OnTick();
    void EnableScheduling();
    void DisableScheduling();
    void Schedule();  ///< Switch to the next thread.
    void Yield();     ///< Yields the rest of this threads time.

   public:
    bool schedulingEnabled = false;

    // Threads
    ThreadControlBlock                              idleThread;
    ThreadControlBlock*                             currentThread;
    common::single_linked_list<ThreadControlBlock*> threads;

    // Tasks
    common::single_linked_list<TaskControlBlock*> tasks;
    TaskControlBlock                              kernelTask;
    TaskControlBlock*                             currentTask;

   private:
    size_t currentThreadIndex = 0;
};
}  // namespace scheduling
}  // namespace kernel