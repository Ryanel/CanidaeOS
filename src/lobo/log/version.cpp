#include "kernel/log.h"

using namespace kernel;

void kernel_version_print() {
    auto& kernelLog = log::Get();
    kernelLog.LogRaw(" +----------------------------------------------------------------------------+\n");
    kernelLog.LogRaw("%26sThe Lobo Kernel (v 0.0.0.1)\n","");
    kernelLog.LogRaw(" +-------+--------------------------------------------------------------------+\n");
}