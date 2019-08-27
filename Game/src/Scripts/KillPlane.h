#pragma once
#include <Dandelion.h>

const Script KillPlane = BEGIN_SCRIPT(KillPlane)

SCRIPT_VALUES()
{
}

SCRIPT_REFS()
{
}

SCRIPT_UPDATE(float dt, GameObject* parent)
{
}

SCRIPT_ON_COLLISION_ENTER(GameObject* parent, GameObject* other)
{
	if (other->HasTag(t_DynamicObject))
	{
		if (other->HasTag(t_Player))
		{
			Dandelion::GetCamera()->SetCameraShake(3.f, 7);
			Dandelion::PlaySound("on_death");
		}
		Dandelion::DeleteGameObject(other);
	}
}

SCRIPT_ON_COLLISION_EXIT(GameObject* parent, GameObject* other)
{
}
END_SCRIPT;
