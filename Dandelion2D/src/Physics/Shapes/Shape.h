#pragma once

#include <Core\Math\Vector.h>
#include <Physics\Manifold.h>
#include <cmath>

struct Body;

enum ShapeType
{
	st_None = 0, // used to denote body has no collider
	st_Circle,
	st_AABB,
	st_Count
};

struct Shape
{
	vec2 position;
	ShapeType type;
	bool justCollided;
	bool isTrigger;
	Body* body;
	u32 __id;

	Shape(Body* parent, ShapeType _type, const vec2& pos = vec2(0.f, 0.f))
		: position(pos), type(_type), justCollided(false), isTrigger(false), body(parent) {}

	virtual bool CollidesWith(const Shape* rhs, Manifold& manifold) const = 0;

	static float ProjectOntoAxis(Manifold& m, const vec2& axis, const vec2& pos1, const vec2& pos2, const vec2& towards)
	{
		float distance = abs(vec2::Dot(axis, towards));
		float firstMax = 0;
		float projection = vec2::Dot(pos1, axis);
		firstMax = (projection > firstMax) ? projection : firstMax;

		float secondMax = 0;
		projection = -1.0f * vec2::Dot(pos2, axis);
		secondMax = (projection > secondMax) ? projection : secondMax;

		return distance - (firstMax + secondMax);
	}
};
