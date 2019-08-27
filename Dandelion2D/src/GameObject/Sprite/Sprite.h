#pragma once

#include <Core\Math\MathCommon.h>
#include <Core\Definitions.h>

class Transform;
class Shader;

class Sprite
{
public:
	Sprite(const Transform* t = 0, const Shader* s = 0, const vec4& color = vec4(0.f, 0.f, 0.f, 1.f))
		: m_color(color), m_transform(t), m_shader(s) {}

	void Draw();

	void SetColor(const vec4& color) { m_color = color; }
	const vec4& GetColor() const { return m_color; }

	void SetTransform(const Transform* transform) { m_transform = transform; }
	const Transform* GetTransform() const { return m_transform; }

	void SetShader(const Shader* shader) { m_shader = shader; }
	const Shader* GetShader() const { return m_shader; }

private:
	vec4 m_color;
	const Transform* m_transform;
	const Shader* m_shader;
};
