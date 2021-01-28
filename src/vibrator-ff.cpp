/*
 * Copyright 2020 UBports foundation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Caleb Connolly <caleb@connolly.tech>
 */

#include "vibrator-ff.h"

#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <fcntl.h>

/*
 * This vibrator supports devices using the Kernel Force Feedback API.
 * Mostly Mainline devices.
 */
namespace hfd {

// For this implementation call the static getFirstFFDevice
// And check that it didn't return the empty string.
bool VibratorFF::usable() {
	return VibratorFF::getFirstFFDevice().size() > 0;
}

// This takes a path to a device like '/dev/input/eventX'
// And queries the device to see if it supports FF_RUMBLE
bool inputDeviceSupportsFF(std::string devname) {
	int ret;
	unsigned char features[1 + FF_MAX/8/sizeof(unsigned char)];
	int tempFd = open(devname.c_str(), O_RDWR);
	int request = EVIOCGBIT(EV_FF, sizeof(features)*sizeof(unsigned char));
	bool supported = false;

	ret = ioctl(tempFd, request, &features);

	if (testBit(FF_RUMBLE, features)) {
		supported =  true;
	} else {
	}

	ret = close(tempFd);
	if (ret != 0) {
		std::cerr << "FF: Failed to close " << tempFd << ": " << ret << std::endl;
	}
	return supported;
}

// Create play and/or stop input events to control
// the rumble effect we uploaded in the constructor.
void VibratorFF::configure(State state, int durationMs) {
	int ret;
	struct input_event play;
	struct input_event stop;

	if (state == State::On) {
		std::cout << "rumbling with magnitude: " << effect.u.rumble.strong_magnitude << " for " << durationMs << "ms" << std::endl;

		play.type = EV_FF;
		play.code = effect.id;
		play.value = 1;
		write(fd, (const void*) &play, sizeof(play));

		usleep(durationMs * 1000);
	}
	stop.type = EV_FF;
	stop.code = effect.id;
	stop.value = 0;
	write(fd, (const void*) &stop, sizeof(stop));
}

// This finds the first device that supports force feedback
// and assumes that it supports rumble, which it may not.
// We should also query the device feature flags and be SURE
std::string VibratorFF::getFirstFFDevice() {
	Udev::Udev udev;
	Udev::UdevEnumerate enumerate = udev.enumerate_new();

	enumerate.add_match_subsystem("input");
	enumerate.scan_devices();
	std::vector<Udev::UdevDevice> devices = enumerate.enumerate_devices();
	std::cout << "FF: Found " << devices.size() << " input devices" << std::endl;
	for(int i = 0; i < devices.size(); i++) {
		const auto properties = devices.at(i).get_properties();
		if (properties.find("DEVNAME") != properties.end()) {
			std::string temp = devices.at(i).get_properties().at("DEVNAME");
			if (inputDeviceSupportsFF(temp)) {
				std::cout << "Using " << temp << std::endl;
				return temp;
			}
		}
	}
	return "";
}

// Create a force feedback vibrator
VibratorFF::VibratorFF(): Vibrator() {
	int ret;
	// Find the first available input device that supports
	// force feedback, we expect this to succeed as the caller
	// should have called the static 'usable' method first.
	devname = VibratorFF::getFirstFFDevice();

	std::memset(&effect, 0, sizeof(effect));

	// Create a rumble effect
	effect.type = FF_RUMBLE;
	effect.id = -1;
	effect.u.rumble.strong_magnitude = 0x6000; // This should be adjustable
	effect.u.rumble.weak_magnitude = 0;

	fd = open(devname.c_str(), O_RDWR);
	if (fd < 0) {
		std::cerr << "Can't open force feedback device path: " << devname << std::endl;
		return;
	}

	// Upload the effect to the device, this doesn't cause
	// it to vibrate, just to store the effect in kernel memory.
	ret = ioctl(fd, EVIOCSFF, &effect);
	if (ret < 0) {
		std::cerr << "Failed to upload rumble effect" << std::endl;
		return;
	}
}

VibratorFF::~VibratorFF() {
	int ret;
	if (fd > 0) {
		// Unload the effect
		ret = ioctl(fd, EVIOCRMFF, effect.id);
		if (ret < 0)
				std::cerr << "Failed to unload effect " << effect.id << std::endl;
		close(fd);
	}
}
}
