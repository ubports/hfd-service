#include <assert.h>
#include <string>
#include <stdexcept>

#include "udev-cpp.h"

namespace Udev
{
	Udev::Udev() : handle(udev_new())
	{
	}

	Udev::Udev(const Udev &other) : handle(udev_ref(other.handle))
	{
	}

	Udev::Udev(Udev &&other) : handle(other.handle)
	{
		other.handle = nullptr;
	}

	Udev& Udev::operator=(const Udev &other)
	{
		this->handle = udev_ref(other.handle);
		return *this;
	}

	Udev& Udev::operator=(Udev &&other)
	{
		this->handle = other.handle;
		other.handle = nullptr;
		return *this;
	}

	Udev::~Udev()
	{
		if (handle)
		{
			udev_unref(handle);
		}
	}

	UdevMonitor Udev::monitor_new_from_netlink(const char *name)
	{
		assert(name != nullptr && (std::string(name) == "udev" || std::string(name) == "kernel"));
		return UdevMonitor(udev_monitor_new_from_netlink(handle, name));
	}

	UdevDevice Udev::device_from_syspath(std::string syspath) const
	{
		UdevDeviceHandle *devicep = udev_device_new_from_syspath(handle, syspath.c_str());
		if (devicep == NULL) {
			throw std::runtime_error((std::string("Error while creating UdevDevice from syspath, error is: ") + std::strerror(errno)).c_str());
		}
		return UdevDevice(devicep);
	}

	UdevEnumerate Udev::enumerate_new()
	{
		return UdevEnumerate(udev_enumerate_new(handle));
	}
}
