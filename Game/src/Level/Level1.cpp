#include <Dandelion.h>
#include "../Scripts/ScriptInclude.h"
#include "Level1.h"

#include <iostream>

extern AudioVoice* CURRENT_MUSIC;

void Level1::InitArchetypes()
{
	Dandelion::StopSound(CURRENT_MUSIC);
	CURRENT_MUSIC = Dandelion::PlaySound("level_music", true);
	GameObject* lava = Dandelion::CreateNewArchetype("lava");
	Body* lavaBody = lava->AttachBody(st_AABB, pmt_Rock);
	lavaBody->body->isStatic = true;
	lava->AddTag(t_Lava);
	lava->AddTag(t_StaticObject);
	lava->SetColor(vec4(245.f / 255.f, 66.f / 255.f, 66.f / 255.f, 1.f));
	lava->AttachScript(LavaScript);

	GameObject* stone = Dandelion::CreateNewArchetype("stone");
	Body* stoneBody = stone->AttachBody(st_AABB, pmt_Rock);
	stoneBody->body->isStatic = true;
	stone->SetColor(vec4(92.f / 255.f, 88.f / 255.f, 87.f / 255.f, 1.f));
	stone->AddTag(t_Platform);
	stoneBody->body->dynamicFriction = 0.3f;
	stoneBody->body->staticFriction = 0.3f;

	GameObject* bullet = Dandelion::CreateNewArchetype("bullet");
	bullet->SetScale(vec2(0.2f, 0.2f));
	bullet->AddTag(t_Bullet);
	bullet->AddTag(t_DynamicObject);
	bullet->SetColor(vec4(252.f / 255.f, 186.f / 255.f, 3.f / 255.f, 1.f));
	bullet->AttachScript(BulletScript);
	Body* bulletBody = bullet->AttachBody(st_Circle, pmt_Metal);
	bulletBody->body->gravityScale = 0.f;
	bulletBody->shape->isTrigger = true;

	GameObject* killPlane = Dandelion::CreateNewArchetype("kill_plane");
	Body* killPlaneBody = killPlane->AttachBody(st_AABB, pmt_Static, false);
	killPlaneBody->shape->isTrigger = true;
	killPlane->AttachScript(KillPlane);

	GameObject* collectable = Dandelion::CreateNewArchetype("collectable");
	collectable->SetScale(vec2(0.3f, 0.3f));
	collectable->AddTag(t_Collectable);
	collectable->AddTag(t_StaticObject);
	collectable->AttachScript(Collectable);
	collectable->SetColor(vec4(194.f / 255.f, 66.f / 255.f, 245.f / 255.f, 1.f));
	Body* collectableBody = collectable->AttachBody(st_AABB, pmt_Static, false);
	collectableBody->shape->isTrigger = true;
}

void Level1::Create()
{
	Dandelion::SetGravity(GRAVITY_MODE_DIRECTION, vec2(0, -1));

	GameObject* playerSpawner = Dandelion::CreateGameObject("playerSpawner", vec2(0, 2));
	playerSpawner->SetScale(vec2(0.0001f, 0.0001f));
	playerSpawner->AttachScript(PlayerSpawner);
	playerSpawner->AddTag(t_StaticObject);
	playerSpawner->GetScript("PlayerSpawner").refVariables["level"] = this;

	GameObject* instance;
	CREATE("stone", 0, 0.5, 6, 1);
	CREATE("stone", 0, -4.5, 2, 1);
	CREATE("stone", 0, 8.5, 2, 1);
	CREATE("stone", -5.5, 6.5, 3, 1);
	CREATE("stone", 5.5, 6.5, 3, 1);
	CREATE("stone", -6.5, -3.5, 4, 1);
	CREATE("stone", 6.5, -3.5, 4, 1);

	CREATE("lava", -7.5, 1, 3, 2);
	CREATE("lava", 7.5, 1, 3, 2);
	CREATE("lava", 0, 9.5, 8, 1);
	CREATE("lava", -3.5, 8.5, 1, 1);
	CREATE("lava", 3.5, 8.5, 1, 1);
	CREATE("lava", 0, -8.5, 6, 1);
	CREATE("lava", -2.5, -7.5, 1, 1);
	CREATE("lava", 2.5, -7.5, 1, 1);

	CREATE("lava", -9.5, 9.5, 1, 1);
	CREATE("lava", 9.5, 9.5, 1, 1);
	CREATE("lava", -9.5, -9.5, 1, 1);
	CREATE("lava", 9.5, -9.5, 1, 1);

	CREATE("kill_plane", -30, 0, 10, 100);
	CREATE("kill_plane", 30, 0, 10, 100);
	CREATE("kill_plane", 0, -30, 100, 10);
	CREATE("kill_plane", 0, 30, 100, 10);

	CREATE_COLLECTABLE(-7.5, 3.5);

	CREATE_COLLECTABLE(7.5, 3.5);

	CREATE_COLLECTABLE(0, -6.5);

	CREATE_COLLECTABLE(-9, -7);

	CREATE_COLLECTABLE(9, -7);
}

void Level1::Update(float dt)
{
	if (!numCollects)
	{
		SetLevelState(ls_GoToNext);
		levelToGoTo = "Level2";
	}
}
