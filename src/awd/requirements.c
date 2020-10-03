#include <awd/awd.h>
#include <awd/logging.h>

extern int check_cpuid();
extern int check_longmode();

void awd_cpu_requirements() {
    if(check_cpuid() == 0) {
        log_error("CPU does not support CPUID. Unable to boot...");
        awd_halt();
    }

    if(check_longmode() == 0) {
        log_error("CPU does not support longmode. Unable to boot...");
        awd_halt();
    }
}