#pragma once

#include <common/single_linked_list.h>

namespace kernel {
namespace drivers {

class driver;

class driver_registry {
    public:
    common::single_linked_list<driver*> driver_list;
    static driver_registry& get();
    void add(driver* driver);
};
}
}