#include <kernel/debug/kernel_debugger.h>
#include <kernel/log.h>

using namespace kernel;
using namespace kernel::debug;

static debugger debugger_ins;

#ifdef KERNEL_FEATURE_DEBUGGER
void debugger::enter() {
    log& klog = log::Get();
    klog.Log("debug", "Entered kernel debugger!");
    klog.LogRaw("> ");
}
#else 
void debugger::enter() {
    // Nothing happens.
}
#endif

debugger& debugger::get() {
    return debugger_ins;
}