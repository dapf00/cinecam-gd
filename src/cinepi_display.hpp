#ifndef CINEPI_DISPLAY_HPP
#define CINEPI_DISPLAY_HPP

#include "shared_context.hpp"

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/texture_rect.hpp>

namespace godot {

class CinePiDisplay : public TextureRect {
	GDCLASS(CinePiDisplay, TextureRect); // NOLINT

public:
	CinePiDisplay();
	~CinePiDisplay() override;
	CinePiDisplay(const CinePiDisplay &) = delete;
	CinePiDisplay(CinePiDisplay &&) = delete;
	void operator=(CinePiDisplay &&) = delete;

	void _process(double delta) override;
	void _ready() override;

protected:
	static void _bind_methods();

private:
	enum PictureDecodeType {
		NONE,
		CPU,
		DISPLAY_SHADER
	};

	const Ref<SharedContext> _shared_context;

	Ref<ImageTexture> _texture;

	Ref<ShaderMaterial> _ycbcr_shader_mat;
	Ref<ShaderMaterial> _raw_shader_mat;
	Ref<ShaderMaterial> _zebra_shader_mat;
	Ref<ShaderMaterial> _focus_peaking_shader_mat;

	enum SharedContext::CameraDataStream _camera_picture_stream = SharedContext::LORES;
	enum PictureDecodeType _picture_decode_type = DISPLAY_SHADER;

	int _last_fd = 0;
	int _video_size = 0;
	PackedByteArray _dst_image_data;
	PackedByteArray _dst_cb_data;
	PackedByteArray _dst_cr_data;

	void _update_display_image();
	void _set_material_for_displaymode();
	void _set_display_image_texture(Ref<Image> image);

	void _set_picture_decode_type(int decode_type);
	void _set_camera_picture_stream(int stream);

	void _set_ycbcr_shader_mat(const Ref<ShaderMaterial> ycbcr_shader_mat);
	void _set_raw_shader_mat(const Ref<ShaderMaterial> raw_shader_mat);
	void _set_zebra_shader_mat(const Ref<ShaderMaterial> zebra_shader_mat);
	void _set_focus_peaking_shader_mat(const Ref<ShaderMaterial> focus_peaking_shader_mat);

	//void _set_shared_context(Ref<SharedContext> shared_context);

	[[nodiscard]] auto _get_picture_decode_type() -> int;
	[[nodiscard]] auto _get_camera_picture_stream() -> int;

	[[nodiscard]] auto _get_ycbcr_shader_mat() const -> Ref<ShaderMaterial>;
	[[nodiscard]] auto _get_raw_shader_mat() const -> Ref<ShaderMaterial>;
	[[nodiscard]] auto _get_zebra_shader_mat() const -> Ref<ShaderMaterial>;
	[[nodiscard]] auto _get_focus_peaking_shader_mat() const -> Ref<ShaderMaterial>;

	[[nodiscard]] auto _get_shared_context() const -> Ref<SharedContext>;
};
} //namespace godot
#endif
