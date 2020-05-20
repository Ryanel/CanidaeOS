#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include <kernel/debug/kernel_debugger.h>
#include <kernel/driver_registry.h>
#include <kernel/mem/memmap.h>
#include "kernel/cpu.h"
#include "kernel/heap.h"
#include "kernel/kernel.h"
#include "kernel/log.h"
#include "kernel/pmm.h"
#include "kernel/scheduler.h"
#include "kernel/vmm.h"
#include <kernel/driver.h>
using namespace kernel;

void kernel_main() {
    auto& kLog                 = log::Get();
    auto& kernelPmm            = kernel::pmm::get();
    auto& kernelVmm            = kernel::vmm::get();
    auto& kernelSched          = kernel::scheduling::Scheduler::Get();
    auto& kernelDriverRegistry = kernel::drivers::driver_registry::get();

    kLog.Log("lobo", "Entered Kernel Main");

    kernelPmm.debug_print_free_memory();  // Print how much memory was used
    kernelSched.Init();                   // Initialise the scheduler

    // Print loaded drivers.
    auto& drv_list = kernelDriverRegistry.driver_list;
    auto drv_node = drv_list.begin();
    kLog.Log("drvreg", "%d Drivers loaded:", kernelDriverRegistry.driver_list.m_size);
    while(drv_node != nullptr) {
        assert(drv_node->value != nullptr);
        kLog.LogRaw("%s ", drv_node->value->m_name);
        drv_node = drv_node->next;
    }
    kLog.LogRaw("\n");

    cpu::IdleLoop();
    debug::debugger().get().enter();
}