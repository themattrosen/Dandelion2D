#include <Dandelion.h>
#include "../Scripts/ScriptInclude.h"
#include "GameWin.h"
extern AudioVoice* CURRENT_MUSIC;

static const vec2 GRAVS[] = 
{
	{1, 0},
	{-1, 0},
	{0, 1},
	{0, -1}
};

static const f32 SWITCH_TIME = 1.f;

void GameWin::InitArchetypes()
{
	Dandelion::StopSound(CURRENT_MUSIC);
	CURRENT_MUSIC = Dandelion::PlaySound("win_music", true);
	GameObject* lava = Dandelion::CreateNewArchetype("lava");
	Body* lavaBody = lava->AttachBody(st_AABB, pmt_Rock);
	lavaBody->body->isStatic = true;
	lava->AddTag(t_Lava);
	lava->AddTag(t_StaticObject);
	lava->SetColor(vec4(245.f / 255.f, 66.f / 255.f, 66.f / 255.f, 1.f));
	lava->AttachScript(LavaScript);

	GameObject* collectable = Dandelion::CreateNewArchetype("collectable");
	collectable->SetScale(vec2(0.3f, 0.3f));
	collectable->AddTag(t_Collectable);
	collectable->AddTag(t_StaticObject);
	collectable->AttachScript(Collectable);
	collectable->SetColor(vec4(194.f / 255.f, 66.f / 255.f, 245.f / 255.f, 1.f));
	Body* collectableBody = collectable->AttachBody(st_AABB, pmt_SuperBall);
	
	timer = SWITCH_TIME;
	gravIndex = 0;
}

void GameWin::Create()
{
	GameObject* instance;
	CREATE("lava", -10.5, 0, 1, 20);
	CREATE("lava", 10.5, 0, 1, 20);
	CREATE("lava", 0, 10.5, 20, 1);
	CREATE("lava", 0, -10.5, 20, 1);
}

void GameWin::Update(float dt)
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
			CREATE_COLLECTABLE(x, y);
		}
	}

	if (Dandelion::IsMouseButtonTriggered(MouseButton::MouseLeft))
	{
		SetLevelState(ls_GoToNext);
		levelToGoTo = "Level1";
	}
}
