// #ifndef CINEPI_HPP
// #define CINEPI_HPP

// #include "shared_context.hpp"

// #include <godot_cpp/classes/node.hpp>
// #include <memory>

// #include <libcamera/color_space.h>
// #include <libcamera/pixel_format.h>
// namespace godot {

// class CinePi : public Node {
// 	GDCLASS(CinePi, Node); //NOLINT

// public:
// 	CinePi();
// 	~CinePi() override;

// 	CinePi(const CinePi &) = delete;

// 	CinePi(CinePi &&) = delete;
// 	void operator=(CinePi &&) = delete;

// 	[[nodiscard]] auto get_shared_context() const -> Ref<SharedContext>;

// protected:
// 	static void _bind_methods();

// private:
// 	Ref<SharedContext> _shared_context;

// 	void _connect_shared_context();

// 	// 	template <typename T, typename U>
// 	// 	auto _get_shared_context_value(U (SharedContext::*member_function)() const, T const default_value) const -> T {
// 	// 		if (!_shared_context.is_valid()) {
// 	// 			UtilityFunctions::printerr("CINEPI: Shared Context not connected.");
// 	// 			return default_value;
// 	// 		}
// 	// 		return (_shared_context.get()->*member_function)();
// 	// 	}

// 	// 	template <typename T, typename U>
// 	// 	auto _get_shared_context_stream_value(U (SharedContext::*member_function)(SharedContext::CameraDataStream) const, SharedContext::CameraDataStream const cds, T const default_value) const -> T {
// 	// 		if (!_shared_context.is_valid()) {
// 	// 			UtilityFunctions::printerr("CINEPI: Shared Context not connected.");
// 	// 			return default_value;
// 	// 		}
// 	// 		return (_shared_context.get()->*member_function)(cds);
// 	// 	}
// };

// } //namespace godot

// #endif
