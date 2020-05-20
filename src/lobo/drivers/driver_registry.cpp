#include <kernel/driver.h>
#include <kernel/driver_registry.h>

using namespace kernel;
using namespace kernel::drivers;

static driver_registry instance_driver_registry;

driver_registry& driver_registry::get() {
    return instance_driver_registry;
}

void driver_registry::add(driver* drv) {
    driver_list.push_back(drv);
}