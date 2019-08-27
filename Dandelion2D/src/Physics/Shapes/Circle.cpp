#include "Circle.h"
#include "AABB.h"

bool Circle::CollidesWith(const Shape* rhs, Manifold& manifold) const
{
	switch (rhs->type)
	{
		case st_None:
		case st_Count:
			return false;
		case st_Circle:
		{
			// circle circle collision
			const Circle* other = dynamic_cast<const Circle *>(rhs);
			vec2 pos1 = position;
			vec2 pos2 = other->position;
			vec2 n = pos2 - pos1;

			f32 r1 = radius;
			f32 r2 = other->radius;
			f32 r = r1 + r2;
			r *= r;

			if (VEC2_SQ_LENGTH(n) > r)
				return false;

			f32 d = VEC2_LENGTH(n);

			if (d != 0.f)
			{
				manifold.penetration = r - d;
				NORMALIZE_VEC2(n);
				manifold.normal = n;
			}
			// circles on same position?
			else
			{
				manifold.penetration = r1;
				manifold.normal = vec2(1.f, 0.f);
			}

			return true;
		}
		case st_AABB:
		{
			// circle aabb collision
			const AABB* box = dynamic_cast<const AABB *>(rhs);
			const Circle* circle = this;

			vec2 local = vec2(box->width / 2.f, box->height / 2.f);
			vec2 min1 = position - local;
			vec2 max1 = position + local;

			vec2 n = circle->position - box->position;
			vec2 closest = n;

			f32 x_extent = (max1.x - min1.x) / 2.f;
			f32 y_extent = (max1.y - min1.y) / 2.f;

			closest.x = CLAMP(-x_extent, x_extent, closest.x);
			closest.y = CLAMP(-y_extent, y_extent, closest.y);

			bool inside = false;

			if (n == closest)
			{
				inside = true;
				if (abs(n.x) > abs(n.y))
				{
					if (closest.x > 0.f)
						closest.x = x_extent;
					else
						closest.x = -x_extent;
				}
				else
				{
					if (closest.y > 0.f)
						closest.y = y_extent;
					else
						closest.y = -y_extent;
				}
			}

			vec2 normal = n - closest;
			f32 d = VEC2_SQ_LENGTH(normal);
			f32 r = circle->radius;

			if (d > r * r && !inside)
				return false;

			d = sqrt(d);

			if (inside)
			{
				manifold.normal = n * -1.f;
				manifold.penetration = r - d;
			}
			else
			{
				manifold.normal = n;
				manifold.penetration = r - d;
			}

			return true;
		}
	}

	return false;
}
