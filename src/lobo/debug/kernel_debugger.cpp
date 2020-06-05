#include <kernel/debug/kernel_debugger.h>
#include <kernel/driver.h>
#include <kernel/driver_registry.h>
#include <kernel/hal/keyboard.h>
#include <kernel/log.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <kernel/scheduler.h>
#include <string.h>

using namespace kernel;
using namespace kernel::debug;

static debugger debugger_ins;

struct debugger_func {
    const char* cmd;
    void (*fn)();
};

#ifdef KERNEL_FEATURE_DEBUGGER
void debugger::enter() {
    log&  klog                 = log::Get();
    //auto& kernelPmm            = kernel::pmm::get();
    //auto& kernelVmm            = kernel::vmm::get();
    //auto& kernelDriverRegistry = kernel::drivers::driver_registry::get();
    auto& kernelSched          = kernel::scheduling::Scheduler::Get();

    kernelSched.DisableScheduling();
    
    klog.Log("debug", "Entered kernel debugger!");

    const int     num_commands = 4;
    debugger_func commands[num_commands];
    commands[0] = {"pmm/free", []() { kernel::pmm::get().debug_print_free_memory(); }};
    commands[1] = {"pmm/freepages", []() { kernel::pmm::get().debug_print_free_pages(); }};
    commands[2] = {"drivers/count", []() {
                       kernel::log::Get().Log("drvreg", "%d Drivers loaded",
                                              kernel::drivers::driver_registry::get().driver_list.m_size);
                   }};
    commands[3] = {"drivers/list", []() {
                       auto& drv_list = kernel::drivers::driver_registry::get().driver_list;
                       auto  drv_node = drv_list.begin();
                       while (drv_node != nullptr) {
                           assert(drv_node->value != nullptr);
                           log::Get().LogRaw("%s ", drv_node->value->m_name);
                           drv_node = drv_node->next;
                       }
                       log::Get().LogRaw("\n");
                   }};

    while (true) {
        // Input
        
        char input_buffer[100];
        int  input_index = 0;
        memset(input_buffer, 0, 100);

        klog.LogRaw("    debug > ");

        while (true) {
            char c = kernel::hal::keyboard::get().getchar_async();
            if (c == '\n') {
                klog.WriteChar(c);
                break;  // We're done with input!
            } else if (c == '\b') {
                klog.WriteChar('\b');
                input_buffer[input_index] = 0;
                input_index--;
                if (input_index < 0) { input_index = 0; }
            } else if (c == 0) {
                // No input, try again.
            } else {
                if(input_index > 98) {input_index = 98;}
                input_buffer[input_index++] = c;
                klog.WriteChar(c);
            }
        }

        // We have now gotten some input, try and process it.
        if (strcmp(input_buffer, "exit") == 0) {
            return;
        } else if (strcmp(input_buffer, "help") == 0) {
            klog.Log("debug", "Command List:");
            klog.Log("debug", "pmm: pmm/free, pmm/freepages,");
            klog.Log("debug", "drivers: drivers/count, drivers/list");
            klog.Log("debug", "debugger: help, exit");
        } else {
            // Search through commands
            debugger_func* func = nullptr;
            for (int i = 0; i < num_commands; i++) {
                if (strcmp(input_buffer, commands[i].cmd) == 0) {
                    func = &commands[i];
                    break;
                }
            }
            if (func != nullptr) {
                func->fn();
            } else {
                klog.Log("debug", "Invalid command \"%s\"", input_buffer);
            }
        }
    }
}
#else
void debugger::enter() {
    // Nothing happens.
}
#endif

debugger& debugger::get() { return debugger_ins; }