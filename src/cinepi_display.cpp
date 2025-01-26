#include "cinepi_display.hpp"
#include "camera_image_data.hpp"
#include "camera_stream_info.hpp"
#include "cinepi_info.hpp"
#include "decoder_functions.hpp"

#include <memory>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/placeholder_texture2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <libcamera/formats.h>

namespace godot {

CinePiDisplay::CinePiDisplay() :
		_shared_context(memnew(SharedContext())) { //NOLINT
	UtilityFunctions::print_verbose("CINEPI_DISPLAY: Constructor called.");
	if (Engine::get_singleton()->is_editor_hint()) {
		set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
	} else {
		set_process_mode(Node::ProcessMode::PROCESS_MODE_ALWAYS);
	}
	_set_material_for_displaymode();
};

CinePiDisplay::~CinePiDisplay() {
	UtilityFunctions::print_verbose("CINEPI_DISPLAY: Destructor called.");
}

void CinePiDisplay::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
	} else {
		set_process_mode(Node::ProcessMode::PROCESS_MODE_ALWAYS);
	}
	// _set_cinepi_from_path();
	_set_material_for_displaymode();
};

void CinePiDisplay::_process(double delta) { //NOLINT(misc-unused-parameters)

	_update_display_image();
}

void CinePiDisplay::_update_display_image() {
	if (!_shared_context.is_valid()) {
		UtilityFunctions::printerr("CINEPI_DISPLAY: No Shared Context connected to CinePi-Node. Can't update display image.");
		return;
	}
	if (!_shared_context->is_shared_memory_bound()) {
		UtilityFunctions::printerr("CINEPI_DISPLAY: Shared Context has no shared memory bound. Can't update display image.");
		return;
	}
	Ref<CameraStreamInfo> stream = _shared_context->get_stream_info(_camera_picture_stream);
	int fd = stream->get_fd();
	if (fd == _last_fd) {
		return;
		//in this case, there's no need to update the image because there's no new image.
	}
	_last_fd = fd;
	size_t length = stream->get_length();
	int procid = _shared_context->get_cinepi_info()->get_procid(); //NOLINT(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions,cppcoreguidelines-pro-type-vararg) This check is skipped, because the syscall is made with an int.
	int width = stream->get_width(); //NOLINT(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
	int height = stream->get_height(); //NOLINT(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
	int stride = stream->get_stride(); //NOLINT(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
	libcamera::PixelFormat pixel_format = stream->get_pixel_format();
	std::optional<libcamera::ColorSpace> color_space = stream->get_color_space();
	if (fd <= 0 || procid <= 0) {
		UtilityFunctions::printerr("CINEPI_DISPLAY: Invalid File Desciptor or ProcID.");
		return;
	}
	if (length == 0) {
		UtilityFunctions::printerr("CINEPI_DISPLAY: Memory Segment is of size 0.");
		return;
	}
	if (width == 0 || height == 0 || stride == 0) {
		UtilityFunctions::printerr("CINEPI_DISPLAY: Invalid image dimensions or stride.");
		return;
	}
	if (!color_space.has_value()) {
		UtilityFunctions::printerr("CINEPI_DISPLAY: Color Space has no value.");
		return;
	}
	std::unique_ptr<CameraImageData> camera_image_data = std::make_unique<CameraImageData>(procid, length, fd);
	if (!camera_image_data.get()->is_valid()) {
		UtilityFunctions::printerr("CINEPI_DISPLAY: Could not get Camera Image Data.");
		return;
	}
	if (_picture_decode_type == CPU && pixel_format == libcamera::formats::YUV420) {
		resize_if_needed(_dst_image_data, static_cast<size_t>(width) * height * 3);
		int result = decode_ycbcr_cpu(camera_image_data.get()->get_data(), width, height, stride, _dst_image_data.ptrw(), color_space);
		if (result != 0) {
			UtilityFunctions::printerr("CINEPI_DISPLAY: Failed decode camera image.");
			return;
		}
		set_texture(create_texture_from_packed_byte_array(_dst_image_data, width, height, Image::FORMAT_RGB8));
		return;
	} else if (_picture_decode_type == NONE || pixel_format == libcamera::formats::SBGGR16) {
		resize_if_needed(_dst_image_data, length);
		memcpy(_dst_image_data.ptrw(), camera_image_data.get()->get_data(), length);
		switch (pixel_format) {
			case libcamera::formats::YUV420:
				set_texture(create_texture_from_packed_byte_array(_dst_image_data, stride, (height * 3 / 2), Image::FORMAT_L8));
				return;
			case libcamera::formats::SBGGR16:
				set_texture(create_texture_from_packed_byte_array(_dst_image_data, stride, height, Image::FORMAT_L8));
				return;
			default:
				UtilityFunctions::printerr("CINEPI_DISPLAY: Pixel_Format not implemented.");
				return;
		}
	} else if (_picture_decode_type == DISPLAY_SHADER && pixel_format == libcamera::formats::YUV420) {
		size_t size_luma = static_cast<size_t>(stride) * height;
		resize_if_needed(_dst_image_data, size_luma);
		resize_if_needed(_dst_cb_data, size_luma / 4);
		resize_if_needed(_dst_cr_data, size_luma / 4);

		memcpy(_dst_image_data.ptrw(), camera_image_data.get()->get_data(), size_luma);
		Ref<ImageTexture> luma_texture = create_texture_from_packed_byte_array(_dst_image_data, stride, height, Image::FORMAT_L8);
		memcpy(_dst_cb_data.ptrw(), camera_image_data.get()->get_data() + static_cast<ptrdiff_t>(size_luma), size_luma / 4); //NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		Ref<ImageTexture> cb_texture = create_texture_from_packed_byte_array(_dst_cb_data, stride / 2, height / 2, Image::FORMAT_L8);
		memcpy(_dst_cr_data.ptrw(), camera_image_data.get()->get_data() + static_cast<ptrdiff_t>(size_luma) * 5 / 4, size_luma / 4); //NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic,cppcoreguidelines-avoid-magic-numbers)
		Ref<ImageTexture> cr_texture = create_texture_from_packed_byte_array(_dst_cr_data, stride / 2, height / 2, Image::FORMAT_L8);

		Ref<ShaderMaterial> shader_mat = _ycbcr_shader_mat;
		if (!shader_mat.is_valid()) {
			UtilityFunctions::printerr("CINEPI_DISPLAY: No valid shader material.");
			return;
		}

		Ref<PlaceholderTexture2D> texture = Object::cast_to<PlaceholderTexture2D>(get_texture().ptr());
		if (!texture.is_valid()) {
			texture.instantiate();
			set_texture(texture);
		}
		if (texture->get_size() != Vector2(width, height)) { //NOLINT(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions]
			texture->set_size(Vector2(width, height)); //NOLINT(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions]
		}

		shader_mat->set_shader_parameter("LumaTexture", luma_texture);
		shader_mat->set_shader_parameter("CbTexture", cb_texture);
		shader_mat->set_shader_parameter("CrTexture", cr_texture);
		shader_mat->set_shader_parameter("Width", width);

		return;
	}
}

void CinePiDisplay::_set_material_for_displaymode() {
	auto material = get_material();
	if (_picture_decode_type == CinePiDisplay::DISPLAY_SHADER && (_camera_picture_stream == SharedContext::ISP || _camera_picture_stream == SharedContext::LORES)) {
		if (material == _ycbcr_shader_mat) {
			return;
		}
		set_material(_ycbcr_shader_mat);
		return;
	}

	if (material.is_valid()) {
		set_material(Ref<Material>());
	}
}

void CinePiDisplay::_set_camera_picture_stream(int stream) {
	_camera_picture_stream = static_cast<SharedContext::CameraDataStream>(stream);
	_set_material_for_displaymode();
};

void CinePiDisplay::_set_picture_decode_type(int decode_type) {
	_picture_decode_type = static_cast<PictureDecodeType>(decode_type);
	_set_material_for_displaymode();
};

void CinePiDisplay::_set_ycbcr_shader_mat(const Ref<ShaderMaterial> ycbcr_shader_mat) { _ycbcr_shader_mat = ycbcr_shader_mat; }
void CinePiDisplay::_set_raw_shader_mat(const Ref<ShaderMaterial> raw_shader_mat) { _raw_shader_mat = raw_shader_mat; }
void CinePiDisplay::_set_zebra_shader_mat(const Ref<ShaderMaterial> zebra_shader_mat) { _zebra_shader_mat = zebra_shader_mat; }
void CinePiDisplay::_set_focus_peaking_shader_mat(const Ref<ShaderMaterial> focus_peaking_shader_mat) { _focus_peaking_shader_mat = focus_peaking_shader_mat; }

//void CinePiDisplay::_set_shared_context(Ref<SharedContext> shared_context) { _shared_context = shared_context; };

auto CinePiDisplay::_get_camera_picture_stream() -> int { return static_cast<int>(_camera_picture_stream); };
auto CinePiDisplay::_get_picture_decode_type() -> int { return static_cast<int>(_picture_decode_type); };

auto CinePiDisplay::_get_ycbcr_shader_mat() const -> Ref<ShaderMaterial> { return _ycbcr_shader_mat; }
auto CinePiDisplay::_get_raw_shader_mat() const -> Ref<ShaderMaterial> { return _raw_shader_mat; }
auto CinePiDisplay::_get_zebra_shader_mat() const -> Ref<ShaderMaterial> { return _zebra_shader_mat; }
auto CinePiDisplay::_get_focus_peaking_shader_mat() const -> Ref<ShaderMaterial> { return _focus_peaking_shader_mat; }

auto CinePiDisplay::_get_shared_context() const -> Ref<SharedContext> { return _shared_context; };

void CinePiDisplay::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_camera_picture_stream"), &CinePiDisplay::_get_camera_picture_stream);
	ClassDB::bind_method(D_METHOD("set_camera_picture_stream", "stream"), &CinePiDisplay::_set_camera_picture_stream);
	ClassDB::bind_method(D_METHOD("get_picture_decode_type"), &CinePiDisplay::_get_picture_decode_type);
	ClassDB::bind_method(D_METHOD("set_picture_decode_type", "decode_type"), &CinePiDisplay::_set_picture_decode_type);
	ClassDB::bind_method(D_METHOD("get_ycbcr_shader_mat"), &CinePiDisplay::_get_ycbcr_shader_mat);
	ClassDB::bind_method(D_METHOD("set_ycbcr_shader_mat", "ycbcr_shader_mat"), &CinePiDisplay::_set_ycbcr_shader_mat);
	ClassDB::bind_method(D_METHOD("get_raw_shader_mat"), &CinePiDisplay::_get_raw_shader_mat);
	ClassDB::bind_method(D_METHOD("set_raw_shader_mat", "raw_shader_mat"), &CinePiDisplay::_set_raw_shader_mat);
	ClassDB::bind_method(D_METHOD("get_zebra_shader_mat"), &CinePiDisplay::_get_zebra_shader_mat);
	ClassDB::bind_method(D_METHOD("set_zebra_shader_mat", "zebra_shader_mat"), &CinePiDisplay::_set_zebra_shader_mat);
	ClassDB::bind_method(D_METHOD("get_focus_peaking_shader_mat"), &CinePiDisplay::_get_focus_peaking_shader_mat);
	ClassDB::bind_method(D_METHOD("set_focus_peaking_shader_mat", "focus_peaking_shader_mat"), &CinePiDisplay::_set_focus_peaking_shader_mat);
	ClassDB::bind_method(D_METHOD("get_shared_context"), &CinePiDisplay::_get_shared_context);
	//	ClassDB::bind_method(D_METHOD("set_shared_context", "shared_context"), &CinePiDisplay::_set_shared_context);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "camera_picture_stream", PROPERTY_HINT_ENUM, "RAW,ISP,LORES", PROPERTY_USAGE_DEFAULT), "set_camera_picture_stream", "get_camera_picture_stream");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "camera_picture_decode_type", PROPERTY_HINT_ENUM, "NONE,CPU_DECODE,DISPLAY_SHADER", PROPERTY_USAGE_DEFAULT), "set_picture_decode_type", "get_picture_decode_type");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "ycbcr_shader_mat", PROPERTY_HINT_RESOURCE_TYPE, "ShaderMaterial"), "set_ycbcr_shader_mat", "get_ycbcr_shader_mat");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "raw_shader_mat", PROPERTY_HINT_RESOURCE_TYPE, "ShaderMaterial"), "set_raw_shader_mat", "get_raw_shader_mat");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "zebra_shader_mat", PROPERTY_HINT_RESOURCE_TYPE, "ShaderMaterial"), "set_zebra_shader_mat", "get_zebra_shader_mat");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "focus_peaking_shader_mat", PROPERTY_HINT_RESOURCE_TYPE, "ShaderMaterial"), "set_focus_peaking_shader_mat", "get_focus_peaking_shader_mat");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shared_context", PROPERTY_HINT_RESOURCE_TYPE, "SharedContext", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_shared_context");
}
} //namespace godot
