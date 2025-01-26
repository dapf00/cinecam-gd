#ifndef SHARED_CONTEXT_HPP
#define SHARED_CONTEXT_HPP

#include "shared_context_structs.hpp"

#include <semaphore.h>
#include <csignal>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <libcamera/color_space.h>
#include <libcamera/pixel_format.h>

constexpr auto PROJECT_ID = "/CINE";

namespace godot {

// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
// cppcheck-suppress-begin [unusedStructMember]
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
// cppcheck-suppress-end [unusedStructMember]

class CameraStreamInfo;
class CinePiCommands;
class CinePiInfo;
class CinePiMetadata;

class SharedContext : public Resource {
	GDCLASS(SharedContext, Resource); //NOLINT
	friend class CameraStreamInfo;
	friend class CinePiCommands;
	friend class CinePiInfo;
	friend class CinePiMetadata;

public:
	enum CameraDataStream {
		RAW,
		ISP,
		LORES
	};

	SharedContext();
	~SharedContext() override;

	[[nodiscard]] auto is_shared_memory_bound() const -> bool;

	[[nodiscard]] auto get_raw_stream_info() const -> Ref<CameraStreamInfo>;
	[[nodiscard]] auto get_isp_stream_info() const -> Ref<CameraStreamInfo>;
	[[nodiscard]] auto get_lores_stream_info() const -> Ref<CameraStreamInfo>;
	[[nodiscard]] auto get_stream_info(CameraDataStream cds) const -> Ref<CameraStreamInfo>;
	[[nodiscard]] auto get_cinepi_commands() const -> Ref<CinePiCommands>;
	[[nodiscard]] auto get_cinepi_info() const -> Ref<CinePiInfo>;
	[[nodiscard]] auto get_cinepi_metadata() const -> Ref<CinePiMetadata>;

protected:
	static void _bind_methods();

private:
	SharedMemoryBuffer *_shared_memory = nullptr;
	const Ref<CameraStreamInfo> _raw;
	const Ref<CameraStreamInfo> _isp;
	const Ref<CameraStreamInfo> _lores;
	const Ref<CinePiCommands> _cinepi_commands;
	const Ref<CinePiInfo> _cinepi_info;
	const Ref<CinePiMetadata> _cinepi_metadata;

	void _attach_shared_memory();
	void _detach_shared_memory();

	[[nodiscard]] auto _aquire_sem() const -> bool;
	[[nodiscard]] auto _release_sem() const -> bool;
};
} //namespace godot

#endif
