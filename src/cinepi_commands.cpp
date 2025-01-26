#include "cinepi_commands.hpp"

namespace godot {
void CinePiCommands::set_ae_constraint_mode(int ae_constraint_mode) { _set_command_value(&CinePiCommandStruct::ae_constraint_mode, ae_constraint_mode); }
void CinePiCommands::set_ae_enable(bool ae_enable) { _set_command_value(&CinePiCommandStruct::ae_enable, ae_enable); }
void CinePiCommands::set_ae_exposure_mode(int ae_exposure_mode) { _set_command_value(&CinePiCommandStruct::ae_exposure_mode, ae_exposure_mode); }
void CinePiCommands::set_ae_flicker_mode(int ae_flicker_mode) { _set_command_value(&CinePiCommandStruct::ae_flicker_mode, ae_flicker_mode); }
void CinePiCommands::set_ae_flicker_period(int ae_flicker_period) { _set_command_value(&CinePiCommandStruct::ae_flicker_period, ae_flicker_period); }
void CinePiCommands::set_ae_metering_mode(int ae_metering_mode) { _set_command_value(&CinePiCommandStruct::ae_metering_mode, ae_metering_mode); }
void CinePiCommands::set_analogue_gain(float analogue_gain) { _set_command_value(&CinePiCommandStruct::analogue_gain, analogue_gain); }
void CinePiCommands::set_awb_enable(bool awb_enable) { _set_command_value(&CinePiCommandStruct::awb_enable, awb_enable); }
void CinePiCommands::set_awb_mode(int awb_mode) { _set_command_value(&CinePiCommandStruct::awb_mode, awb_mode); }
void CinePiCommands::set_brightness(float brightness) { _set_command_value(&CinePiCommandStruct::brightness, brightness); }
void CinePiCommands::set_color_temperature(int color_temperature) { _set_command_value(&CinePiCommandStruct::color_temperature, color_temperature); }
void CinePiCommands::set_contrast(float contrast) { _set_command_value(&CinePiCommandStruct::contrast, contrast); }
void CinePiCommands::set_exposure_time(int exposure_time) { _set_command_value(&CinePiCommandStruct::exposure_time, exposure_time); }
void CinePiCommands::set_exposure_value(float exposure_value) { _set_command_value(&CinePiCommandStruct::exposure_value, exposure_value); }
void CinePiCommands::set_gamma(float gamma) { _set_command_value(&CinePiCommandStruct::gamma, gamma); }
void CinePiCommands::set_saturation(float saturation) { _set_command_value(&CinePiCommandStruct::saturation, saturation); }
void CinePiCommands::set_sharpness(float sharpness) { _set_command_value(&CinePiCommandStruct::sharpness, sharpness); }
void CinePiCommands::set_recording(bool recording) { _set_command_value(&CinePiCommandStruct::set_recording, recording); }
void CinePiCommands::set_reinitialize(bool reinitialize) { _set_command_value(&CinePiCommandStruct::reinitialize, reinitialize); }
void CinePiCommands::set_width(int width) { _set_command_value(&CinePiCommandStruct::width, width); }
void CinePiCommands::set_height(int height) { _set_command_value(&CinePiCommandStruct::height, height); }
void CinePiCommands::set_compression(int compression) { _set_command_value(&CinePiCommandStruct::compression, compression); }
void CinePiCommands::set_thumbnail(int thumbnail) { _set_command_value(&CinePiCommandStruct::thumbnail, thumbnail); }
void CinePiCommands::set_thumbnail_size(int thumbnail_size) { _set_command_value(&CinePiCommandStruct::thumbnail_size, thumbnail_size); }
void CinePiCommands::set_color_gains(Vector2 color_gains) {
	std::array<float, 2> val = { color_gains.x, color_gains.y }; //NOLINT(cppcoreguidelines-pro-type-union-access)
	_set_command_value(&CinePiCommandStruct::color_gains, val);
}
void CinePiCommands::set_frame_duration_limits(Vector2i frame_duration_limits) {
	std::array<int64_t, 2> val = { frame_duration_limits.x, frame_duration_limits.y }; //NOLINT(cppcoreguidelines-pro-type-union-access)
	_set_command_value(&CinePiCommandStruct::frame_duration_limits, val);
}
void CinePiCommands::set_raw_crop(Vector4i raw_crop) {
	std::array<int, 4> val = { raw_crop.x, raw_crop.y, raw_crop.z, raw_crop.w }; //NOLINT(cppcoreguidelines-pro-type-union-access)
	_set_command_value(&CinePiCommandStruct::raw_crop, val);
}
void CinePiCommands::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_ae_constraint_mode", "ae_constraint_mode"), &CinePiCommands::set_ae_constraint_mode);
	ClassDB::bind_method(D_METHOD("set_ae_enable", "ae_enable"), &CinePiCommands::set_ae_enable);
	ClassDB::bind_method(D_METHOD("set_ae_exposure_mode", "ae_exposure_mode"), &CinePiCommands::set_ae_exposure_mode);
	ClassDB::bind_method(D_METHOD("set_ae_flicker_mode", "ae_flicker_mode"), &CinePiCommands::set_ae_flicker_mode);
	ClassDB::bind_method(D_METHOD("set_ae_flicker_period", "ae_flicker_period"), &CinePiCommands::set_ae_flicker_period);
	ClassDB::bind_method(D_METHOD("set_ae_metering_mode", "ae_metering_mode"), &CinePiCommands::set_ae_metering_mode);
	ClassDB::bind_method(D_METHOD("set_analogue_gain", "analogue_gain"), &CinePiCommands::set_analogue_gain);
	ClassDB::bind_method(D_METHOD("set_awb_enable", "awb_enable"), &CinePiCommands::set_awb_enable);
	ClassDB::bind_method(D_METHOD("set_awb_mode", "awb_mode"), &CinePiCommands::set_awb_mode);
	ClassDB::bind_method(D_METHOD("set_brightness", "brightness"), &CinePiCommands::set_brightness);
	ClassDB::bind_method(D_METHOD("set_color_gains", "color_gains"), &CinePiCommands::set_color_gains);
	ClassDB::bind_method(D_METHOD("set_color_temperature", "color_temperature"), &CinePiCommands::set_color_temperature);
	ClassDB::bind_method(D_METHOD("set_contrast", "contrast"), &CinePiCommands::set_contrast);
	ClassDB::bind_method(D_METHOD("set_exposure_time", "exposure_time"), &CinePiCommands::set_exposure_time);
	ClassDB::bind_method(D_METHOD("set_exposure_value", "exposure_value"), &CinePiCommands::set_exposure_value);
	ClassDB::bind_method(D_METHOD("set_frame_duration_limits", "frame_duration_limits"), &CinePiCommands::set_frame_duration_limits);
	ClassDB::bind_method(D_METHOD("set_gamma", "gamma"), &CinePiCommands::set_gamma);
	ClassDB::bind_method(D_METHOD("set_saturation", "saturation"), &CinePiCommands::set_saturation);
	ClassDB::bind_method(D_METHOD("set_sharpness", "sharpness"), &CinePiCommands::set_sharpness);
	ClassDB::bind_method(D_METHOD("set_recording", "recording"), &CinePiCommands::set_recording);
	ClassDB::bind_method(D_METHOD("set_reinitialize", "reinitialize"), &CinePiCommands::set_reinitialize);
	ClassDB::bind_method(D_METHOD("set_width", "width"), &CinePiCommands::set_width);
	ClassDB::bind_method(D_METHOD("set_height", "height"), &CinePiCommands::set_height);
	ClassDB::bind_method(D_METHOD("set_compression", "compression"), &CinePiCommands::set_compression);
	ClassDB::bind_method(D_METHOD("set_thumbnail", "thumbnail"), &CinePiCommands::set_thumbnail);
	ClassDB::bind_method(D_METHOD("set_thumbnail_size", "thumbnail_size"), &CinePiCommands::set_thumbnail_size);
	ClassDB::bind_method(D_METHOD("set_raw_crop", "raw_crop"), &CinePiCommands::set_raw_crop);
}

} //namespace godot
