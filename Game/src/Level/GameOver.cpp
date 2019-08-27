#include <Dandelion.h>
#include "../Scripts/ScriptInclude.h"
#include "GameOver.h"
extern AudioVoice* CURRENT_MUSIC;

static const vec2 GRAVS[] =
{
	{1, 0},
	{-1, 0},
	{0, 1},
	{0, -1}
};

static const f32 SWITCH_TIME = 1.f;

void GameOver::InitArchetypes()
{
	Dandelion::StopSound(CURRENT_MUSIC);
	CURRENT_MUSIC = Dandelion::PlaySound("lose_music", true);
	GameObject* lava = Dandelion::CreateNewArchetype("lava");
	Body* lavaBody = lava->AttachBody(st_AABB, pmt_Rock);
	lava->AddTag(t_Lava);
	lava->AddTag(t_StaticObject);
	lava->SetColor(vec4(245.f / 255.f, 66.f / 255.f, 66.f / 255.f, 1.f));
	lava->AttachScript(LavaScript);
	lavaBody->body->isStatic = true;

	GameObject* stone = Dandelion::CreateNewArchetype("stone");
	Body* stoneBody = stone->AttachBody(st_AABB, pmt_Rock);
	stoneBody->body->isStatic = false;
	stone->SetColor(vec4(92.f / 255.f, 88.f / 255.f, 87.f / 255.f, 1.f));
	stone->AddTag(t_Platform);
	stone->AddTag(t_DynamicObject);
	stoneBody->body->dynamicFriction = 0.3f;
	stoneBody->body->staticFriction = 0.3f;
}

void GameOver::Create()
{
	GameObject* instance;
	CREATE("lava", -10.5, 0, 1, 20);

	CREATE("lava", 10.5, 0, 1, 20);

	CREATE("lava", 0, 10.5, 20, 1);

	CREATE("lava", 0, -10.5, 20, 1);

}

void GameOver::Update(float dt)
{
	timer -= dt;
	if (timer <= 0.f)
	{
		gravIndex = (gravIndex + 1) % 4;
		Dandelion::SetGravity(GRAVITY_MODE_DIRECTION, GRAVS[gravIndex]);
		timer = SWITCH_TIME;

		u32 numObjects = Dandelion::GetNumGameObjects();
		for (u32 i = numObjects; i < MAX_GAME_OBJECTS; ++i)
		{
			f32 x = (f32)RAND_BETWEEN(-9, 9);
			f32 y = (f32)RAND_BETWEEN(-9, 9);
			GameObject* instance;
			CREATE("stone", x, y, 1.5, 1.5);
		}
	}

	if (Dandelion::IsMouseButtonTriggered(MouseButton::MouseLeft))
	{
		SetLevelState(ls_GoToNext);
		levelToGoTo = "Level1";
	}
}
