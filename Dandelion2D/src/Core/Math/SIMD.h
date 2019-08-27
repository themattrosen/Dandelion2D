#pragma once

#include <xmmintrin.h>

using SMD = __m128;

#define SMD_ADD(s1, s2) (_mm_add_ps(s1, s2))
#define SMD_SUB(s1, s2) (_mm_sub_ps(s1, s2))
#define SMD_MUL(s1, s2) (_mm_mul_ps(s1, s2))
#define SMD_LOAD(float_ptr) (_mm_load_ps(float_ptr))
#define SMD_STORE(ptr, smd) (_mm_store_ps(ptr, smd))
