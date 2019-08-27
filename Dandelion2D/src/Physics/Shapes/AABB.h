#pragma once

#include "Shape.h"
#include <Core\Math\MathCommon.h>

struct AABB : public Shape
{
	AABB(Body* body, f32 _w = 0.5f, f32 _h = 0.5f, const vec2& pos = vec2(0.f, 0.f))
		: Shape(body, st_AABB, pos), width(_w), height(_h) {}

	f32 width, height;

	bool CollidesWith(const Shape* rhs, Manifold& manifold) const override;

};
