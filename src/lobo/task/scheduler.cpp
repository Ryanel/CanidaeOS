#include <assert.h>
#include <kernel/heap.h>
#include <kernel/kernel.h>
#include <kernel/log.h>
#include <kernel/scheduler.h>
#include <kernel/task.h>
#include <kernel/vmm.h>

using namespace kernel::scheduling;

extern "C" void switch_thread(ThreadControlBlock* newThread, ThreadControlBlock* oldThread);
extern "C" void switch_task_vas(TaskControlBlock* newTask, TaskControlBlock* oldTask);

static Scheduler kernel_sched;

// TODO: Fix
static int next_tid = 1;

Scheduler& Scheduler::Get() { return kernel_sched; }

void Scheduler::PrintThreadInfo(ThreadControlBlock* tcb) {
    assert(tcb != nullptr);

    auto& kLog = log::Get();
    kLog.Log("sched", "Thread %s: TID %d, Stack: 0x%016p", tcb->threadName, tcb->threadID, tcb->stack_top);
}

void Scheduler::Init() {
    // Initialise the kernel task
    kernelTask.state    = TCBState::Running;
    kernelTask.taskName = "Lobo Kernel";
    kernelTask.vas      = MEM_VIRT_TO_PHYS(vmm().get().pdir_kernel);
    kernelTask.taskID   = 0;
    tasks.push_back(&kernelTask);

    // Setup the kernel idle task...
    idleThread.stack_top  = 0;  // RSP will be filled in from the task switching function
    idleThread.threadName = "k/idle";
    idleThread.taskID     = 0;  // Owned by the kernel...

    // We're currently in the "idle task"
    currentThread = &idleThread;
    currentTask = &kernelTask;

    SwitchTask(&kernelTask);

    SwitchThread(&idleThread);  // Switch to populate some values.
    threads.push_back(&idleThread);
}

void Scheduler::SwitchTask(TaskControlBlock* task) {
    assert(task != nullptr);

    TaskControlBlock * curTask = currentTask;
    currentTask = task;

    switch_task_vas(task, curTask);
}

void Scheduler::SwitchThread(ThreadControlBlock* newThread) {
    assert(newThread != nullptr);

    ThreadControlBlock* curThread = currentThread;
    currentThread                 = newThread;

    switch_thread(newThread, curThread);
    // Function will return here once control is given back...
}

ThreadControlBlock* Scheduler::CreateThread(const char* name, void* function) {
    ThreadControlBlock* toCreate = (ThreadControlBlock*)kmalloc(sizeof(ThreadControlBlock));

    log::Get().Log("thread", "Creating thread %s => 0x%16p", name, function);

    uint64_t stackAddr = (uint64_t)kmalloc(THREAD_STACK_SIZE) + THREAD_STACK_SIZE;

    toCreate->threadName = name;                      // Set name
    toCreate->stack_top  = stackAddr - (0x8 * 0x10);  // Set rsp, as if we're in the middle of switch_threadr_kernel);
    toCreate->threadID   = next_tid++;  // TODO: Fix to be concurrency safe.
    toCreate->state      = TCBState::Ready;
    toCreate->taskID = currentTask->taskID;

    uint64_t* newStack = (uint64_t*)(toCreate->stack_top);
    newStack[0x00]     = 0;                    // r15
    newStack[0x01]     = 0;                    // r14
    newStack[0x02]     = 0;                    // r13
    newStack[0x03]     = 0;                    // r12
    newStack[0x04]     = 0;                    // r11
    newStack[0x05]     = 0;                    // r10
    newStack[0x06]     = 0;                    // r9
    newStack[0x07]     = 0;                    // r8
    newStack[0x08]     = 0;                    // rdi
    newStack[0x09]     = 0;                    // rsi
    newStack[0x0A]     = toCreate->stack_top;  // rbp
    newStack[0x0B]     = 0;                    // rbx
    newStack[0x0C]     = 0;                    // rdx
    newStack[0x0D]     = 0;                    // rcx
    newStack[0x0E]     = 0;                    // rax
    newStack[0x0F]     = (uint64_t)function;   // rip

    threads.push_back(toCreate);
    return toCreate;
}

void Scheduler::EnableScheduling() { schedulingEnabled = true; }
void Scheduler::DisableScheduling() { schedulingEnabled = false; }

void Scheduler::Schedule() {
    if (!schedulingEnabled) { return; }
    unsigned int tries    = 0;
    const int    maxTries = 500;
    // Find a canidate thread...
    while (true) {
        if (++currentThreadIndex >= threads.size()) { currentThreadIndex = 0; }
        auto new_thread = threads.at(currentThreadIndex);
        if (new_thread->state != TCBState::Ready) {
            tries++;
            if (tries > maxTries) { return; }  // Give up. Can happen when it's only the idle thread.
            continue;
        }
        // Critical Section Start
        currentThread->state = TCBState::Ready;
        new_thread->state    = TCBState::Running;
        SwitchThread(threads.at(currentThreadIndex));
        // Critical Section End...
        break;
    }
}

void Scheduler::Yield() {
    // TODO: Set thread remaining time to 0 to force an instant switch.
    Schedule();
}