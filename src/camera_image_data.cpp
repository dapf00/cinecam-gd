#include "camera_image_data.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

#include <sys/mman.h>
#include <sys/syscall.h>
#include <cerrno>
namespace godot {
CameraImageData::CameraImageData(int procid, size_t length, int fd) //NOLINT(bugprone-easily-swappable-parameters
		:
		_length(length) {
	if (_length == 0) {
		UtilityFunctions::printerr("CAMERA_IMAGE_DATA: Memory Segment is of size 0.");
		return;
	}
	int proc_fd = _get_shared_proc_fd(procid, fd);
	//values are set according to users selected video stream
	if (proc_fd < 0) {
		UtilityFunctions::printerr("CAMERA_IMAGE_DATA: Invalid File Desciptor.");
		return;
	}

	_src_image_data = static_cast<uint8_t *>(mmap(nullptr, _length, PROT_READ, MAP_SHARED, proc_fd, 0));
	//memory is mapped to image data with file descripter obtained from shared context

	if (_src_image_data == MAP_FAILED) {
		UtilityFunctions::printerr("CAMERA_IMAGE_DATA: mmap failed. Error = ", errno);
		return;
	}

	if (close(proc_fd) == -1) {
		UtilityFunctions::printerr("CAMERA_IMAGE_DATA: mmap failed. Error = ", errno);
		return;
	}

	_valid = true;
}

CameraImageData::~CameraImageData() {
	if (_src_image_data == MAP_FAILED || _src_image_data == nullptr) {
		return;
	}
	munmap(_src_image_data, _length);
}

auto CameraImageData::get_data() const -> uint8_t * {
	return _src_image_data;
}

auto CameraImageData::is_valid() const -> bool {
	return _valid;
}

auto CameraImageData::_get_shared_proc_fd(int pid, int fd) -> int { //NOLINT(bugprone-easily-swappable-parameters)
	int pid_fd = syscall(SYS_pidfd_open, pid, 0); //NOLINT(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions,cppcoreguidelines-pro-type-vararg)
	if (pid_fd == -1) {
		UtilityFunctions::printerr("CAMERA_IMAGE_DATA: Failed to open process.");
		return -1;
	}

	int new_fd = syscall(SYS_pidfd_getfd, pid_fd, fd, 0); //NOLINT(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions,cppcoreguidelines-pro-type-vararg)
	if (new_fd == -1) {
		UtilityFunctions::printerr("CAMERA_IMAGE_DATA: Failed to get FD of image.");
		close(pid_fd);
		return -1;
	}

	close(pid_fd);

	return new_fd;
}

} //namespace godot
