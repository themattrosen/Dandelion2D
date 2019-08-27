#include "AABB.h"
#include "Circle.h"

bool AABB::CollidesWith(const Shape* rhs, Manifold& manifold) const
{
	switch (rhs->type)
	{
		case st_None:
		case st_Count:
			return false;
		case st_Circle:
		{
			// circle aabb collision
			const AABB* box = this;
			const Circle* circle = dynamic_cast<const Circle *>(rhs);

			vec2 local = vec2(width / 2.f, height / 2.f);
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
		case st_AABB:
		{
			// AABB AABB collision
			const AABB* other = dynamic_cast<const AABB *>(rhs);
			vec2 local = vec2(width / 2.f, height / 2.f);
			vec2 min1 = position - local;
			vec2 max1 = position + local;
			local = vec2(other->width / 2.f, other->height / 2.f);
			vec2 min2 = other->position - local;
			vec2 max2 = other->position + local;

			vec2 pos1 = this->position;
			vec2 pos2 = other->position;
			vec2 n = pos1 - pos2;
			f32 a_extent = (max1.x - min1.x) / 2.f;
			f32 b_extent = (max2.x - min2.x) / 2.f;

			f32 x_overlap = a_extent + b_extent - abs(n.x);

			// sat test on x axis
			if (x_overlap > 0)
			{
				// calculate half extents along x axis for each object
				f32 a_extent = (max1.y - min1.y) / 2.f;
				f32 b_extent = (max2.y - min2.y) / 2.f;
				f32 y_overlap = a_extent + b_extent - abs(n.y);

				// sat test on y axis
				if (y_overlap > 0)
				{
					if (x_overlap < y_overlap)
					{
						if (n.x < 0)
							manifold.normal = vec2(-1.f, 0.f);
						else
							manifold.normal = vec2(1.f, 0.f);
						manifold.penetration = x_overlap;
						return true;
					}
					else
					{
						if (n.y < 0)
							manifold.normal = vec2(0.f, -1.f);
						else
							manifold.normal = vec2(0.f, 1.f);
						manifold.penetration = y_overlap;
						return true;
					}
				}
			}

		}
	}

	return false;
}
