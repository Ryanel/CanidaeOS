#pragma once

#define KERNEL_FEATURE_DEBUGGER

namespace kernel {
namespace debug {

class debugger {
   public:
    static debugger& get();

    void enter();

#ifdef KERNEL_FEATURE_DEBUGGER

#endif
};

}  // namespace debug
}  // namespace kernel