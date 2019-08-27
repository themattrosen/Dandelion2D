#pragma once

#include "SIMD.h"
#include "Vector.h"
#include "Matrix.h"
#include <cmath>

#define M_PI 3.141593f

#define DEG_TO_RAD(deg) deg * M_PI / 180.0f
#define RAD_TO_DEG(rad) rad * 180.0f / M_PI

#define VEC2_SQ_LENGTH(v2) (v2.x * v2.x + v2.y * v2.y)

#define VEC2_LENGTH(v2) std::sqrt(VEC2_SQ_LENGTH(v2))

#define NORMALIZE_VEC2(v2) v2 *= 1.f / \
                          ((VEC2_SQ_LENGTH(v2) == 0.f) ? 1.0f : VEC2_LENGTH(v2))

#define LERP_FLOAT(start, end, factor) ((start * (1.0f - factor)) + (end * factor))

#define LERP_VEC2(start, end, factor) (vec2(LERP_FLOAT((start).x, (end).x, factor), \
                                            LERP_FLOAT((start).y, (end).y, factor)))

#define LERP_VEC4(start, end, factor) (vec4(LERP_FLOAT((start).x, (end).x, factor), \
											LERP_FLOAT((start).y, (end).y, factor), \
											LERP_FLOAT((start).z, (end).z, factor), \
                                            LERP_FLOAT((start).w, (end).w, factor)))


#define CLAMP(low, high, val) ( (val > high) ? high : (val < low) ? low : val )
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )

#define RAND_BETWEEN(min, max) (std::rand() % (max - min + 1) + min)
