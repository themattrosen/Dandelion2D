#pragma once

#include <Core\Math\MathCommon.h>

class Transform
{
public:
	Transform(const vec2& position = vec2(0.f, 0.f), f32 rotation = 0.f, const vec2& scale = vec2(1.f, 1.f))
		: m_position(position), m_scale(scale), m_rotation(rotation) {}

	const vec2& GetPosition() const { return m_position; }
	void SetPosition(const vec2& val) { m_position = val; }

	const vec2& GetScale() const { return m_scale; }
	void SetScale(const vec2& val) { m_scale = val; }

	// stored in degrees
	const f32& GetRotation() const { return m_rotation; }
	void SetRotation(f32 val) 
	{
		if (val > 360.f) val -= 360.f;
		else if (val < 0.f) val += 360.f;
		m_rotation = val; 
	}

	mat4 GetTransformationMatrix() const
	{
		mat4 trans = mat4(1.f);
		trans = glm::translate(trans, vec3(m_position.x, m_position.y, 0.f));
		trans = glm::rotate(trans, DEG_TO_RAD(m_rotation), vec3(0.f, 0.f, 1.f));
		trans = glm::scale(trans, vec3(m_scale.x, m_scale.y, 0.f));

		return trans;
	}

private:
	vec2 m_position;
	vec2 m_scale;
	f32 m_rotation;
};
