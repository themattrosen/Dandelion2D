#pragma once

#include "Level.h"

class GameOver : public Level
{
public:
	void InitArchetypes() override;
	void Create() override;
	void Update(float dt) override;

	float timer;
	unsigned gravIndex;
};