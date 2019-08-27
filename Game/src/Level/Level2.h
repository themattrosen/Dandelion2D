#pragma once

#include "Level.h"

class Level2 : public Level
{
public:
	void InitArchetypes() override;
	void Create() override;
	void Update(float dt) override;

	float gravity = 1.f;
	float timer;
};