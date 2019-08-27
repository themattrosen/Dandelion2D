#pragma once
#include <Dandelion.h>

const Script Spawner = BEGIN_SCRIPT(Spawner)

SCRIPT_VALUES()
{
	SCRIPT_VALUE(timeToSpawn, 1.f),
	SCRIPT_VALUE(spawnTime, -1.f)
}

SCRIPT_REFS()
{
	SCRIPT_REF(spawnedObject, nullptr)
}

SCRIPT_UPDATE(float dt, GameObject* parent)
{
	f32 spawnTime = VALUE(spawnTime, Spawner);
	if (spawnTime == -1.f)
	{
		VALUE(spawnTime, Spawner) = (f32)(RAND_BETWEEN(20, 100)) / 20.f;
		VALUE(timeToSpawn, Spawner) = VALUE(spawnTime, Spawner);
	}

	VALUE(timeToSpawn, Spawner) -= dt;
	if (VALUE(timeToSpawn, Spawner) < 0.f)
	{
		// try to spawn a point
		GameObject* spawned = (GameObject*)REF(spawnedObject, Spawner);
		if (!spawned)
		{
			spawned = Dandelion::Instantiate("collectable");
			spawned->GetScript("Collectable").refVariables["spawner"] = (void*)parent->GetName();
			spawned->SetPosition(parent->GetPosition());
			REF(spawnedObject, Spawner) = spawned;
			VALUE(timeToSpawn, Spawner) = VALUE(spawnTime, Spawner);
		}
	}
}

SCRIPT_ON_COLLISION_ENTER(GameObject* parent, GameObject* other)
{
}

SCRIPT_ON_COLLISION_EXIT(GameObject* parent, GameObject* other)
{
	REF(spawnedObject, Spawner) = nullptr;
}
END_SCRIPT;