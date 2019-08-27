#pragma once

#include <Core/Definitions.h>

class DANDELION_API Application
{
public:
	Application() {}
	virtual ~Application() {}

	virtual void Run();
	static void StopRunning() { s_isRunning = false; }
	static bool IsRunning() { return s_isRunning; }

protected:
	virtual void Init() {}
	virtual void Exit() {}
	virtual void Update(float dt) { (void)dt; }
	virtual void LateUpdate(float dt) { (void)dt; }

private:
	void CoreInit();
	void CoreUpdate(float dt);
	void CoreLateUpdate(float dt);
	void CoreExit();

	static bool s_isRunning;
};

Application* CreateApplication();
void DestroyApplication(Application* app);
