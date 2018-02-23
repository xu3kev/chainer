#include "xchainer/device.h"

#include <type_traits>

#include "xchainer/error.h"

namespace xchainer {
namespace {

thread_local Device* t_default_device = nullptr;
static_assert(std::is_pod<decltype(t_default_device)>::value, "t_default_device must be POD");

}  // namespace

namespace internal {

Device* GetDefaultDeviceNoExcept() noexcept { return t_default_device; }

}  // namespace internal

Device& GetDefaultDevice() {
    if (t_default_device == nullptr) {
        throw XchainerError("Default device_id is not set.");
    }
    return *t_default_device;
}

void SetDefaultDevice(Device* device) {
    if (device != nullptr && &device->backend().context() != &GetDefaultContext()) {
        throw ContextError("Context mismatch between default device and default context.");
    }
    t_default_device = device;
}

}  // namespace xchainer
