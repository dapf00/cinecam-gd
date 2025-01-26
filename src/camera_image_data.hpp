#ifndef CAMERA_IMAGE_DATA_HPP
#define CAMERA_IMAGE_DATA_HPP
#include <csignal>
#include <cstdint>
#include <span>

namespace godot {
class CameraImageData {
public:
	CameraImageData(int procid, size_t length, int fd);
	~CameraImageData();

	CameraImageData(const CameraImageData &) = delete;
	void operator=(const CameraImageData &) = delete;

	CameraImageData(CameraImageData &&) = delete;
	void operator=(CameraImageData &&) = delete;

	[[nodiscard]] auto get_data() const -> uint8_t *;
	[[nodiscard]] auto is_valid() const -> bool;

private:
	uint8_t *_src_image_data = nullptr;
	size_t _length = 0;
	bool _valid = false;

	auto _get_shared_proc_fd(int pid, int fd) -> int;
};
} //namespace godot
#endif
