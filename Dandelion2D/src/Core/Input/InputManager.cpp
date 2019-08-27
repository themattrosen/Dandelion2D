#include "InputManager.h"
#include <Graphics\Graphics.h>
#include <GLFW\glfw3.h>

#define KEY_IS_VALID(key) (key > 0 && key <= 256)

static InputManager& IM = InputManager::Instance();

InputManager::InputManager() :
	m_keysCurr{ 0 },
	m_keysPrev{ 0 }
{
}

void InputManager::Init()
{
	m_windowHandle = (GLFWwindow*)Dandelion::GetWindow();
	glfwSetKeyCallback(m_windowHandle, InputManager::KeyCallback);
	glfwSetMouseButtonCallback(m_windowHandle, InputManager::MouseButtonCallback);
}

void InputManager::Update()
{
	for (int i = 0; i < NUM_KEYS; ++i)
	{
		m_keysPrev[i] = m_keysCurr[i];
	}

	for (int i = 0; i < MouseButtonCount; ++i)
	{
		m_mouseButtonPrev[i] = m_mouseButtonCurr[i];
	}

	double x, y;
	glfwGetCursorPos(m_windowHandle, &x, &y);

	m_mouseDragDelta = vec2((float)x - m_mousePosCache.x, (float)y - m_mousePosCache.y);

	m_mousePosCache = vec2((float)x, (float)y);
	Camera* camera = Dandelion::GetCamera();
	vec2 windowSize = Dandelion::GetWindowSize();
	m_mousePosCache.x -= windowSize.x / 2.f;
	m_mousePosCache.y -= windowSize.y / 2.f;
	m_mousePosCache.y *= -1.f;
	m_mousePosCache *= (1.f / camera->m_pos.z) * 0.5f;
}

void InputManager::Exit()
{
}

void InputManager::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (KEY_IS_VALID(key))
	{
		if (action == GLFW_RELEASE)
		{
			IM.m_keysCurr[key - 1] = false;
		}
		else if (action == GLFW_PRESS)
		{
			IM.m_keysCurr[key - 1] = true;
		}
	}
}

void InputManager::MouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_RELEASE)
		{
			IM.m_mouseButtonCurr[button] = false;
		}
		else if (action == GLFW_PRESS)
		{
			IM.m_mouseButtonCurr[button] = true;
		}
	}
}

bool InputManager::IsKeyPressed(Key key) const
{
	ASSERT(KEY_IS_VALID(key));
	return m_keysCurr[key - 1] == true;
}

bool InputManager::IsKeyTriggered(Key key) const
{
	ASSERT(KEY_IS_VALID(key));
	return (m_keysPrev[key - 1] == false && m_keysCurr[key - 1] == true);
}

bool InputManager::IsKeyReleased(Key key) const
{
	ASSERT(KEY_IS_VALID(key));
	return (m_keysPrev[key - 1] == true && m_keysCurr[key - 1] == false);
}

bool InputManager::IsMouseButtonPressed(MouseButton button) const
{
	ASSERT(button != MouseButtonCount);
	return m_mouseButtonCurr[button] == true;
}

bool InputManager::IsMouseButtonTriggered(MouseButton button) const
{
	ASSERT(button != MouseButtonCount);
	return (m_mouseButtonCurr[button] == true && m_mouseButtonPrev[button] == false);
}

bool InputManager::IsMouseButtonReleased(MouseButton button) const
{
	ASSERT(button != MouseButtonCount);
	return (m_mouseButtonCurr[button] == false && m_mouseButtonPrev[button] == true);
}

const vec2 & InputManager::GetMousePos() const
{
	return m_mousePosCache;
}

const vec2 & InputManager::GetMouseDragDelta() const
{
	return m_mouseDragDelta;
}


bool Dandelion::IsKeyPressed(Key key)
{
	return IM.IsKeyPressed(key);
}

bool Dandelion::IsKeyTriggered(Key key)
{
	return IM.IsKeyTriggered(key);
}

bool Dandelion::IsKeyReleased(Key key)
{
	return IM.IsKeyReleased(key);
}

bool Dandelion::IsMouseButtonPressed(MouseButton button)
{
	return IM.IsMouseButtonPressed(button);
}

bool Dandelion::IsMouseButtonTriggered(MouseButton button)
{
	return IM.IsMouseButtonTriggered(button);
}

bool Dandelion::IsMouseButtonReleased(MouseButton button)
{
	return IM.IsMouseButtonReleased(button);
}

const vec2 & Dandelion::GetMousePos()
{
	return IM.GetMousePos();
}

const vec2 & Dandelion::GetMouseDragDelta()
{
	return IM.GetMouseDragDelta();
}
