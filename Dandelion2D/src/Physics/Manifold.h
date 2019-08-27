#pragma once
#include <Core\Definitions.h>
#include <Core\Math\Vector.h>

struct Body;

struct Manifold
{
	Body* A;
	Body* B;
	f32 penetration;
	vec2 normal;
	vec2 contactPoint;

	bool operator<(const Manifold& rhs) const;
};