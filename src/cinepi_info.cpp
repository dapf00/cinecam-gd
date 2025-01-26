#include "cinepi_info.hpp"

namespace godot {

auto CinePiInfo::get_is_recording() const -> bool { return _get_info_value(&CinePiInfoStruct::is_recording, false); }
auto CinePiInfo::get_ts() const -> uint64_t { return _get_info_value(&CinePiInfoStruct::ts, 0); }
auto CinePiInfo::get_procid() const -> int { return _get_info_value(&CinePiInfoStruct::procid, 0); }
auto CinePiInfo::get_frame() const -> uint64_t { return _get_info_value(&CinePiInfoStruct::frame, 0); }
auto CinePiInfo::get_sequence() const -> unsigned int { return _get_info_value(&CinePiInfoStruct::sequence, 0); }
auto CinePiInfo::get_framerate() const -> float { return _get_info_value(&CinePiInfoStruct::framerate, 0); }
auto CinePiInfo::get_width() const -> int { return _get_info_value(&CinePiInfoStruct::width, 0); }
auto CinePiInfo::get_height() const -> int { return _get_info_value(&CinePiInfoStruct::height, 0); }
auto CinePiInfo::get_compression() const -> int { return _get_info_value(&CinePiInfoStruct::compression, 0); }
auto CinePiInfo::get_thumbnail() const -> int { return _get_info_value(&CinePiInfoStruct::thumbnail, 0); }
auto CinePiInfo::get_thumbnail_size() const -> int { return _get_info_value(&CinePiInfoStruct::thumbnail_size, 0); }
auto CinePiInfo::get_raw_crop() const -> Vector4i {
	auto val = _get_info_value<std::array<int, 4>>(&CinePiInfoStruct::raw_crop, { 0, 0, 0, 0 });
	return Vector4i(val.at(0), val.at(1), val.at(2), val.at(3));
}

void CinePiInfo::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_recording"), &CinePiInfo::get_is_recording);
	ClassDB::bind_method(D_METHOD("get_ts"), &CinePiInfo::get_ts);
	ClassDB::bind_method(D_METHOD("get_procid"), &CinePiInfo::get_procid);
	ClassDB::bind_method(D_METHOD("get_frame"), &CinePiInfo::get_frame);
	ClassDB::bind_method(D_METHOD("get_sequence"), &CinePiInfo::get_sequence);
	ClassDB::bind_method(D_METHOD("get_framerate"), &CinePiInfo::get_framerate);
	ClassDB::bind_method(D_METHOD("get_width"), &CinePiInfo::get_width);
	ClassDB::bind_method(D_METHOD("get_height"), &CinePiInfo::get_height);
	ClassDB::bind_method(D_METHOD("get_compression"), &CinePiInfo::get_compression);
	ClassDB::bind_method(D_METHOD("get_thumbnail"), &CinePiInfo::get_thumbnail);
	ClassDB::bind_method(D_METHOD("get_thumbnail_size"), &CinePiInfo::get_thumbnail_size);
	ClassDB::bind_method(D_METHOD("get_raw_crop"), &CinePiInfo::get_raw_crop);
}
} //namespace godot
