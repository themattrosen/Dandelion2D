#pragma once
#include <Core\Math\MathCommon.h>

class Camera
{
public:
	mat4 ViewMatrix() const
	{
		vec2 pos = vec2(m_pos.x, m_pos.y) + shakeJigglePos;
		vec2 target = vec2(m_target.x, m_target.y) + shakeJigglePos;
		return glm::lookAt(vec3(pos.x, pos.y, m_pos.z), vec3(target.x, target.y, m_target.z), m_up);
	}

	void SetCameraShake(f32 time, s32 factor)
	{
		shakeTimeRemaining = time;
		shakeFactor = factor;
	}

	void Update(f32 dt)
	{
		if (shakeTimeRemaining > 0.f)
		{
			s32 sign = 1;
			s32 s = RAND_BETWEEN(0, 100);
			if (s > 50) sign = -1;
			shakeJigglePos.x = (f32)sign * dt * (f32)RAND_BETWEEN(0, shakeFactor);
			shakeJigglePos.y = (f32)sign * dt * (f32)RAND_BETWEEN(0, shakeFactor);
			shakeTimeRemaining -= dt;
		}
	}

	vec3 m_pos;
	vec3 m_target;
	vec3 m_up;

	s32 shakeFactor;
	f32 shakeTimeRemaining;
	vec2 shakeJigglePos;
};
