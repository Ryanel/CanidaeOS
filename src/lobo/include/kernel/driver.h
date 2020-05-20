#pragma once

namespace kernel {
namespace drivers {

enum class driver_type : int { IN_KERNEL, LOADABLE };

class driver {
   public:
    driver(const char* name, driver_type drv_type) : m_name(name), m_type(drv_type) {}
    virtual void init();
    virtual void call();
    const char*  m_name;
    driver_type  m_type;
};
}  // namespace drivers
}  // namespace kernel