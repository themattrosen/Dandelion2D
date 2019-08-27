#pragma once
#include <Dandelion.h>
#include "../Level/Level.h"
#include <iostream>

static const f32 ALPHA_MAX = 220.f / 255.f;
static const f32 ALPHA_MIN = 100.f / 255.f;

const Script Collectable = BEGIN_SCRIPT(Collectable)

SCRIPT_VALUES()
{
	SCRIPT_VALUE(rotationSpeed, 10.f),
	SCRIPT_VALUE(alphaTarget, ALPHA_MIN)
}

SCRIPT_REFS()
{
	SCRIPT_REF(spawner, nullptr)
}

SCRIPT_UPDATE(float dt, GameObject* parent)
{
	parent->SetRotation(parent->GetRotation() + VALUE(rotationSpeed, Collectable) * dt);
	f32 alphaTarget = VALUE(alphaTarget, Collectable);
	vec4 currentColor = parent->GetColor();
	f32 newAlpha = LERP_FLOAT(currentColor.r, alphaTarget, dt);
	currentColor.r = newAlpha;

	parent->SetColor(currentColor);

	if (alphaTarget == ALPHA_MAX && alphaTarget - newAlpha < 0.01f)
	{
		VALUE(alphaTarget, Collectable) = ALPHA_MIN;
	}
	else if (alphaTarget == ALPHA_MIN && newAlpha - alphaTarget < 0.01f)
	{
		VALUE(alphaTarget, Collectable) = ALPHA_MAX;
	}
}

SCRIPT_ON_COLLISION_ENTER(GameObject* parent, GameObject* other)
{
	if (other->HasTag(t_Player))
	{
		Level* level = (Level*)REF(spawner, Collectable);
		level->OnCollect();
		Dandelion::PlaySound("on_collect");
		Dandelion::DeleteGameObject(parent);
	}
}

SCRIPT_ON_COLLISION_EXIT(GameObject* parent, GameObject* other)
{
}
END_SCRIPT;