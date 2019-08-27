#pragma once

#include "Level.h"

class Level3 : public Level
{
public:
	void InitArchetypes() override;
	void Create() override;
	void Update(float dt) override;

};