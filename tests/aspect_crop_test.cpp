// The D435 path streamed 640x480 (4:3) and the encoder squashed it to the 16:9
// input the policy trained on. These pin the guard that stops that stretch.
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <utility>

#include "vision_encoders/aspect_crop.hpp"

#define CHECK(x)                                            \
  do {                                                      \
    if (!(x)) {                                             \
      std::fprintf(stderr, "FAIL %s:%d %s\n", __FILE__, __LINE__, #x); \
      std::exit(1);                                         \
    }                                                       \
  } while (0)

using vision_encoders::aspect_center_crop;
using vision_encoders::CropRect;

static bool same(const CropRect& c, int x, int y, int w, int h) {
  return c.x == x && c.y == y && c.width == w && c.height == h;
}

int main() {
  // Exact 16:9 multiples pass untouched: no crop, no stretch, no field of view lost.
  CHECK(same(aspect_center_crop(1120, 630, 112, 63), 0, 0, 1120, 630));
  CHECK(same(aspect_center_crop(112, 63, 112, 63), 0, 0, 112, 63));
  // The failure: 4:3 into 16:9 keeps the width and loses the top and bottom.
  CHECK(same(aspect_center_crop(640, 480, 112, 63), 0, 60, 640, 360));
  // Too wide: keeps the height and loses the sides.
  CHECK(same(aspect_center_crop(800, 315, 112, 63), 120, 0, 560, 315));
  // Negative paths: nonsense dimensions throw instead of dividing by zero.
  for (const auto& d : {std::pair<int, int>{0, 480}, std::pair<int, int>{640, -1}}) {
    bool threw = false;
    try {
      aspect_center_crop(d.first, d.second, 112, 63);
    } catch (const std::invalid_argument&) {
      threw = true;
    }
    CHECK(threw);
  }
  bool threw = false;
  try {
    aspect_center_crop(640, 480, 0, 63);
  } catch (const std::invalid_argument&) {
    threw = true;
  }
  CHECK(threw);
  std::puts("ok  aspect_crop");
  return 0;
}
