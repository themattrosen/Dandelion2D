#pragma once
#include <Dandelion.h>

const Script BulletScript = BEGIN_SCRIPT(BulletScript)

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
	if (other->HasTag(t_Platform))
	{
		Dandelion::SetGravity(GRAVITY_MODE_POINT, other->GetPosition());
		Dandelion::GetCamera()->SetCameraShake(0.2f, 3);
		Dandelion::DeleteGameObject(parent);
	}
}

SCRIPT_ON_COLLISION_EXIT(GameObject* parent, GameObject* other)
{
}
END_SCRIPT;