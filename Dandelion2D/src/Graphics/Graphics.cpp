#include <Core\Definitions.h>
#include <Application\Application.h>
#include <Graphics\Mesh\Mesh.h>
#include "Graphics.h"
#include "Shader\Shader.h"
#include "glad\glad.h"
#include <GLFW\glfw3.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static GraphicsEngine& GE = GraphicsEngine::Instance();

void GraphicsEngine::Init()
{
	// initialize glfw
	glfwInit();

	// initialize window information before window creation
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window

	// use this code for fullscreen
	//GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	//const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	//glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	//glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	//glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	//glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	//m_window = glfwCreateWindow(mode->width, mode->height, "Dandelion2D", monitor, NULL);

	m_window = glfwCreateWindow(800, 600, "Dandelion2D", NULL, NULL);
	ASSERT(m_window && "Failed to create window!");

	// set the current window
	glfwMakeContextCurrent(m_window);

	// set the callback for the window being resized
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	// init glad
	ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) && 
		"Failed to initialize GLAD");

	glEnable(GL_DEPTH_TEST);

	m_shader = new Shader("basic");
	m_shader->Use();

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadMeshVerts), quadMeshVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadMeshIndices), quadMeshIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	m_camera.m_pos = vec3(0.f, 0.f, 20.f);
	m_camera.m_target = vec3();
	m_camera.m_up = vec3(0.f, 1.f, 0.f);

	m_shader->SetMatrix4("view", m_camera.ViewMatrix());
	f32 fovDeg = RAD_TO_DEG(std::atan2f(m_windowSize.x / 2.f, m_camera.m_pos.z) / 2.f);
	
	mat4 perspMat = PERSPECTIVE_MATRIX(fovDeg, m_windowSize.x, m_windowSize.y, 0.1f, 100.f);
	m_shader->SetMatrix4("perspective", perspMat);
}

void GraphicsEngine::Update(float dt)
{
	glfwPollEvents();

	if (glfwWindowShouldClose(m_window))
	{
		Application::StopRunning();
	}
	//processInput(mWindow) {
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, true);
	//}

	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_camera.Update(dt);

	m_shader->Use();
	m_shader->SetMatrix4("view", m_camera.ViewMatrix());

	glBindVertexArray(m_vao);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GraphicsEngine::Draw()
{
	glfwSwapBuffers(m_window);
}

void GraphicsEngine::Exit()
{
	delete m_shader;
	m_shader = 0;

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);

	// close glfw windowing
	glfwTerminate();
}

void GraphicsEngine::SetWindowSize(const vec2 & s)
{
	m_windowSize = s;
	f32 fovDeg = RAD_TO_DEG(std::atan2f(m_windowSize.x / 2.f, m_camera.m_pos.z) / 2.f);

	mat4 perspMat = PERSPECTIVE_MATRIX(fovDeg, m_windowSize.x, m_windowSize.y, 0.1f, 100.f);
	m_shader->SetMatrix4("perspective", perspMat);
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
	GE.SetWindowSize(vec2((float)width, (float)height));
}

Camera * Dandelion::GetCamera()
{
	return GE.GetCamera();
}

void * Dandelion::GetWindow()
{
	return GE.GetWindow();
}

const vec2& Dandelion::GetWindowSize()
{
	return GE.GetWindowSize();
}

const vec4 & Dandelion::GetBackgroundColor()
{
	return GE.GetBackgroundColor();
}

void Dandelion::SetBackgroundColor(const vec4 & color)
{
	GE.SetBackgroundColor(color);
}
