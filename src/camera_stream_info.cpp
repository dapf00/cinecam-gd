#include "camera_stream_info.hpp"

namespace godot {
auto CameraStreamInfo::get_fd() const -> int { return _get_shared_memory_stream_value(&CinePiInfoStruct::fd_raw, &CinePiInfoStruct::fd_isp, &CinePiInfoStruct::fd_lores, 0); }
auto CameraStreamInfo::get_length() const -> size_t { return _get_shared_memory_stream_value(&CinePiInfoStruct::raw_length, &CinePiInfoStruct::isp_length, &CinePiInfoStruct::lores_length, 0); }
auto CameraStreamInfo::get_width() const -> unsigned int { return _get_streaminfo_value(&StreamInfo::width, 0); }
auto CameraStreamInfo::get_height() const -> unsigned int { return _get_streaminfo_value(&StreamInfo::height, 0); }
auto CameraStreamInfo::get_stride() const -> unsigned int { return _get_streaminfo_value(&StreamInfo::stride, 0); }
auto CameraStreamInfo::get_pixel_format() const -> libcamera::PixelFormat { return _get_streaminfo_value(&StreamInfo::pixel_format, libcamera::PixelFormat()); }
auto CameraStreamInfo::get_color_space() const -> std::optional<libcamera::ColorSpace> { return _get_streaminfo_value<std::optional<libcamera::ColorSpace>>(&StreamInfo::colour_space, std::nullopt); }
auto CameraStreamInfo::get_pixel_format_string() const -> String { return get_pixel_format().toString().c_str(); }
auto CameraStreamInfo::get_color_space_string() const -> String {
	auto color_space = get_color_space();
	if (color_space.has_value()) {
		return color_space.value().toString().c_str();
	} else {
		return "";
	}
}

void CameraStreamInfo::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_fd"), &CameraStreamInfo::get_fd);
	ClassDB::bind_method(D_METHOD("get_length"), &CameraStreamInfo::get_width);
	ClassDB::bind_method(D_METHOD("get_width"), &CameraStreamInfo::get_width);
	ClassDB::bind_method(D_METHOD("get_height"), &CameraStreamInfo::get_height);
	ClassDB::bind_method(D_METHOD("get_stride"), &CameraStreamInfo::get_stride);
	ClassDB::bind_method(D_METHOD("get_pixel_format"), &CameraStreamInfo::get_pixel_format_string);
	ClassDB::bind_method(D_METHOD("get_color_space"), &CameraStreamInfo::get_color_space_string);
	// 	ADD_PROPERTY(PropertyInfo(Variant::INT, "fd", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_fd");
	// 	ADD_PROPERTY(PropertyInfo(Variant::INT, "length", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_lengt");
	// 	ADD_PROPERTY(PropertyInfo(Variant::INT, "width", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_width");
	// 	ADD_PROPERTY(PropertyInfo(Variant::INT, "height", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_height");
	// 	ADD_PROPERTY(PropertyInfo(Variant::INT, "stride", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_stride");
	// 	ADD_PROPERTY(PropertyInfo(Variant::STRING, "pixel_format", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_pixel_format");
	// 	ADD_PROPERTY(PropertyInfo(Variant::STRING, "color_space", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_color_space");
}
} //namespace godot
