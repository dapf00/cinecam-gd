#include "decoder_functions.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

#include <libyuv/convert_argb.h>

namespace godot {
auto decode_ycbcr_cpu(uint8_t *src_image_data, int width, int height, int stride, uint8_t *dst_image_data, std::optional<libcamera::ColorSpace> color_space) -> int {
	uint8_t *cb = src_image_data + static_cast<ptrdiff_t>(height) * static_cast<ptrdiff_t>(stride) * 5 / 4;
	uint8_t *cr = src_image_data + static_cast<ptrdiff_t>(height) * static_cast<ptrdiff_t>(stride); //NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	//NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic,cppcoreguidelines-avoid-magic-numbers)

	int result = -1;

	switch (color_space.value().range) {
		case libcamera::ColorSpace::Range::Limited:
			switch (color_space.value().ycbcrEncoding) {
				case libcamera::ColorSpace::YcbcrEncoding::Rec601:
					libyuv::I420ToRGB24Matrix(src_image_data, stride, cb, stride / 2, cr, stride / 2, dst_image_data, width * 3, &libyuv::kYvuI601Constants, width, height);
					result = 0;
					break;
				case libcamera::ColorSpace::YcbcrEncoding::Rec709:
					libyuv::I420ToRGB24Matrix(src_image_data, stride, cb, stride / 2, cr, stride / 2, dst_image_data, width * 3, &libyuv::kYvuH709Constants, width, height);
					// libyuv::I420ToRGB24(y, stride, cr, stride / 2, cb, stride / 2, dst_image_data, width * 3, width, height);
					result = 0;
					break;
				case libcamera::ColorSpace::YcbcrEncoding::Rec2020:
					libyuv::I420ToRGB24Matrix(src_image_data, stride, cb, stride / 2, cr, stride / 2, dst_image_data, width * 3, &libyuv::kYvu2020Constants, width, height);
					result = 0;
					break;
				default:
					result = -1;
			}
			break;
		case libcamera::ColorSpace::Range::Full:
			switch (color_space.value().ycbcrEncoding) {
				case libcamera::ColorSpace::YcbcrEncoding::Rec601:
					libyuv::I420ToRGB24Matrix(src_image_data, stride, cb, stride / 2, cr, stride / 2, dst_image_data, width * 3, &libyuv::kYvuJPEGConstants, width, height);
					result = 0;
					break;
				case libcamera::ColorSpace::YcbcrEncoding::Rec709:
					libyuv::I420ToRGB24Matrix(src_image_data, stride, cb, stride / 2, cr, stride / 2, dst_image_data, width * 3, &libyuv::kYvuF709Constants, width, height);
					result = 0;
					break;
				case libcamera::ColorSpace::YcbcrEncoding::Rec2020:
					libyuv::I420ToRGB24Matrix(src_image_data, stride, cb, stride / 2, cr, stride / 2, dst_image_data, width * 3, &libyuv::kYvuV2020Constants, width, height);
					result = 0;
					break;
				default:
					return -1;
			}
			break;
		default:
			UtilityFunctions::printerr("DECODER_FUNCTIONS: Color Space has no valid range.");
			return -1;
	}
	if (result < 0) {
		UtilityFunctions::printerr("DECODER_FUNCTIONS: Decode failed.");
		return -1;
	}
	return 0;
}

void resize_if_needed(PackedByteArray &pba, size_t new_size) {
	if (pba.size() != new_size) {
		pba.resize(new_size);
	}
};
auto create_texture_from_packed_byte_array(const PackedByteArray &pba, int width, int height, Image::Format format) -> Ref<ImageTexture> {
	Ref<Image> image = memnew(Image); //NOLINT(cppcoreguidelines-owning-memory)
	image->set_data(width, height, false, format, pba);
	Ref<ImageTexture> texture = memnew(ImageTexture);
	texture->set_image(image);
	return texture;
}
} //namespace godot
