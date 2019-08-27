#pragma once

#include <Core\Singleton.h>
#include <Graphics\Camera\Camera.h>

struct GLFWwindow;
class Shader;

class GraphicsEngine : public Singleton<GraphicsEngine>
{
public:
	void Init();
	void Update(float dt);
	void Draw();
	void Exit();

	const Shader* GetShader() const { return m_shader; }
	Camera* GetCamera() { return &m_camera; }

	const vec2& GetWindowSize() const { return m_windowSize; }
	void SetWindowSize(const vec2& s);

	void* GetWindow() { return m_window; }

	const vec4& GetBackgroundColor() const { return m_clearColor; }
	void SetBackgroundColor(const vec4& color) { m_clearColor = color; }

private:
	friend class Singleton<GraphicsEngine>;

	GLFWwindow* m_window;
	Shader* m_shader;
	Camera m_camera;

	u32 m_vbo;
	u32 m_vao;
	u32 m_ebo;

	vec2 m_windowSize{ 800, 600 };
	vec4 m_clearColor{ 0.2f, 0.3f, 0.3f, 1.f };
};

namespace Dandelion
{
	DANDELION_API Camera* GetCamera();
	DANDELION_API void* GetWindow();
	DANDELION_API const vec2& GetWindowSize();
	DANDELION_API const vec4& GetBackgroundColor();
	DANDELION_API void SetBackgroundColor(const vec4& color);
}
