#ifndef CAMERA_STREAM_INFO_HPP
#define CAMERA_STREAM_INFO_HPP
#include "shared_context.hpp"

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
class CameraStreamInfo : public RefCounted {
	GDCLASS(CameraStreamInfo, RefCounted); //NOLINT

public:
	explicit CameraStreamInfo(SharedContext *shared_context, SharedContext::CameraDataStream stream) :
			_shared_context(shared_context), _stream(stream){};
	~CameraStreamInfo() override = default;

	[[nodiscard]] auto get_fd() const -> int;
	[[nodiscard]] auto get_length() const -> size_t;
	[[nodiscard]] auto get_width() const -> unsigned int;
	[[nodiscard]] auto get_height() const -> unsigned int;
	[[nodiscard]] auto get_stride() const -> unsigned int;
	[[nodiscard]] auto get_pixel_format() const -> libcamera::PixelFormat;
	[[nodiscard]] auto get_color_space() const -> std::optional<libcamera::ColorSpace>;
	[[nodiscard]] auto get_pixel_format_string() const -> String;
	[[nodiscard]] auto get_color_space_string() const -> String;

protected:
	static void _bind_methods();

private:
	CameraStreamInfo() :
			_shared_context(nullptr), _stream(SharedContext::RAW){};
	const SharedContext *_shared_context;
	const SharedContext::CameraDataStream _stream;

	template <typename T, typename U>
	auto _get_shared_memory_stream_value(U const CinePiInfoStruct::*member_raw, U const CinePiInfoStruct::*member_isp, U const CinePiInfoStruct::*member_lores, T const default_value) const -> T { //NOLINT(bugprone-easily-swappable-parameters)
		if (!_shared_context) {
			UtilityFunctions::printerr("CAMERA_STREAM_INFO: Shared context not attached.");
			return default_value;
		}
		if (!_shared_context->is_shared_memory_bound()) {
			UtilityFunctions::printerr("CAMERA_STREAM_INFO: Shared memory not attached.");
			return default_value;
		}
		if (!_shared_context->_aquire_sem()) {
			return default_value;
		}
		T val;
		switch (_stream) {
			case SharedContext::RAW:
				val = _shared_context->_shared_memory->info.*member_raw;
				break;
			case SharedContext::ISP:
				val = _shared_context->_shared_memory->info.*member_isp;
				break;
			case SharedContext::LORES:
				val = _shared_context->_shared_memory->info.*member_lores;
				break;
			default:
				UtilityFunctions::printerr("CAMERA_STREAM_NFO: Invalid selection of CameraDataStream.");
				val = default_value;
		}
		if (!_shared_context->_release_sem()) {
			return default_value;
		}
		return val;
	}

	template <typename T, typename U>
	auto _get_streaminfo_value(U const StreamInfo::*streaminfo_member, T const default_value) const -> T {
		if (!_shared_context) {
			UtilityFunctions::printerr("CAMERA_STREAM_INFO: Shared context not attached.");
			return default_value;
		}
		if (!_shared_context->is_shared_memory_bound()) {
			UtilityFunctions::printerr("CAMERA_STREAM_INFO: Shared memory not attached.");
			return default_value;
		}
		if (!_shared_context->_aquire_sem()) {
			return default_value;
		}
		T val;
		switch (_stream) {
			case SharedContext::RAW:
				val = _shared_context->_shared_memory->raw.*streaminfo_member;
				break;
			case SharedContext::ISP:
				val = _shared_context->_shared_memory->isp.*streaminfo_member;
				break;
			case SharedContext::LORES:
				val = _shared_context->_shared_memory->lores.*streaminfo_member;
				break;
			default:
				UtilityFunctions::printerr("CAMERA_STREAM_INFO: Invalid selection of CameraDataStream.");
				val = default_value;
		}
		if (!_shared_context->_release_sem()) {
			return default_value;
		}
		return val;
	}
};

} //namespace godot
#endif
