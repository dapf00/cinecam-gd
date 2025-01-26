#ifndef CINEPI_COMMANDS_HPP
#define CINEPI_COMMANDS_HPP
#include "shared_context.hpp"

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
class CinePiCommands : public Resource {
	GDCLASS(CinePiCommands, RefCounted); //NOLINT

public:
	explicit CinePiCommands(SharedContext *shared_context) :
			_shared_context(shared_context){};
	~CinePiCommands() override = default;

	void set_ae_constraint_mode(int ae_constraint_mode);
	void set_ae_enable(bool ae_enable);
	void set_ae_exposure_mode(int ae_exposure_mode);
	void set_ae_flicker_mode(int ae_flicker_mode);
	void set_ae_flicker_period(int ae_flicker_period);
	void set_ae_metering_mode(int ae_metering_mode);
	void set_analogue_gain(float analogue_gain);
	void set_awb_enable(bool awb_enable);
	void set_awb_mode(int awb_mode);
	void set_brightness(float brightness);
	void set_color_gains(Vector2 color_gains);
	void set_color_temperature(int color_temperature);
	void set_contrast(float contrast);
	void set_exposure_time(int exposure_time);
	void set_exposure_value(float exposure_value);
	void set_frame_duration_limits(Vector2i frame_duration_limits);
	void set_gamma(float gamma);
	void set_saturation(float saturation);
	void set_sharpness(float sharpness);
	void set_recording(bool set_recording);
	void set_reinitialize(bool reinitialize);
	void set_width(int width);
	void set_height(int height);
	void set_compression(int compression);
	void set_thumbnail(int thumbnail);
	void set_thumbnail_size(int thumbnail_size);
	void set_raw_crop(Vector4i raw_crop);

protected:
	static void _bind_methods();

private:
	CinePiCommands() :
			_shared_context(nullptr){};
	const SharedContext *_shared_context;

	template <typename U, typename T>
	void _set_command_value(U CinePiCommandStruct::*command_member, T value) {
		if (!_shared_context) {
			UtilityFunctions::printerr("CAMERA_STREAM_INFO: Shared context not attached.");
			return;
		}
		if (!_shared_context->is_shared_memory_bound()) {
			UtilityFunctions::printerr("CAMERA_STREAM_INFO: Shared memory not attached.");
			return;
		}
		if (!_shared_context->_aquire_sem()) {
			return;
		}
		_shared_context->_shared_memory->commands.*command_member = std::optional(value);
		if (!_shared_context->_release_sem()) {
			return;
		}
	}
};
} //namespace godot

#endif
