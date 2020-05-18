#include "kernel/vmm.h"

#include <stddef.h>
#include <stdint.h>

/** This function is supposed to lock the memory data structures. It
 * could be as simple as disabling interrupts or acquiring a spinlock.
 * It's up to you to decide. 
 *
 * \return 0 if the lock was acquired successfully. Anything else is
 * failure.
 */
extern "C" int liballoc_lock() {
    // TODO: Fix
    asm("cli");
    return 0;
}

/** This function unlocks what was previously locked by the liballoc_lock
 * function.  If it disabled interrupts, it enables interrupts. If it
 * had acquiried a spinlock, it releases the spinlock. etc.
 *
 * \return 0 if the lock was successfully released.
 */
extern "C" int liballoc_unlock() {
    // TODO: Fix
    asm("sti");
    return 0;
}

/** This is the hook into the local system which allocates pages. It
 * accepts an integer parameter which is the number of pages
 * required.  The page size was set up in the liballoc_init function.
 *
 * \return NULL if the pages were not allocated.
 * \return A pointer to the allocated memory.
 */
extern "C" void* liballoc_alloc(size_t n) {
    return kernel::vmm::get().map(nullptr, n*0x1000, VMM_PAGE_PRESENT | VMM_PAGE_WRITE, VMM_MAP_KERNEL);
}

/** This frees previously allocated memory. The void* parameter passed
 * to the function is the exact same value returned from a previous
 * liballoc_alloc call.
 *
 * The integer value is the number of pages to free.
 *
 * \return 0 if the memory was successfully freed.
 */
extern "C" int liballoc_free(void* addr,size_t n) {
    kernel::vmm::get().unmap(addr, n * 0x1000);
    return 0;
}