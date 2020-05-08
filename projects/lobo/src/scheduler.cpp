#include "kernel/scheduler.h"
#include <assert.h>
#include "kconfig.h"
#include "kernel/heap.h"
#include "kernel/kernel.h"
#include "kernel/log.h"
#include "kernel/task.h"
#include "kernel/vmm.h"

using namespace Kernel;

extern "C" void switch_thread(ThreadControlBlock* newThread, ThreadControlBlock* oldThread);

static Scheduler kernel_sched;

Scheduler& Scheduler::Get() { return kernel_sched; }

void Scheduler::PrintTaskInfo(ThreadControlBlock* tcb) {
    assert(tcb != nullptr);

    auto& kLog = KernelLog::Get();
    kLog.Log("sched", "Task %s: VAS: 0x%016p, Stack: 0x%016p", tcb->taskName, tcb->vas, tcb->stack_top);
}

void Scheduler::Init() {
    // Setup the kernel idle task...
    idleThread.stack_top = 0;  // RSP will be filled in from the task switching function
    idleThread.taskName  = "k/idle";
    idleThread.vas       = MEM_VIRT_TO_PHYS(Kernel::VMM().Get().kernelP4);

    // We're currently in the "idle task"
    currentThread = &idleThread;
    SwitchThread(&idleThread);  // Switch to populate some values.
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

    KernelLog::Get().Log("thread", "Creating thread %s => 0x%16p", name, function);

    uint64_t stackAddr = (uint64_t)kmalloc(THREAD_STACK_SIZE) + THREAD_STACK_SIZE;

    toCreate->taskName  = name;                      // Set name
    toCreate->stack_top = stackAddr - (0x8 * 0x10);  // Set rsp, as if we're in the middle of switch_thread
    toCreate->vas       = MEM_VIRT_TO_PHYS(Kernel::VMM().Get().kernelP4);

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
    return toCreate;
}

void Scheduler::EnableScheduling() { schedulingEnabled = true; }
void Scheduler::DisableScheduling() { schedulingEnabled = false; }

void Scheduler::Schedule() {
    if (schedulingEnabled && currentThread != nullptr ) {
        assert(currentThread->next != nullptr);
        SwitchThread(currentThread->next); 
    }
}