#include "Application.h"
#include <GameObject\Manager\GameObjectManager.h>
#include <Physics\World\PhysicsWorld.h>
#include <Graphics\Graphics.h>
#include <Audio\AudioEngine\AudioEngine.h>

bool Application::s_isRunning = true;

void Application::Run()
{
	CoreInit();
	Init();

	float dt = 1.f / 60.f;

	while (IsRunning())
	{
		CoreUpdate(dt);

		Update(dt);

		CoreLateUpdate(dt);

		LateUpdate(dt);
	}

	Exit();
	CoreExit();
}

void Application::CoreInit()
{
	GraphicsEngine::Instance().Init();
	PhysicsWorld::Instance().Init();
	AudioEngine::Instance().Init();
	InputManager::Instance().Init();
	GameObjectManager::Instance();
}

void Application::CoreUpdate(float dt)
{
	GraphicsEngine::Instance().Update(dt);
	PhysicsWorld::Instance().Update(dt);
	AudioEngine::Instance().Update();
	GameObjectManager::Instance().Update(dt);
}

void Application::CoreLateUpdate(float dt)
{
	InputManager::Instance().Update();
	GameObjectManager::Instance().Draw();
	GraphicsEngine::Instance().Draw();
}

void Application::CoreExit()
{
	InputManager::Instance().Exit();
	AudioEngine::Instance().Exit();
	PhysicsWorld::Instance().Exit();
	GraphicsEngine::Instance().Exit();
}

