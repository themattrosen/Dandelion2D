#pragma once
#include "Dandelion.h"

static vec4 YELLOW{ 1.f, 0.8f, 0.6f, 1.f };
static vec4 RED{ 1.f, 0.f, 0.f, 1.f };

const Script LavaScript = BEGIN_SCRIPT(LavaScript)

SCRIPT_VALUES()
{
	SCRIPT_VALUE(colorLerpFactor, 1.f)
}

SCRIPT_REFS()
{
	SCRIPT_REF(targetColor, nullptr)
}

SCRIPT_UPDATE(f32 dt, GameObject* parent)
{
	vec4* color = (vec4*)REF(targetColor, LavaScript);

	if (!color)
	{
		REF(targetColor, LavaScript) = &YELLOW;
		color = (vec4*)REF(targetColor, LavaScript);
		VALUE(colorLerpFactor, LavaScript) = (f32)(RAND_BETWEEN(0, 10)) / 10.f;
	}

	f32 factor = VALUE(colorLerpFactor, LavaScript);
	vec4 newColor = LERP_VEC4(parent->GetColor(), (*color), dt * factor);
	parent->SetColor(newColor);

	if (*color == YELLOW && (YELLOW.y - 0.01f) - newColor.y <= 0.015f)
	{
		REF(targetColor, LavaScript) = &RED;
	}
	else if (*color == RED && newColor.y <= 0.015f)
	{
		REF(targetColor, LavaScript) = &YELLOW;
	}
}

SCRIPT_ON_COLLISION_ENTER(GameObject* parent, GameObject* other)
{
	if (other->HasTag(Tag::t_DynamicObject))
	{
		Dandelion::DeleteGameObject(other);
		if (other->HasTag(Tag::t_Player))
		{
			Dandelion::GetCamera()->SetCameraShake(3.f, 7);

			Dandelion::PlaySound("on_death");
		}
	}
}

SCRIPT_ON_COLLISION_EXIT(GameObject* parent, GameObject* other)
{
}

END_SCRIPT;
