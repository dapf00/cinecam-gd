#ifndef DECODER_FUNCTIONS_HPP
#define DECODER_FUNCTIONS_HPP

#include <optional>

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>

#include <libcamera/color_space.h>
#include <libcamera/formats.h>

namespace godot {
auto decode_ycbcr_cpu(uint8_t *src_image_data, int width, int height, int stride, uint8_t *dst_image_data, std::optional<libcamera::ColorSpace> color_space) -> int;
void resize_if_needed(PackedByteArray &pba, size_t new_size);
auto create_texture_from_packed_byte_array(const PackedByteArray &pba, int width, int height, Image::Format format) -> Ref<ImageTexture>;
} //namespace godot

#endif
