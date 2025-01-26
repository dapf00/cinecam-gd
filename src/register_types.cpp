#include "register_types.hpp"

#include "camera_stream_info.hpp"
#include "cinepi_commands.hpp"
#include "cinepi_display.hpp"
#include "cinepi_info.hpp"
#include "cinepi_metadata.hpp"
#include "shared_context.hpp"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

namespace godot {

void initialize_cinepi_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(CinePiDisplay);
	GDREGISTER_CLASS(SharedContext);
	GDREGISTER_CLASS(CameraStreamInfo);
	GDREGISTER_CLASS(CinePiCommands);
	GDREGISTER_CLASS(CinePiInfo);
	GDREGISTER_CLASS(CinePiMetadata);
}

void uninitialize_cinepi_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
auto GDE_EXPORT cinepi_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) -> GDExtensionBool { //NOLINT
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_cinepi_module);
	init_obj.register_terminator(uninitialize_cinepi_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}

} //namespace godot
