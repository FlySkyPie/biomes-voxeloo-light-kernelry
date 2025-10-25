#pragma once

#include <algorithm>
#include <array>

#include <VoxelooGeometry/geometry.hpp>

namespace voxeloo::galois::lighting {

static const std::array<int, 256> kMaskToGroupLut = {
    0,  1,  1,  2,  1,  2,  3,  4,  1,  3,  2,  4,  2,  4,  4,  5,  1,  2,  3,
    4,  3,  4,  6,  7,  8,  9,  9,  10, 9,  10, 11, 12, 1,  3,  2,  4,  8,  9,
    9,  10, 3,  6,  4,  7,  9,  11, 10, 12, 2,  4,  4,  5,  9,  10, 11, 12, 9,
    11, 10, 12, 13, 14, 14, 15, 1,  3,  8,  9,  2,  4,  9,  10, 3,  6,  9,  11,
    4,  7,  10, 12, 2,  4,  9,  10, 4,  5,  11, 12, 9,  11, 13, 14, 10, 12, 14,
    15, 3,  6,  9,  11, 9,  11, 13, 14, 6,  16, 11, 17, 11, 17, 14, 18, 4,  7,
    10, 12, 10, 12, 14, 15, 11, 17, 14, 18, 14, 18, 19, 20, 1,  8,  3,  9,  3,
    9,  6,  11, 2,  9,  4,  10, 4,  10, 7,  12, 3,  9,  6,  11, 6,  11, 16, 17,
    9,  13, 11, 14, 11, 14, 17, 18, 2,  9,  4,  10, 9,  13, 11, 14, 4,  11, 5,
    12, 10, 14, 12, 15, 4,  10, 7,  12, 11, 14, 17, 18, 10, 14, 12, 15, 14, 19,
    18, 20, 2,  9,  9,  13, 4,  10, 11, 14, 4,  11, 10, 14, 5,  12, 12, 15, 4,
    10, 11, 14, 7,  12, 17, 18, 10, 14, 14, 19, 12, 15, 18, 20, 4,  11, 10, 14,
    10, 14, 14, 19, 7,  17, 12, 18, 12, 18, 15, 20, 5,  12, 12, 15, 12, 15, 18,
    20, 12, 18, 15, 20, 15, 20, 20, 21,
};

inline auto quantize_light_value(Vec3f value) {
  return (15.0f * clamp(value, 0.0f, 1.0f) + Vec3f{0.5, 0.5, 0.5})
      .to<uint32_t>();
}

template <typename LightMask>
inline auto group_mask(const std::array<Vec3f, 8> &samples, int group) {
  LightMask out;
  switch (group) {
  case 0 /* 00000000 */:
    break;
  case 1 /* 00000001 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 1, 1}, value);
    }
    break;
  case 2 /* 00000011 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[6];
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 1, 1}, value);
      out.set({1, 1, 1}, value);
    }
    break;
  case 3 /* 00000110 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[5];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 0, 1}, value);
    }
    // Emit component 1
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[6];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 1, 1}, value);
    }
    break;
  case 4 /* 00000111 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[5];
      sum += samples[6];
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 0, 1}, value);
      out.set({0, 1, 1}, value);
      out.set({1, 1, 1}, value);
    }
    break;
  case 5 /* 00001111 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[4];
      sum += samples[5];
      sum += samples[6];
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 0, 1}, value);
      out.set({1, 0, 1}, value);
      out.set({0, 1, 1}, value);
      out.set({1, 1, 1}, value);
    }
    break;
  case 6 /* 00010110 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[3];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 1, 0}, value);
    }
    // Emit component 1
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[5];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 0, 1}, value);
    }
    // Emit component 2
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[6];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 1, 1}, value);
    }
    break;
  case 7 /* 00010111 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[3];
      sum += samples[5];
      sum += samples[6];
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 1, 0}, value);
      out.set({1, 0, 1}, value);
      out.set({0, 1, 1}, value);
      out.set({1, 1, 1}, value);
    }
    break;
  case 8 /* 00011000 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[3];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 1, 0}, value);
    }
    // Emit component 1
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[4];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 0, 1}, value);
    }
    break;
  case 9 /* 00011001 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[3];
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 1, 0}, value);
      out.set({1, 1, 1}, value);
    }
    // Emit component 1
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[4];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 0, 1}, value);
    }
    break;
  case 10 /* 00011011 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[3];
      sum += samples[4];
      sum += samples[6];
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 1, 0}, value);
      out.set({0, 0, 1}, value);
      out.set({0, 1, 1}, value);
      out.set({1, 1, 1}, value);
    }
    break;
  case 11 /* 00011110 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[3];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 1, 0}, value);
    }
    // Emit component 1
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[4];
      sum += samples[5];
      sum += samples[6];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 0, 1}, value);
      out.set({1, 0, 1}, value);
      out.set({0, 1, 1}, value);
    }
    break;
  case 12 /* 00011111 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[3];
      sum += samples[4];
      sum += samples[5];
      sum += samples[6];
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 1, 0}, value);
      out.set({0, 0, 1}, value);
      out.set({1, 0, 1}, value);
      out.set({0, 1, 1}, value);
      out.set({1, 1, 1}, value);
    }
    break;
  case 13 /* 00111100 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[2];
      sum += samples[3];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 1, 0}, value);
      out.set({1, 1, 0}, value);
    }
    // Emit component 1
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[4];
      sum += samples[5];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 0, 1}, value);
      out.set({1, 0, 1}, value);
    }
    break;
  case 14 /* 00111101 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[2];
      sum += samples[3];
      sum += samples[4];
      sum += samples[5];
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 1, 0}, value);
      out.set({1, 1, 0}, value);
      out.set({0, 0, 1}, value);
      out.set({1, 0, 1}, value);
      out.set({1, 1, 1}, value);
    }
    break;
  case 15 /* 00111111 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[2];
      sum += samples[3];
      sum += samples[4];
      sum += samples[5];
      sum += samples[6];
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 1, 0}, value);
      out.set({1, 1, 0}, value);
      out.set({0, 0, 1}, value);
      out.set({1, 0, 1}, value);
      out.set({0, 1, 1}, value);
      out.set({1, 1, 1}, value);
    }
    break;
  case 16 /* 01101001 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[1];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 0, 0}, value);
    }
    // Emit component 1
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[2];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 1, 0}, value);
    }
    // Emit component 2
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[4];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 0, 1}, value);
    }
    // Emit component 3
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 1, 1}, value);
    }
    break;
  case 17 /* 01101011 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[1];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 0, 0}, value);
    }
    // Emit component 1
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[2];
      sum += samples[4];
      sum += samples[6];
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 1, 0}, value);
      out.set({0, 0, 1}, value);
      out.set({0, 1, 1}, value);
      out.set({1, 1, 1}, value);
    }
    break;
  case 18 /* 01101111 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[1];
      sum += samples[2];
      sum += samples[4];
      sum += samples[5];
      sum += samples[6];
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 0, 0}, value);
      out.set({0, 1, 0}, value);
      out.set({0, 0, 1}, value);
      out.set({1, 0, 1}, value);
      out.set({0, 1, 1}, value);
      out.set({1, 1, 1}, value);
    }
    break;
  case 19 /* 01111110 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[1];
      sum += samples[2];
      sum += samples[3];
      sum += samples[4];
      sum += samples[5];
      sum += samples[6];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 0, 0}, value);
      out.set({0, 1, 0}, value);
      out.set({1, 1, 0}, value);
      out.set({0, 0, 1}, value);
      out.set({1, 0, 1}, value);
      out.set({0, 1, 1}, value);
    }
    break;
  case 20 /* 01111111 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[1];
      sum += samples[2];
      sum += samples[3];
      sum += samples[4];
      sum += samples[5];
      sum += samples[6];
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({1, 0, 0}, value);
      out.set({0, 1, 0}, value);
      out.set({1, 1, 0}, value);
      out.set({0, 0, 1}, value);
      out.set({1, 0, 1}, value);
      out.set({0, 1, 1}, value);
      out.set({1, 1, 1}, value);
    }
    break;
  case 21 /* 11111111 */:
    // Emit component 0
    {
      Vec3f sum{0.0, 0.0, 0.0};
      sum += samples[0];
      sum += samples[1];
      sum += samples[2];
      sum += samples[3];
      sum += samples[4];
      sum += samples[5];
      sum += samples[6];
      sum += samples[7];

      // Quantize the vertex light value.
      auto value = quantize_light_value(sum / 8.0f);

      // Emit the final quantized light value for each corner.
      out.set({0, 0, 0}, value);
      out.set({1, 0, 0}, value);
      out.set({0, 1, 0}, value);
      out.set({1, 1, 0}, value);
      out.set({0, 0, 1}, value);
      out.set({1, 0, 1}, value);
      out.set({0, 1, 1}, value);
      out.set({1, 1, 1}, value);
    }
    break;
  default:
    std::cout << "Invalid isomorphism group";
    // CHECK_UNREACHABLE("Invalid isomorphism group");
  }
  return out;
}

inline auto permute_samples(const std::array<Vec3f, 8> &samples, int permute) {
  std::array<Vec3f, 8> out = samples;
  switch (permute) {
  case 0 /* [0, 1, 2] */:
    break;
  case 1 /* [0, 2, 1] */:
    out[1] = samples[2];
    out[2] = samples[1];
    out[5] = samples[6];
    out[6] = samples[5];
    break;
  case 2 /* [1, 0, 2] */:
    out[2] = samples[4];
    out[3] = samples[5];
    out[4] = samples[2];
    out[5] = samples[3];
    break;
  case 3 /* [1, 2, 0] */:
    out[1] = samples[4];
    out[2] = samples[1];
    out[3] = samples[5];
    out[4] = samples[2];
    out[5] = samples[6];
    out[6] = samples[3];
    break;
  case 4 /* [2, 0, 1] */:
    out[1] = samples[2];
    out[2] = samples[4];
    out[3] = samples[6];
    out[4] = samples[1];
    out[5] = samples[3];
    out[6] = samples[5];
    break;
  case 5 /* [2, 1, 0] */:
    out[1] = samples[4];
    out[3] = samples[6];
    out[4] = samples[1];
    out[6] = samples[3];
    break;
  default:
    std::cout << "Invalid permutation";
    // CHECK_UNREACHABLE("Invalid permutation");
  }
  return out;
}

inline auto reflect_samples(const std::array<Vec3f, 8> &samples, int reflect) {
  std::array<Vec3f, 8> out = samples;
  switch (reflect) {
  case 0 /* [0, 0, 0] */:
    break;
  case 1 /* [1, 0, 0] */:
    out[0] = samples[4];
    out[1] = samples[5];
    out[2] = samples[6];
    out[3] = samples[7];
    out[4] = samples[0];
    out[5] = samples[1];
    out[6] = samples[2];
    out[7] = samples[3];
    break;
  case 2 /* [0, 1, 0] */:
    out[0] = samples[2];
    out[1] = samples[3];
    out[2] = samples[0];
    out[3] = samples[1];
    out[4] = samples[6];
    out[5] = samples[7];
    out[6] = samples[4];
    out[7] = samples[5];
    break;
  case 3 /* [1, 1, 0] */:
    out[0] = samples[6];
    out[1] = samples[7];
    out[2] = samples[4];
    out[3] = samples[5];
    out[4] = samples[2];
    out[5] = samples[3];
    out[6] = samples[0];
    out[7] = samples[1];
    break;
  case 4 /* [0, 0, 1] */:
    out[0] = samples[1];
    out[1] = samples[0];
    out[2] = samples[3];
    out[3] = samples[2];
    out[4] = samples[5];
    out[5] = samples[4];
    out[6] = samples[7];
    out[7] = samples[6];
    break;
  case 5 /* [1, 0, 1] */:
    out[0] = samples[5];
    out[1] = samples[4];
    out[2] = samples[7];
    out[3] = samples[6];
    out[4] = samples[1];
    out[5] = samples[0];
    out[6] = samples[3];
    out[7] = samples[2];
    break;
  case 6 /* [0, 1, 1] */:
    out[0] = samples[3];
    out[1] = samples[2];
    out[2] = samples[1];
    out[3] = samples[0];
    out[4] = samples[7];
    out[5] = samples[6];
    out[6] = samples[5];
    out[7] = samples[4];
    break;
  case 7 /* [1, 1, 1] */:
    out[0] = samples[7];
    out[1] = samples[6];
    out[2] = samples[5];
    out[3] = samples[4];
    out[4] = samples[3];
    out[5] = samples[2];
    out[6] = samples[1];
    out[7] = samples[0];
    break;
  default:
    std::cout << "Invalid reflection";
    // CHECK_UNREACHABLE("Invalid reflection");
  }
  return out;
}

static const std::array<int, 256> kSamplesPermuteLut = {
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 3, 2, 2, 4, 4, 0, 0,
    0, 0, 2, 0, 4, 1, 0, 0, 0, 2, 3, 2, 0, 2, 0, 0, 4, 0, 4, 0, 4, 0, 1, 0,
    0, 2, 2, 2, 4, 3, 2, 2, 4, 2, 3, 2, 0, 0, 0, 0, 0, 4, 0, 4, 3, 4, 0, 1,
    2, 0, 2, 0, 2, 0, 0, 0, 1, 4, 2, 3, 4, 4, 4, 4, 2, 4, 1, 1, 3, 4, 1, 1,
    0, 0, 0, 2, 0, 4, 3, 3, 0, 0, 4, 0, 2, 0, 3, 0, 0, 0, 0, 2, 1, 4, 3, 3,
    0, 0, 1, 2, 0, 4, 0, 0, 0, 0, 4, 4, 2, 2, 0, 0, 3, 0, 4, 1, 2, 0, 0, 0,
    0, 0, 0, 2, 0, 4, 0, 0, 0, 3, 4, 3, 2, 3, 0, 0, 1, 2, 4, 3, 2, 1, 4, 1,
    4, 4, 4, 4, 3, 1, 4, 1, 0, 0, 0, 2, 0, 1, 0, 2, 1, 3, 4, 3, 0, 0, 4, 0,
    0, 4, 4, 0, 2, 3, 2, 0, 2, 2, 3, 0, 2, 2, 2, 0, 0, 1, 0, 0, 0, 4, 0, 4,
    0, 3, 1, 0, 2, 3, 2, 0, 0, 0, 1, 0, 0, 1, 3, 0, 0, 0, 4, 4, 2, 2, 3, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
};

static const std::array<int, 256> kSamplesReflectLut = {
    0, 0, 4, 0, 2, 0, 0, 0, 6, 2, 2, 4, 2, 2, 6, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 0, 4, 0, 0, 0, 5, 2, 2, 4, 3, 0, 4, 4, 1, 4, 1, 4, 5, 4, 2, 4,
    1, 2, 6, 0, 2, 0, 0, 0, 3, 4, 2, 4, 0, 0, 4, 0, 3, 2, 2, 0, 1, 4, 2, 4,
    1, 2, 5, 2, 1, 2, 2, 2, 1, 2, 2, 4, 6, 0, 0, 0, 3, 4, 0, 0, 5, 4, 4, 0,
    1, 1, 5, 2, 3, 2, 0, 0, 7, 0, 7, 0, 7, 6, 3, 0, 1, 1, 5, 2, 5, 2, 4, 0,
    7, 5, 3, 0, 3, 0, 0, 0, 7, 1, 3, 1, 3, 1, 6, 6, 3, 6, 5, 6, 5, 6, 6, 6,
    3, 1, 5, 6, 3, 6, 1, 4, 7, 1, 3, 2, 3, 1, 2, 2, 3, 6, 3, 6, 7, 1, 5, 2,
    7, 1, 1, 1, 7, 6, 5, 2, 5, 1, 5, 6, 3, 1, 1, 2, 7, 6, 3, 2, 7, 3, 1, 4,
    3, 6, 7, 1, 3, 1, 5, 2, 7, 1, 3, 6, 1, 1, 5, 2, 3, 1, 5, 1, 3, 6, 7, 2,
    7, 5, 7, 2, 3, 1, 1, 2, 7, 1, 3, 5, 3, 5, 7, 1, 7, 3, 7, 3, 7, 3, 3, 6,
    1, 1, 5, 1, 3, 1, 1, 1, 7, 3, 3, 5, 3, 3, 7, 0,
};

inline auto transform_samples(const std::array<Vec3f, 8> &samples,
                              uint8_t occlusion_mask) {
  auto permute = kSamplesPermuteLut[occlusion_mask];
  auto reflect = kSamplesReflectLut[occlusion_mask];
  return reflect_samples(permute_samples(samples, permute), reflect);
}

template <typename LightMask>
inline auto permute_mask(LightMask mask, int permute) {
  LightMask out = mask;
  switch (permute) {
  case 0 /* [0, 1, 2] */:
    break;
  case 1 /* [0, 2, 1] */:
    out.set({1, 0, 0}, mask.get({0, 1, 0}));
    out.set({0, 1, 0}, mask.get({1, 0, 0}));
    out.set({1, 0, 1}, mask.get({0, 1, 1}));
    out.set({0, 1, 1}, mask.get({1, 0, 1}));
    break;
  case 2 /* [1, 0, 2] */:
    out.set({0, 1, 0}, mask.get({0, 0, 1}));
    out.set({1, 1, 0}, mask.get({1, 0, 1}));
    out.set({0, 0, 1}, mask.get({0, 1, 0}));
    out.set({1, 0, 1}, mask.get({1, 1, 0}));
    break;
  case 3 /* [1, 2, 0] */:
    out.set({1, 0, 0}, mask.get({0, 0, 1}));
    out.set({0, 1, 0}, mask.get({1, 0, 0}));
    out.set({1, 1, 0}, mask.get({1, 0, 1}));
    out.set({0, 0, 1}, mask.get({0, 1, 0}));
    out.set({1, 0, 1}, mask.get({0, 1, 1}));
    out.set({0, 1, 1}, mask.get({1, 1, 0}));
    break;
  case 4 /* [2, 0, 1] */:
    out.set({1, 0, 0}, mask.get({0, 1, 0}));
    out.set({0, 1, 0}, mask.get({0, 0, 1}));
    out.set({1, 1, 0}, mask.get({0, 1, 1}));
    out.set({0, 0, 1}, mask.get({1, 0, 0}));
    out.set({1, 0, 1}, mask.get({1, 1, 0}));
    out.set({0, 1, 1}, mask.get({1, 0, 1}));
    break;
  case 5 /* [2, 1, 0] */:
    out.set({1, 0, 0}, mask.get({0, 0, 1}));
    out.set({1, 1, 0}, mask.get({0, 1, 1}));
    out.set({0, 0, 1}, mask.get({1, 0, 0}));
    out.set({0, 1, 1}, mask.get({1, 1, 0}));
    break;
  default:
    std::cout << "Invalid permutation";
    // CHECK_UNREACHABLE("Invalid permutation");
  }
  return out;
}

template <typename LightMask>
inline auto reflect_mask(LightMask mask, int reflect) {
  LightMask out = mask;
  switch (reflect) {
  case 0 /* [0, 0, 0] */:
    break;
  case 1 /* [1, 0, 0] */:
    out.set({0, 0, 0}, mask.get({0, 0, 1}));
    out.set({1, 0, 0}, mask.get({1, 0, 1}));
    out.set({0, 1, 0}, mask.get({0, 1, 1}));
    out.set({1, 1, 0}, mask.get({1, 1, 1}));
    out.set({0, 0, 1}, mask.get({0, 0, 0}));
    out.set({1, 0, 1}, mask.get({1, 0, 0}));
    out.set({0, 1, 1}, mask.get({0, 1, 0}));
    out.set({1, 1, 1}, mask.get({1, 1, 0}));
    break;
  case 2 /* [0, 1, 0] */:
    out.set({0, 0, 0}, mask.get({0, 1, 0}));
    out.set({1, 0, 0}, mask.get({1, 1, 0}));
    out.set({0, 1, 0}, mask.get({0, 0, 0}));
    out.set({1, 1, 0}, mask.get({1, 0, 0}));
    out.set({0, 0, 1}, mask.get({0, 1, 1}));
    out.set({1, 0, 1}, mask.get({1, 1, 1}));
    out.set({0, 1, 1}, mask.get({0, 0, 1}));
    out.set({1, 1, 1}, mask.get({1, 0, 1}));
    break;
  case 3 /* [1, 1, 0] */:
    out.set({0, 0, 0}, mask.get({0, 1, 1}));
    out.set({1, 0, 0}, mask.get({1, 1, 1}));
    out.set({0, 1, 0}, mask.get({0, 0, 1}));
    out.set({1, 1, 0}, mask.get({1, 0, 1}));
    out.set({0, 0, 1}, mask.get({0, 1, 0}));
    out.set({1, 0, 1}, mask.get({1, 1, 0}));
    out.set({0, 1, 1}, mask.get({0, 0, 0}));
    out.set({1, 1, 1}, mask.get({1, 0, 0}));
    break;
  case 4 /* [0, 0, 1] */:
    out.set({0, 0, 0}, mask.get({1, 0, 0}));
    out.set({1, 0, 0}, mask.get({0, 0, 0}));
    out.set({0, 1, 0}, mask.get({1, 1, 0}));
    out.set({1, 1, 0}, mask.get({0, 1, 0}));
    out.set({0, 0, 1}, mask.get({1, 0, 1}));
    out.set({1, 0, 1}, mask.get({0, 0, 1}));
    out.set({0, 1, 1}, mask.get({1, 1, 1}));
    out.set({1, 1, 1}, mask.get({0, 1, 1}));
    break;
  case 5 /* [1, 0, 1] */:
    out.set({0, 0, 0}, mask.get({1, 0, 1}));
    out.set({1, 0, 0}, mask.get({0, 0, 1}));
    out.set({0, 1, 0}, mask.get({1, 1, 1}));
    out.set({1, 1, 0}, mask.get({0, 1, 1}));
    out.set({0, 0, 1}, mask.get({1, 0, 0}));
    out.set({1, 0, 1}, mask.get({0, 0, 0}));
    out.set({0, 1, 1}, mask.get({1, 1, 0}));
    out.set({1, 1, 1}, mask.get({0, 1, 0}));
    break;
  case 6 /* [0, 1, 1] */:
    out.set({0, 0, 0}, mask.get({1, 1, 0}));
    out.set({1, 0, 0}, mask.get({0, 1, 0}));
    out.set({0, 1, 0}, mask.get({1, 0, 0}));
    out.set({1, 1, 0}, mask.get({0, 0, 0}));
    out.set({0, 0, 1}, mask.get({1, 1, 1}));
    out.set({1, 0, 1}, mask.get({0, 1, 1}));
    out.set({0, 1, 1}, mask.get({1, 0, 1}));
    out.set({1, 1, 1}, mask.get({0, 0, 1}));
    break;
  case 7 /* [1, 1, 1] */:
    out.set({0, 0, 0}, mask.get({1, 1, 1}));
    out.set({1, 0, 0}, mask.get({0, 1, 1}));
    out.set({0, 1, 0}, mask.get({1, 0, 1}));
    out.set({1, 1, 0}, mask.get({0, 0, 1}));
    out.set({0, 0, 1}, mask.get({1, 1, 0}));
    out.set({1, 0, 1}, mask.get({0, 1, 0}));
    out.set({0, 1, 1}, mask.get({1, 0, 0}));
    out.set({1, 1, 1}, mask.get({0, 0, 0}));
    break;
  default:
    std::cout << "Invalid reflection";
    // CHECK_UNREACHABLE("Invalid reflection");
  }
  return out;
}

static const std::array<int, 256> kMaskPermuteLut = {
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 4, 2, 2, 3, 3, 0, 0,
    0, 0, 2, 0, 3, 1, 0, 0, 0, 2, 4, 2, 0, 2, 0, 0, 3, 0, 3, 0, 3, 0, 1, 0,
    0, 2, 2, 2, 3, 4, 2, 2, 3, 2, 4, 2, 0, 0, 0, 0, 0, 3, 0, 3, 4, 3, 0, 1,
    2, 0, 2, 0, 2, 0, 0, 0, 1, 3, 2, 4, 3, 3, 3, 3, 2, 3, 1, 1, 4, 3, 1, 1,
    0, 0, 0, 2, 0, 3, 4, 4, 0, 0, 3, 0, 2, 0, 4, 0, 0, 0, 0, 2, 1, 3, 4, 4,
    0, 0, 1, 2, 0, 3, 0, 0, 0, 0, 3, 3, 2, 2, 0, 0, 4, 0, 3, 1, 2, 0, 0, 0,
    0, 0, 0, 2, 0, 3, 0, 0, 0, 4, 3, 4, 2, 4, 0, 0, 1, 2, 3, 4, 2, 1, 3, 1,
    3, 3, 3, 3, 4, 1, 3, 1, 0, 0, 0, 2, 0, 1, 0, 2, 1, 4, 3, 4, 0, 0, 3, 0,
    0, 3, 3, 0, 2, 4, 2, 0, 2, 2, 4, 0, 2, 2, 2, 0, 0, 1, 0, 0, 0, 3, 0, 3,
    0, 4, 1, 0, 2, 4, 2, 0, 0, 0, 1, 0, 0, 1, 4, 0, 0, 0, 3, 3, 2, 2, 4, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
};

static const std::array<int, 256> kMaskReflectLut = {
    0, 0, 4, 0, 2, 0, 0, 0, 6, 2, 4, 4, 2, 2, 6, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 0, 2, 0, 0, 0, 5, 1, 4, 4, 3, 0, 4, 4, 4, 4, 4, 4, 6, 4, 4, 4,
    1, 1, 5, 0, 1, 0, 0, 0, 5, 4, 4, 4, 0, 0, 4, 0, 3, 1, 2, 0, 2, 2, 2, 2,
    2, 2, 6, 2, 2, 2, 2, 2, 1, 1, 1, 1, 3, 0, 0, 0, 3, 2, 0, 0, 3, 2, 2, 0,
    1, 1, 5, 1, 3, 1, 0, 0, 7, 0, 7, 0, 7, 6, 6, 0, 1, 1, 5, 1, 3, 1, 1, 0,
    7, 5, 5, 0, 3, 0, 0, 0, 7, 1, 5, 4, 3, 2, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    3, 1, 5, 5, 3, 3, 1, 4, 7, 2, 5, 4, 3, 2, 2, 2, 5, 5, 5, 5, 7, 1, 6, 4,
    7, 4, 4, 4, 7, 6, 6, 4, 5, 1, 5, 5, 3, 1, 1, 1, 7, 5, 5, 4, 7, 3, 4, 4,
    3, 3, 7, 1, 3, 2, 6, 2, 7, 2, 6, 6, 2, 2, 6, 2, 3, 1, 5, 1, 3, 3, 7, 1,
    7, 3, 7, 2, 3, 2, 2, 2, 7, 1, 5, 5, 3, 3, 7, 1, 7, 3, 7, 5, 7, 3, 6, 6,
    1, 1, 5, 1, 3, 1, 1, 1, 7, 3, 5, 5, 3, 3, 7, 0,
};

template <typename LightMask>
inline auto transform_mask(LightMask out, uint8_t occlusion_mask) {
  auto permute = kMaskPermuteLut[occlusion_mask];
  auto reflect = kMaskReflectLut[occlusion_mask];
  return reflect_mask(permute_mask(out, permute), reflect);
}

template <typename LightMask>
inline auto
apply_light_kernel_with_occlusion(uint8_t occlusion_mask,
                                  const std::array<Vec3f, 8> &samples) {
  // Transform the samples to the group version.
  auto group_samples = transform_samples(samples, occlusion_mask);

  // Get the light mask for the occlusion mask's isomorphism group.
  auto group = kMaskToGroupLut[occlusion_mask];
  auto light_mask = group_mask<LightMask>(group_samples, group);

  // Transform the light mask to the final output.
  return transform_mask<LightMask>(light_mask, occlusion_mask);
}

template <typename LightMask>
inline auto apply_light_kernel(const std::array<Vec3f, 8> &samples) {
  Vec3f sum{0.0, 0.0, 0.0};
  for (auto sample : samples) {
    sum += sample;
  }

  // Quantize the vertex light value.
  auto value = quantize_light_value(sum / 8.0f);

  // Write the output to each corner.
  LightMask out;
  for (auto dz : {0u, 1u}) {
    for (auto dy : {0u, 1u}) {
      for (auto dx : {0u, 1u}) {
        out.set({dx, dy, dz}, value);
      }
    }
  }
  return out;
}

} // namespace voxeloo::galois::lighting
