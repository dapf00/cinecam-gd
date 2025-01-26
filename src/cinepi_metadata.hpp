#ifndef CINEPI_METADATA_HPP
#define CINEPI_METADATA_HPP

#include "shared_context.hpp"

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
class CinePiMetadata : public RefCounted {
	GDCLASS(CinePiMetadata, RefCounted); //NOLINT
public:
	CinePiMetadata(SharedContext *shared_context) :
			_shared_context(shared_context){};
	~CinePiMetadata() override = default;

	[[nodiscard]] auto get_ae_constraint_mode() const -> int;
	[[nodiscard]] auto get_ae_enable() const -> bool;
	[[nodiscard]] auto get_ae_exposure_mode() const -> int;
	[[nodiscard]] auto get_ae_flicker_detected() const -> int;
	[[nodiscard]] auto get_ae_flicker_mode() const -> int;
	[[nodiscard]] auto get_ae_flicker_period() const -> int;
	[[nodiscard]] auto get_ae_locked() const -> bool;
	[[nodiscard]] auto get_ae_metering_mode() const -> int;
	[[nodiscard]] auto get_analogue_gain() const -> float;
	[[nodiscard]] auto get_awb_enable() const -> bool;
	[[nodiscard]] auto get_awb_locked() const -> bool;
	[[nodiscard]] auto get_awb_mode() const -> int;
	[[nodiscard]] auto get_brightness() const -> float;
	[[nodiscard]] auto get_color_gains() const -> Vector2;
	[[nodiscard]] auto get_color_temperature() const -> int;
	[[nodiscard]] auto get_contrast() const -> float;
	[[nodiscard]] auto get_digital_gain() const -> float;
	[[nodiscard]] auto get_exposure_time() const -> int;
	[[nodiscard]] auto get_exposure_value() const -> float;
	[[nodiscard]] auto get_frame_duration() const -> int;
	[[nodiscard]] auto get_frame_duration_limits() const -> Vector2i;
	[[nodiscard]] auto get_gamma() const -> float;
	[[nodiscard]] auto get_lens_position() const -> float;
	[[nodiscard]] auto get_lux() const -> int;
	[[nodiscard]] auto get_saturation() const -> float;
	[[nodiscard]] auto get_sharpness() const -> float;

protected:
	static void _bind_methods();

private:
	CinePiMetadata() :
			_shared_context(nullptr){};
	const SharedContext *_shared_context;

	template <typename T, typename U>
	auto _get_metadata_value(U const CinePiMetadataStruct::*metadata_member, T const default_value) const -> T {
		if (!_shared_context) {
			UtilityFunctions::printerr("CINEPI_METADATA: Shared context not attached.");
			return default_value;
		}
		if (!_shared_context->is_shared_memory_bound()) {
			UtilityFunctions::printerr("CINEPI_METADATA: Shared memory not attached.");
			return default_value;
		}
		if (!_shared_context->_aquire_sem()) {
			return default_value;
		}
		auto val = _shared_context->_shared_memory->metadata.*metadata_member;
		if (!_shared_context->_release_sem()) {
			return default_value;
		}
		return val;
	}
};
} //namespace godot
#endif
