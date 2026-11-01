#pragma once

#include <cstdint>
#include <stdexcept>

namespace vision_encoders {

struct CropRect {
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
};

/// Center crop of a (src_w, src_h) frame that matches the (dst_w, dst_h) aspect
/// exactly, so the later resize cannot stretch it. Identity when they match.
inline CropRect aspect_center_crop(int src_w, int src_h, int dst_w, int dst_h) {
  if (src_w <= 0 || src_h <= 0 || dst_w <= 0 || dst_h <= 0)
    throw std::invalid_argument("aspect_center_crop: dimensions must be positive");
  const int64_t lhs = static_cast<int64_t>(src_w) * dst_h;
  const int64_t rhs = static_cast<int64_t>(dst_w) * src_h;
  if (lhs == rhs) return {0, 0, src_w, src_h};
  if (lhs > rhs) {  // source too wide
    const int width = static_cast<int>(rhs / dst_h);
    return {(src_w - width) / 2, 0, width, src_h};
  }
  const int height = static_cast<int>(lhs / dst_w);  // source too tall
  return {0, (src_h - height) / 2, src_w, height};
}

}  // namespace vision_encoders
