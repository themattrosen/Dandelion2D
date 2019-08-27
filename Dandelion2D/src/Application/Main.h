#pragma once

#include <Application/Application.h>

extern Application* CreateApplication();
extern void DestroyApplication(Application* app);


int main(int argc, char** argv)
{
	APP = CreateApplication();
	APP->Run();
	DestroyApplication(APP);

	return 0;
}
