#include "GameObjectFactory.h"
#include <Core\Math\MathCommon.h>
#include <GameObject\Manager\GameObjectManager.h>
#include <iostream>

static GameObjectFactory& GOF = GameObjectFactory::Instance();

GameObject * GameObjectFactory::CreateArchetype(const string & name)
{
	if (m_archetypeMap.find(name) == m_archetypeMap.end())
	{
		GameObject* archetype = m_objectPool.Construct(name, vec2(0.f, 0.f));
		m_archetypeMap[name] = archetype;
		archetype->m_isArchetype = true;
		return archetype;
	}
	else
	{
		std::cout << "ERROR: Tried to insert duplicate archetype!" << std::endl;
		return nullptr;
	}
}

void GameObjectFactory::DestroyArchetype(GameObject * archetype)
{
	if (m_archetypeMap.find(archetype->GetName()) != m_archetypeMap.end())
	{
		m_archetypeMap.erase(archetype->GetName());
		m_objectPool.Free(archetype);
	}
	else
	{
		std::cout << "ERROR: Tried to destroy a non-existent archetype!" << std::endl;
	}
}

void GameObjectFactory::DestroyAllArchetypes()
{
	m_objectPool.Clear();
	m_archetypeMap.clear();
}

GameObject * GameObjectFactory::Instantiate(const string & archetypeName)
{
	auto it = m_archetypeMap.find(archetypeName);
	if (it != m_archetypeMap.end())
	{
		GameObject* archetype = it->second;
		return Dandelion::DuplicateGameObject(archetype);
	}
	else
	{
		std::cout << "EROR: Tried to instantiate a non-existent archetype!" << std::endl;
		return nullptr;
	}
}

GameObjectFactory::GameObjectFactory() :
	m_objectPool(MAX_GAME_OBJECT_ARCHETYPES, false),
	m_archetypeMap()
{
}

GameObject * Dandelion::CreateNewArchetype(const string & name)
{
	return GOF.CreateArchetype(name);
}

void Dandelion::DestroyArchetype(GameObject * archetype)
{
	GOF.DestroyArchetype(archetype);
}

void Dandelion::DestroyAllArchetypes()
{
	GOF.DestroyAllArchetypes();
}

GameObject * Dandelion::Instantiate(const string & archetypeName)
{
	return GOF.Instantiate(archetypeName);
}
