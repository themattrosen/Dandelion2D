#include "Sprite.h"
#include <GameObject\Transform\Transform.h>
#include <Graphics\Shader\Shader.h>
#include "glad\glad.h"

void Sprite::Draw()
{
	mat4 transform = m_transform->GetTransformationMatrix();
	m_shader->SetMatrix4("transform", transform);
	m_shader->SetVector4("ourColor", m_color);
	m_shader->Use();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
