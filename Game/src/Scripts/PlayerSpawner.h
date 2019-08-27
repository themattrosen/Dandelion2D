#pragma once
#include <Dandelion.h>
#include "../Level/Level.h"
#include "PlayerController.h"

static const f32 RESPAWN_TIME = 1.5f;
static const u32 STARTING_LIVES = 5;

static void SpawnPlayer(const vec2& position)
{
	GameObject* player = Dandelion::CreateGameObject("player", position);
	player->SetScale(vec2(0.5f, 0.5f));
	player->SetColor(vec4(0.6f, 0.6f, 0.6f, 0.6f));
	player->AttachBody(st_AABB, pmt_Pillow);
	player->AddTag(t_Player);
	player->AddTag(t_DynamicObject);
	player->AttachScript(PlayerController);
}

const Script PlayerSpawner = BEGIN_SCRIPT(PlayerSpawner)

SCRIPT_VALUES()
{
	SCRIPT_VALUE(timer, 0.f),
	SCRIPT_VALUE(numLives, (f32)STARTING_LIVES)
}

SCRIPT_REFS()
{
	SCRIPT_REF(level, nullptr)
}

SCRIPT_UPDATE(float dt, GameObject* parent)
{
	f32 timer = VALUE(timer, PlayerSpawner);
	if (timer > 0.f)
	{
		timer -= dt;
		if (timer <= 0.f)
		{
			SpawnPlayer(parent->GetPosition());
		}
		VALUE(timer, PlayerSpawner) = timer;
	}
	else
	{
		GameObject* player = Dandelion::GetGameObject("player");
		if (!player)
		{
			VALUE(timer, PlayerSpawner) = RESPAWN_TIME;
			if (--VALUE(numLives, PlayerSpawner) == -1)
			{
				Level* level = (Level*)REF(level, PlayerSpawner);
				level->SetLevelState(ls_GoToNext);
				level->levelToGoTo = "GameOver";
			}
		}
	}
}

SCRIPT_ON_COLLISION_ENTER(GameObject* parent, GameObject* other)
{
}

SCRIPT_ON_COLLISION_EXIT(GameObject* parent, GameObject* other)
{
}
END_SCRIPT;