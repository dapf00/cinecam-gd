#include "shared_context.hpp"
#include "camera_stream_info.hpp"
#include "cinepi_commands.hpp"
#include "cinepi_info.hpp"
#include "cinepi_metadata.hpp"

#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstdint>

namespace godot {

SharedContext::SharedContext() :
		_raw(memnew(CameraStreamInfo(this, RAW))), _isp(memnew(CameraStreamInfo(this, ISP))), _lores(memnew(CameraStreamInfo(this, LORES))), _cinepi_commands(memnew(CinePiCommands(this))), _cinepi_metadata(memnew(CinePiMetadata(this))), _cinepi_info(memnew(CinePiInfo(this))) { //NOLINT
	UtilityFunctions::print_verbose("SHARED_CONTEXT: Constructor called.");
	_attach_shared_memory();
}

SharedContext::~SharedContext() {
	UtilityFunctions::print_verbose("SHARED_CONTEXT: Destructor called.");
	_detach_shared_memory();
}

void SharedContext::_attach_shared_memory() {
	int fd = shm_open(PROJECT_ID, O_RDWR, 0);
	int size = sizeof(SharedMemoryBuffer);
	if (fd == -1) { // Handle error
		UtilityFunctions::printerr("SHARED_CONTEXT: shm_open failed. Error = ", errno);
	}
	_shared_memory = static_cast<SharedMemoryBuffer *>(mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

	if (_shared_memory == reinterpret_cast<void *>(-1)) { //NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
		UtilityFunctions::printerr("SHARED_CONTEXT: mmap failed. Error = ", errno);
	}

	if (close(fd) == -1) {
		UtilityFunctions::printerr("SHARED_CONTEXT: fclose failed. Error = ", errno);
	}

	if (is_shared_memory_bound()) {
		UtilityFunctions::print_verbose("SHARED_CONTEXT: Shared Memory attached.");
	}
}

void SharedContext::_detach_shared_memory() {
	if (_shared_memory != reinterpret_cast<void *>(-1) && _shared_memory != nullptr) { //NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
		int result = munmap(_shared_memory, sizeof(SharedMemoryBuffer));
		//shmdt(_shared_memory);
		if (result == -1) {
			UtilityFunctions::printerr("SHARED_CONTEXT: SharedMemory detach failed, Error = %d", errno);
		} else {
			UtilityFunctions::print_verbose("SHARED_CONTEXT: Shared Memory detached.");
		}
	}
}

auto SharedContext::is_shared_memory_bound() const -> bool { return (_shared_memory != nullptr && _shared_memory != reinterpret_cast<void *>(-1)); }

auto SharedContext::_aquire_sem() const -> bool {
	if (sem_wait(&_shared_memory->sem) == -1) {
		UtilityFunctions::printerr("SHARED_CONTEXT: sem_wait failed. Error = ", errno);
		return false;
	}
	return true;
};

auto SharedContext::_release_sem() const -> bool {
	if (sem_post(&_shared_memory->sem) == -1) {
		UtilityFunctions::printerr("SHARED_CONTEXT: sem_post failed. Error = ", errno);
		return false;
	}
	return true;
};

auto SharedContext::get_stream_info(CameraDataStream cds) const -> Ref<CameraStreamInfo> {
	switch (cds) {
		case RAW:
			return _raw;
		case ISP:
			return _isp;
		case LORES:
			return _lores;
		default:
			UtilityFunctions::printerr("SHARED_CONTEXT: Invalid Camera Data stream.", cds);
			return _isp;
	}
}

auto SharedContext::get_raw_stream_info() const -> Ref<CameraStreamInfo> { return _raw; }
auto SharedContext::get_isp_stream_info() const -> Ref<CameraStreamInfo> { return _isp; }
auto SharedContext::get_lores_stream_info() const -> Ref<CameraStreamInfo> { return _lores; }
auto SharedContext::get_cinepi_commands() const -> Ref<CinePiCommands> { return _cinepi_commands; }
auto SharedContext::get_cinepi_info() const -> Ref<CinePiInfo> { return _cinepi_info; }
auto SharedContext::get_cinepi_metadata() const -> Ref<CinePiMetadata> { return _cinepi_metadata; }

void SharedContext::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_raw_stream_info"), &SharedContext::get_raw_stream_info);
	ClassDB::bind_method(D_METHOD("get_isp_stream_info"), &SharedContext::get_isp_stream_info);
	ClassDB::bind_method(D_METHOD("get_lores_stream_info"), &SharedContext::get_lores_stream_info);
	ClassDB::bind_method(D_METHOD("get_cinepi_commands"), &SharedContext::get_cinepi_commands);
	ClassDB::bind_method(D_METHOD("get_cinepi_info"), &SharedContext::get_cinepi_info);
	ClassDB::bind_method(D_METHOD("get_cinepi_metadata"), &SharedContext::get_cinepi_metadata);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "cinepi_commands", PROPERTY_HINT_NONE, "CinePiCommands", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "",
			"get_cinepi_commands");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "cinepi_info", PROPERTY_HINT_NONE, "CinePiInfo", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "",
			"get_cinepi_info");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "cinepi_metadata", PROPERTY_HINT_NONE, "CinePiMetadata", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "",
			"get_cinepi_metadata");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "streams/raw", PROPERTY_HINT_NONE, "CameraStreamInfo", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY, "streams"), "", "get_raw_stream_info");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "streams/isp", PROPERTY_HINT_NONE, "CameraStreamInfo", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY, "streams"), "", "get_isp_stream_info");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "streams/lores", PROPERTY_HINT_NONE, "CameraStreamInfo", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY, "streams"), "", "get_lores_stream_info");
}
} //namespace godot
