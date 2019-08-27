#pragma once

#include <Core\Singleton.h>
#include <Core\Definitions.h>
#include <Core\Memory\Allocator.h>
#include <GameObject\GameObject.h>

#include <unordered_map>


class GameObjectFactory : public Singleton<GameObjectFactory>
{
public:
	GameObject* CreateArchetype(const string& name);
	void DestroyArchetype(GameObject* archetype);
	void DestroyAllArchetypes();

	GameObject* Instantiate(const string& archetypeName);

	GameObjectFactory();

private:
	PoolAllocator<GameObject> m_objectPool;
	std::unordered_map<string, GameObject *> m_archetypeMap;
};

namespace Dandelion
{
	DANDELION_API GameObject* CreateNewArchetype(const string& name);
	DANDELION_API void DestroyArchetype(GameObject* archetype);
	DANDELION_API void DestroyAllArchetypes();
	DANDELION_API GameObject* Instantiate(const string& archetypeName);
}
