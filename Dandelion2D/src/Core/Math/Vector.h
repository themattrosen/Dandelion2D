#pragma once

#include <Core/Definitions.h>
#include "SIMD.h"

DL_ALIGN(16) struct vec2
{
	union
	{
		struct
		{
			f32 x, y, __dummy1, __dummy2;
		};

		f32 m[4];

		SMD smd;
	};

	__forceinline vec2(float _x = 0.f, float _y = 0.f) : x(_x), y(_y), __dummy1(0.f), __dummy2(0.f) {}
	__forceinline vec2(const vec2& rhs) : smd(rhs.smd) {}
	__forceinline vec2& operator=(const vec2& rhs)
	{
		if (this != &rhs)
		{
			smd = rhs.smd;
		}
		return *this;
	}

	__forceinline vec2& operator+=(const vec2& rhs)
	{
		smd = SMD_ADD(smd, rhs.smd);
		return *this;
	}

	__forceinline vec2 operator+(const vec2& rhs) const
	{
		vec2 lhs(*this);
		lhs += rhs;
		return lhs;
	}

	__forceinline vec2& operator-=(const vec2& rhs)
	{
		smd = SMD_SUB(smd, rhs.smd);
		return *this;
	}

	__forceinline vec2 operator-(const vec2& rhs) const
	{
		vec2 lhs(*this);
		lhs -= rhs;
		return lhs;
	}

	__forceinline vec2& operator*=(const f32& rhs)
	{
		vec2 sv(rhs, rhs);
		smd = SMD_MUL(smd, sv.smd);
		return *this;
	}

	__forceinline vec2 operator*(const f32& rhs) const
	{
		vec2 lhs(*this);
		lhs *= rhs;
		return lhs;
	}

	__forceinline bool operator==(const vec2& rhs) const
	{
		return (x == rhs.x) && (y == rhs.y);
	}

	__forceinline bool operator!=(const vec2& rhs) const
	{
		return !(*this == rhs);
	}

	__forceinline static f32 Dot(const vec2& lhs, const vec2& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	__forceinline f32 Dot(const vec2& rhs) const
	{
		return Dot(*this, rhs);
	}

	__forceinline static f32 Cross(const vec2& lhs, const vec2& rhs)
	{
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}

	__forceinline static vec2 Cross(const vec2& lhs, f32 rhs)
	{
		return vec2(rhs * lhs.y, -rhs * lhs.x);
	}

	__forceinline static vec2 Cross(f32 lhs, const vec2& rhs)
	{
		return vec2(-lhs * rhs.y, lhs * rhs.x);
	}
};

inline vec2 operator*(const f32& lhs, const vec2& rhs)
{
	return rhs * lhs;
}
