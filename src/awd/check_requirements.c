#include "awd/console.h"
#include "awd/error.h"

int check_cpuid();
int check_longmode();

void awd_check_requirements() {
    // CPUID
    if (check_cpuid() != 1) {
        panic("Unsupported hardware, no CPUID");
    }
    if (check_longmode() != 1) {
        panic("Unsupported hardware, no Longmode");
    }
}