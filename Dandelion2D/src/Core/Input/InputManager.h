#pragma once

#include <Core\Singleton.h>
#include <Core\Definitions.h>
#include <Core\Math\MathCommon.h>

struct GLFWwindow;

enum Key
{
	KeyQ = 81,
	KeyW = 87,
	KeyE = 69,
	KeyR = 82,
	KeyT = 84,
	KeyY = 89,
	KeyU = 85,
	KeyI = 73,
	KeyO = 79,
	KeyP = 80,
	KeyA = 65,
	KeyS = 83,
	KeyD = 68,
	KeyF = 70,
	KeyG = 71,
	KeyH = 72,
	KeyJ = 74,
	KeyK = 75,
	KeyL = 76,
	KeyZ = 90,
	KeyX = 88,
	KeyC = 67,
	KeyV = 86,
	KeyB = 66,
	KeyN = 78,
	KeyM = 77,
	Key1 = 49,
	Key2 = 50,
	Key3 = 51,
	Key4 = 52,
	Key5 = 53,
	Key6 = 54,
	Key7 = 55,
	Key8 = 56,
	Key9 = 57,
	Key0 = 58,
	KeySpace = 32,
	KeyEscape = 256,
};

enum MouseButton
{
	MouseLeft = 0,
	MouseRight = 1,

	MouseButtonCount
};

#define NUM_KEYS 256

class InputManager : public Singleton<InputManager>
{
public:
	InputManager();
	
	void Init();
	void Update();
	void Exit();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	bool IsKeyPressed(Key key) const;
	bool IsKeyTriggered(Key key) const;
	bool IsKeyReleased(Key key) const;

	bool IsMouseButtonPressed(MouseButton button) const;
	bool IsMouseButtonTriggered(MouseButton button) const;
	bool IsMouseButtonReleased(MouseButton button) const;

	const vec2& GetMousePos() const;
	const vec2& GetMouseDragDelta() const;

private:
	GLFWwindow* m_windowHandle;
	
	bool m_keysCurr[NUM_KEYS];
	bool m_keysPrev[NUM_KEYS];

	bool m_mouseButtonCurr[MouseButtonCount];
	bool m_mouseButtonPrev[MouseButtonCount];

	vec2 m_mousePosCache;
	vec2 m_mouseDragDelta;
};

namespace Dandelion
{
	DANDELION_API bool IsKeyPressed(Key key);
	DANDELION_API bool IsKeyTriggered(Key key);
	DANDELION_API bool IsKeyReleased(Key key);

	DANDELION_API bool IsMouseButtonPressed(MouseButton button);
	DANDELION_API bool IsMouseButtonTriggered(MouseButton button);
	DANDELION_API bool IsMouseButtonReleased(MouseButton button);

	DANDELION_API const vec2& GetMousePos();
	DANDELION_API const vec2& GetMouseDragDelta();
}
