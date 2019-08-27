#pragma once

#include <Core\CoreCommon.h>
#include "Shape.h"

struct Circle : public Shape
{
	Circle(Body* body, f32 _radius = 1.f, const vec2& pos = vec2(0.f, 0.f))
		: Shape(body, st_Circle, pos), radius(_radius) {}

	f32 radius;

	bool CollidesWith(const Shape* rhs, Manifold& manifold) const override;
};
