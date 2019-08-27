#pragma once

#include <Core\Memory\Allocator.h>
#include <Core\Singleton.h>
#include <GameObject\GameObject.h>

#include <unordered_map>
#include <iostream>
#include <string>

class GameObjectManager : public Singleton<GameObjectManager>
{
public:

	void Update(float dt)
	{
		GameObject* looper = m_activeList;
		while (looper)
		{
			looper->Update(dt);
			looper = looper->m_next;
		}
	}

	void Draw()
	{
		GameObject* looper = m_activeList;
		while (looper)
		{
			looper->Draw();
			looper = looper->m_next;
		}

		RunDeleteStep();
	}

	void RunDeleteStep()
	{
		while (!m_toDelete.empty())
		{
			GameObject* obj = m_toDelete.back();

			// remove from linked list
			// case has prev (not front)
			if (obj->m_prev)
			{
				obj->m_prev->m_next = obj->m_next;
				if (obj->m_next) obj->m_next->m_prev = obj->m_prev;
			}
			// case is front
			else
			{
				m_activeList = obj->m_next;
				if (m_activeList) m_activeList->m_prev = nullptr;
			}


			--m_numObjects;
			m_nameMap.erase(obj->GetName());
			m_allocator.Free(obj);
			m_toDelete.pop_back();
		}
	}

	GameObject* GetGameObject(const string& name)
	{
		auto iter = m_nameMap.find(name);
		if (iter == m_nameMap.end())
			return nullptr;
		else
			return iter->second;
	}

	GameObject* CreateGameObject(const string& name, const vec2& pos = vec2())
	{
		GameObject* obj = m_allocator.Construct(name, pos);
		++m_numObjects;

		// push front linked list
		obj->m_next = m_activeList;
		if(m_activeList) m_activeList->m_prev = obj;
		m_activeList = obj;

		if (m_nameMap.find(name) == m_nameMap.end())
		{
			m_nameMap.insert_or_assign(name, obj);
			auto iter = m_nameMap.find(name);
			obj->m_name = iter->first.c_str();
		}

		obj->m_isArchetype = false;

		return obj;
	}

	GameObject* DuplicateGameObject(const GameObject* other)
	{
		std::string newName(other->GetName());
		newName.append("-Copy");
		GameObject* dup = CreateGameObject(newName.c_str(), other->GetPosition());
		dup->SetRotation(other->GetRotation());
		dup->SetScale(other->GetScale());
		dup->SetColor(other->GetColor());
		dup->m_tag = other->m_tag;

		Body* otherBod = other->m_body;
		if (otherBod)
		{
			dup->AttachBody(otherBod->shape->type,
				otherBod->body->type,
				otherBod->body->autoCalcMass);
			Body* dupBod = dup->m_body;
			dupBod->body->isStatic = otherBod->body->isStatic;
			dupBod->body->canRotate = otherBod->body->canRotate;
			dupBod->body->gravityScale = otherBod->body->gravityScale;
			dupBod->body->staticFriction = otherBod->body->staticFriction;
			dupBod->body->dynamicFriction = otherBod->body->dynamicFriction;

			if (!otherBod->body->autoCalcMass)
			{
				dupBod->body->mass = otherBod->body->mass;
				dupBod->body->inertia = otherBod->body->inertia;
			}
		}

		dup->m_scripts = other->m_scripts;

		return dup;
	}

	void DeleteGameObject(GameObject* obj)
	{
		ASSERT(obj);
		m_toDelete.push_back(obj);

	}

	void DeleteAllGameObjects()
	{
		GameObject* looper = m_activeList;
		while (looper)
		{
			DeleteGameObject(looper);
			looper = looper->m_next;
		}

		RunDeleteStep();
		m_nameMap.clear();
		m_toDelete.clear();
		m_activeList = nullptr;
		m_numObjects = 0;
		m_allocator.Clear();
	}

	u32 GetNumGameObjects() const
	{
		return m_numObjects;
	}

private:
	PoolAllocator<GameObject> m_allocator;
	GameObject* m_activeList;
	u32 m_numObjects;

	std::unordered_map<std::string, GameObject*> m_nameMap;
	std::vector<GameObject*> m_toDelete;

	static const bool ALLOW_EXTRA_ALLOCATIONS = false;

	friend class Singleton<GameObjectManager>;
	GameObjectManager() :
		m_allocator(MAX_GAME_OBJECTS, ALLOW_EXTRA_ALLOCATIONS),
		m_activeList(nullptr),
		m_numObjects(0),
		m_nameMap(),
		m_toDelete()
	{
		m_toDelete.reserve(10);
	}
};

namespace Dandelion
{
	DANDELION_API GameObject* CreateGameObject(const string& name, const vec2& pos = vec2());
	DANDELION_API GameObject* DuplicateGameObject(const GameObject* other);
	DANDELION_API void DeleteGameObject(GameObject* obj);
	DANDELION_API void DeleteGameObject(const string& name);
	DANDELION_API GameObject* GetGameObject(const string& name);
	DANDELION_API void DeleteAllGameObjects();
	DANDELION_API u32 GetNumGameObjects();
}
