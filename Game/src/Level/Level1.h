#pragma once

#include "Level.h"

class Level1 : public Level
{
public:
	void InitArchetypes();
	void Create();
	void Update(float dt);

	
	//void LateUpdate(float dt) {}
	//void Exit() {}
};