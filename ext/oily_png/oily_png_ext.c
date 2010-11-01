#include "oily_png_ext.h"

void Init_oily_png_ext() {
  VALUE OilyPNG = rb_define_module("OilyPNG");

  // Setup decoding
  VALUE OilyPNG_PNGDecoding = rb_define_module_under(OilyPNG, "PNGDecoding");
  rb_define_method(OilyPNG_PNGDecoding, "decode_png_image_pass", oily_png_decode_png_image_pass, 6);
  
  // Setup encoding
  VALUE OilyPNG_PNGEncoding = rb_define_module_under(OilyPNG, "PNGEncoding");
  rb_define_method(OilyPNG_PNGEncoding, "encode_png_image_pass_to_stream", oily_png_encode_png_image_pass_to_stream, 3);
}

int oily_png_samples_per_pixel(int color_mode) {
  switch (color_mode) {
    case OILY_PNG_COLOR_GRAYSCALE:       return 1;
    case OILY_PNG_COLOR_TRUECOLOR:       return 3;
    case OILY_PNG_COLOR_INDEXED:         return 1;
    case OILY_PNG_COLOR_GRAYSCALE_ALPHA: return 2;
    case OILY_PNG_COLOR_TRUECOLOR_ALPHA: return 4;
    default: rb_raise(rb_eRuntimeError, "Unsupported color mode: %d", color_mode);
  }
}

int oily_png_pixel_bitsize(int color_mode, int bit_depth) {
  return oily_png_samples_per_pixel(color_mode) * bit_depth;
}

int oily_png_pixel_bytesize(int color_mode, int bit_depth) {
  return (bit_depth < 8) ? 1 : (oily_png_pixel_bitsize(color_mode, bit_depth) + 7) >> 3;
}

int oily_png_scanline_bytesize(int color_mode, int bit_depth, int width) {
  return 8 + ((oily_png_pixel_bitsize(color_mode, bit_depth) * width) + 7) >> 3;
}

int oily_png_pass_bytesize(int color_mode, int bit_depth, int width, int height) {
  return (width == 0 || height == 0) ? 0 : (oily_png_scanline_bytesize(color_mode, bit_depth, width)) * height;
}
