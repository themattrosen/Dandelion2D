#include <Dandelion.h>
#include "Level3.h"
#include "../Scripts/ScriptInclude.h"
extern AudioVoice* CURRENT_MUSIC;

void Level3::InitArchetypes()
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

void Level3::Create()
{
	Dandelion::SetGravity(GRAVITY_MODE_DIRECTION, vec2(1, -1));
	GameObject* playerSpawner = Dandelion::CreateGameObject("playerSpawner", vec2(0.f, 4.f / 2.f));
	playerSpawner->SetScale(vec2(0.0001f, 0.0001f));
	playerSpawner->AttachScript(PlayerSpawner);
	playerSpawner->AddTag(t_StaticObject);
	playerSpawner->GetScript("PlayerSpawner").refVariables["level"] = this;

	// edge lava
	GameObject* instance;
	CREATE("lava", -9, 9, 2, 2);
	CREATE("lava", 9, 9, 2, 2);
	CREATE("lava", 9, -9, 2, 2);
	CREATE("lava", -9, -9, 2, 2);
	CREATE("lava", -9.5, 7.5, 1, 1);
	CREATE("lava", -7.5, 9.5, 1, 1);
	CREATE("lava", -9.5, -7.5, 1, 1);
	CREATE("lava", -7.5, -9.5, 1, 1);
	CREATE("lava", 9.5, -7.5, 1, 1);
	CREATE("lava", 7.5, -9.5, 1, 1);
	CREATE("lava", 9.5, 7.5, 1, 1);
	CREATE("lava", 7.5, 9.5, 1, 1);

	// center
	CREATE("stone", 0, 0, 2, 2);
	CREATE("stone", 2, 2.5, 2, 1);
	CREATE("stone", 2.5, 1.5, 1, 1);
	CREATE("lava", -2, 2.5, 2, 1);
	CREATE("lava", -2.5, 1.5, 1, 1);
	CREATE("stone", -2, -2.5, 2, 1);
	CREATE("stone", -2.5, -1.5, 1, 1);
	CREATE("lava", 2, -2.5, 2, 1);
	CREATE("lava", 2.5, -1.5, 1, 1);

	// next ring
	CREATE("stone", 7, 0, 2, 2);
	CREATE("stone", -7, 0, 2, 2);
	CREATE("stone", 0, 7, 2, 2);
	CREATE("stone", 0, -7, 2, 2);

	CREATE("lava", 3.5, 6, 1, 2);
	CREATE("lava", 5, 5.5, 2, 1);
	CREATE("lava", 6, 4.5, 2, 1);

	CREATE("stone", -3.5, 6, 1, 2);
	CREATE("stone", -5, 5.5, 2, 1);
	CREATE("stone", -6, 4.5, 2, 1);

	CREATE("lava", -3.5, -6, 1, 2);
	CREATE("lava", -5, -5.5, 2, 1);
	CREATE("lava", -6, -4.5, 2, 1);

	CREATE("stone", 3.5, -6, 1, 2);
	CREATE("stone", 5, -5.5, 2, 1);
	CREATE("stone", 6, -4.5, 2, 1);

	// collectables
	CREATE_COLLECTABLE(-4.5, 6.5);
	CREATE_COLLECTABLE(4.5, -6.5);
	CREATE_COLLECTABLE(-5.5, 0);
	CREATE_COLLECTABLE(5.5, 0);
	CREATE_COLLECTABLE(0, 5.5);
	CREATE_COLLECTABLE(0, -5.5);
	CREATE_COLLECTABLE(3.5, 3.5);
	CREATE_COLLECTABLE(-3.5, -3.5);

}

void Level3::Update(float dt)
{
	if (!numCollects)
	{
		SetLevelState(ls_GoToNext);
		levelToGoTo = "GameWin";
	}
}
