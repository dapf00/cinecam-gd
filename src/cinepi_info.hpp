#ifndef CINEPI_INFO_HPP
#define CINEPI_INFO_HPP

#include "shared_context.hpp"

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
class CinePiInfo : public RefCounted {
	GDCLASS(CinePiInfo, RefCounted); //NOLINT
public:
	explicit CinePiInfo(SharedContext *shared_context) :
			_shared_context(shared_context){};
	~CinePiInfo() override = default;

	[[nodiscard]] auto get_is_recording() const -> bool;
	[[nodiscard]] auto get_ts() const -> uint64_t;
	[[nodiscard]] auto get_fd_raw() const -> int;
	[[nodiscard]] auto get_fd_isp() const -> int;
	[[nodiscard]] auto get_fd_lores() const -> int;
	[[nodiscard]] auto get_raw_length() const -> size_t;
	[[nodiscard]] auto get_isp_length() const -> size_t;
	[[nodiscard]] auto get_lores_length() const -> size_t;
	[[nodiscard]] auto get_procid() const -> int;
	[[nodiscard]] auto get_frame() const -> uint64_t;
	[[nodiscard]] auto get_sequence() const -> unsigned int;
	[[nodiscard]] auto get_framerate() const -> float;
	[[nodiscard]] auto get_width() const -> int;
	[[nodiscard]] auto get_height() const -> int;
	[[nodiscard]] auto get_compression() const -> int;
	[[nodiscard]] auto get_thumbnail() const -> int;
	[[nodiscard]] auto get_thumbnail_size() const -> int;
	[[nodiscard]] auto get_raw_crop() const -> Vector4i;

protected:
	static void _bind_methods();

private:
	CinePiInfo() :
			_shared_context(nullptr){};
	const SharedContext *_shared_context;

	template <typename T, typename U>
	auto _get_info_value(U const CinePiInfoStruct::*info_member, T const default_value) const -> T {
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
		auto val = _shared_context->_shared_memory->info.*info_member;
		if (!_shared_context->_release_sem()) {
			return default_value;
		}
		return val;
	}
};
} //namespace godot
#endif
