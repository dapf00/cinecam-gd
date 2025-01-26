#include "cinepi_metadata.hpp"

namespace godot {
auto CinePiMetadata::get_ae_constraint_mode() const -> int { return _get_metadata_value(&CinePiMetadataStruct::ae_constraint_mode, 0); }
auto CinePiMetadata::get_ae_enable() const -> bool { return _get_metadata_value(&CinePiMetadataStruct::ae_enable, false); }
auto CinePiMetadata::get_ae_exposure_mode() const -> int { return _get_metadata_value(&CinePiMetadataStruct::ae_exposure_mode, 0); }
auto CinePiMetadata::get_ae_flicker_detected() const -> int { return _get_metadata_value(&CinePiMetadataStruct::ae_flicker_detected, 0); }
auto CinePiMetadata::get_ae_flicker_mode() const -> int { return _get_metadata_value(&CinePiMetadataStruct::ae_flicker_mode, 0); }
auto CinePiMetadata::get_ae_flicker_period() const -> int { return _get_metadata_value(&CinePiMetadataStruct::ae_flicker_period, 0); }
auto CinePiMetadata::get_ae_locked() const -> bool { return _get_metadata_value(&CinePiMetadataStruct::ae_locked, false); }
auto CinePiMetadata::get_ae_metering_mode() const -> int { return _get_metadata_value(&CinePiMetadataStruct::ae_metering_mode, 0); }
auto CinePiMetadata::get_analogue_gain() const -> float { return _get_metadata_value(&CinePiMetadataStruct::analogue_gain, 0.); }
auto CinePiMetadata::get_awb_enable() const -> bool { return _get_metadata_value(&CinePiMetadataStruct::awb_enable, false); }
auto CinePiMetadata::get_awb_locked() const -> bool { return _get_metadata_value(&CinePiMetadataStruct::awb_locked, false); }
auto CinePiMetadata::get_awb_mode() const -> int { return _get_metadata_value(&CinePiMetadataStruct::awb_mode, 0); }
auto CinePiMetadata::get_brightness() const -> float { return _get_metadata_value(&CinePiMetadataStruct::brightness, 0.); }
auto CinePiMetadata::get_color_temperature() const -> int { return _get_metadata_value(&CinePiMetadataStruct::color_temperature, 0); }
auto CinePiMetadata::get_contrast() const -> float { return _get_metadata_value(&CinePiMetadataStruct::contrast, 0.); }
auto CinePiMetadata::get_digital_gain() const -> float { return _get_metadata_value(&CinePiMetadataStruct::digital_gain, 0.); }
auto CinePiMetadata::get_exposure_time() const -> int { return _get_metadata_value(&CinePiMetadataStruct::exposure_time, 0); }
auto CinePiMetadata::get_exposure_value() const -> float { return _get_metadata_value(&CinePiMetadataStruct::exposure_value, 0.); }
auto CinePiMetadata::get_frame_duration() const -> int { return _get_metadata_value(&CinePiMetadataStruct::frame_duration, 0); }
auto CinePiMetadata::get_gamma() const -> float { return _get_metadata_value(&CinePiMetadataStruct::gamma, 0.); }
auto CinePiMetadata::get_lens_position() const -> float { return _get_metadata_value(&CinePiMetadataStruct::lens_position, 0.); }
auto CinePiMetadata::get_lux() const -> int { return _get_metadata_value(&CinePiMetadataStruct::lux, 0); }
auto CinePiMetadata::get_saturation() const -> float { return _get_metadata_value(&CinePiMetadataStruct::saturation, 0.); }
auto CinePiMetadata::get_sharpness() const -> float { return _get_metadata_value(&CinePiMetadataStruct::sharpness, 0.); }

auto CinePiMetadata::get_color_gains() const -> Vector2 {
	auto val = _get_metadata_value<std::array<float, 2>>(&CinePiMetadataStruct::color_gains, { 0., 0. });
	return Vector2(val.at(0), val.at(1));
}
auto CinePiMetadata::get_frame_duration_limits() const -> Vector2i {
	auto val = _get_metadata_value<std::array<int, 2>>(&CinePiMetadataStruct::frame_duration_limits, { 0, 0 });
	return Vector2i(val.at(0), val.at(1));
}

void CinePiMetadata::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_ae_constraint_mode"), &CinePiMetadata::get_ae_constraint_mode);
	ClassDB::bind_method(D_METHOD("get_ae_enable"), &CinePiMetadata::get_ae_enable);
	ClassDB::bind_method(D_METHOD("get_ae_exposure_mode"), &CinePiMetadata::get_ae_exposure_mode);
	ClassDB::bind_method(D_METHOD("get_ae_flicker_detected"), &CinePiMetadata::get_ae_flicker_detected);
	ClassDB::bind_method(D_METHOD("get_ae_flicker_mode"), &CinePiMetadata::get_ae_flicker_mode);
	ClassDB::bind_method(D_METHOD("get_ae_flicker_period"), &CinePiMetadata::get_ae_flicker_period);
	ClassDB::bind_method(D_METHOD("get_ae_locked"), &CinePiMetadata::get_ae_locked);
	ClassDB::bind_method(D_METHOD("get_ae_metering_mode"), &CinePiMetadata::get_ae_metering_mode);
	ClassDB::bind_method(D_METHOD("get_analogue_gain"), &CinePiMetadata::get_analogue_gain);
	ClassDB::bind_method(D_METHOD("get_awb_enable"), &CinePiMetadata::get_awb_enable);
	ClassDB::bind_method(D_METHOD("get_awb_locked"), &CinePiMetadata::get_awb_locked);
	ClassDB::bind_method(D_METHOD("get_awb_mode"), &CinePiMetadata::get_awb_mode);
	ClassDB::bind_method(D_METHOD("get_brightness"), &CinePiMetadata::get_brightness);
	ClassDB::bind_method(D_METHOD("get_color_gains"), &CinePiMetadata::get_color_gains);
	ClassDB::bind_method(D_METHOD("get_color_temperature"), &CinePiMetadata::get_color_temperature);
	ClassDB::bind_method(D_METHOD("get_contrast"), &CinePiMetadata::get_contrast);
	ClassDB::bind_method(D_METHOD("get_digital_gain"), &CinePiMetadata::get_digital_gain);
	ClassDB::bind_method(D_METHOD("get_exposure_time"), &CinePiMetadata::get_exposure_time);
	ClassDB::bind_method(D_METHOD("get_exposure_value"), &CinePiMetadata::get_exposure_value);
	ClassDB::bind_method(D_METHOD("get_frame_duration"), &CinePiMetadata::get_frame_duration);
	ClassDB::bind_method(D_METHOD("get_frame_duration_limits"), &CinePiMetadata::get_frame_duration_limits);
	ClassDB::bind_method(D_METHOD("get_gamma"), &CinePiMetadata::get_gamma);
	ClassDB::bind_method(D_METHOD("get_lens_position"), &CinePiMetadata::get_lens_position);
	ClassDB::bind_method(D_METHOD("get_lux"), &CinePiMetadata::get_lux);
	ClassDB::bind_method(D_METHOD("get_saturation"), &CinePiMetadata::get_saturation);
	ClassDB::bind_method(D_METHOD("get_sharpness"), &CinePiMetadata::get_sharpness);
}
} //namespace godot
