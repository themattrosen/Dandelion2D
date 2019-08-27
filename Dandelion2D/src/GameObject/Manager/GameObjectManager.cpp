#include "GameObjectManager.h"

static GameObjectManager& GOM = GameObjectManager::Instance();

GameObject * Dandelion::CreateGameObject(const string & name, const vec2 & pos)
{
	return GOM.CreateGameObject(name, pos);
}

GameObject * Dandelion::DuplicateGameObject(const GameObject * other)
{
	return GOM.DuplicateGameObject(other);
}

void Dandelion::DeleteGameObject(GameObject * obj)
{
	return GOM.DeleteGameObject(obj);
}

void Dandelion::DeleteGameObject(const string & name)
{
	return DeleteGameObject(GetGameObject(name));
}

GameObject * Dandelion::GetGameObject(const string & name)
{
	return GOM.GetGameObject(name);
}

void Dandelion::DeleteAllGameObjects()
{
	GOM.DeleteAllGameObjects();
}

u32 Dandelion::GetNumGameObjects()
{
	return GOM.GetNumGameObjects();
}


