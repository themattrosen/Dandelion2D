#include <Dandelion.h>
#include "../Scripts/ScriptInclude.h"
#include "Level2.h"
extern AudioVoice* CURRENT_MUSIC;

void Level2::InitArchetypes()
{
	Dandelion::StopSound(CURRENT_MUSIC);
	CURRENT_MUSIC = Dandelion::PlaySound("level_music", true);
	GameObject* lava = Dandelion::CreateNewArchetype("lava");
	Body* lavaBody = lava->AttachBody(st_AABB, pmt_Rock);
	lava->AddTag(t_Lava);
	lava->AddTag(t_StaticObject);
	lavaBody->body->isStatic = true;

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

void Level2::Create()
{
	Dandelion::SetGravity(GRAVITY_MODE_DIRECTION, vec2(this->gravity, 0));
	GameObject* playerSpawner = Dandelion::CreateGameObject("playerSpawner", vec2(-7.f / 2.f, 0.f));
	playerSpawner->SetScale(vec2(0.0001f, 0.0001f));
	playerSpawner->AttachScript(PlayerSpawner);
	playerSpawner->AddTag(t_StaticObject);
	playerSpawner->GetScript("PlayerSpawner").refVariables["level"] = this;

	GameObject* instance;

	// corners
	CREATE("lava", -9.5, 9.5, 1, 1);
	CREATE("lava", -9.5, 8.5, 1, 1);
	CREATE("lava", -8.5, 9.5, 1, 1);
	CREATE("lava", 9.5, 9.5, 1, 1);
	CREATE("lava", 9.5, 8.5, 1, 1);
	CREATE("lava", 8.5, 9.5, 1, 1);
	CREATE("lava", -9.5, -9.5, 1, 1);
	CREATE("lava", -9.5, -8.5, 1, 1);
	CREATE("lava", -8.5, -9.5, 1, 1);
	CREATE("lava", 9.5, -9.5, 1, 1);
	CREATE("lava", 9.5, -8.5, 1, 1);
	CREATE("lava", 8.5, -9.5, 1, 1);

	// mid area lava
	CREATE("lava", -6.5, 4, 1, 2);
	CREATE("lava", -6.5, -4, 1, 2);
	CREATE("lava", 0, 0, 2, 2);
	CREATE("lava", 5.5, 6, 1, 2);
	CREATE("lava", 5.5, -6, 1, 2);
	CREATE("lava", 6.5, 0, 1, 5);

	// edge platforms
	CREATE("stone", -1.5, 7, 1, 4);
	CREATE("stone", 1.5, -7, 1, 4);
	CREATE("stone", -10.5, 0, 1, 20);
	CREATE("stone", 10.5, 0, 1, 20);
	CREATE("stone", 0, 10.5, 20, 1);
	CREATE("stone", 0, -10.5, 20, 1);

	// center platforms
	CREATE("stone", 2, 2.5, 2, 1);
	CREATE("stone", 2.5, 1.5, 1, 1);
	CREATE("stone", -2, 2.5, 2, 1);
	CREATE("stone", -2.5, 1.5, 1, 1);
	CREATE("stone", -2, -2.5, 2, 1);
	CREATE("stone", -2.5, -1.5, 1, 1);
	CREATE("stone", 2, -2.5, 2, 1);
	CREATE("stone", 2.5, -1.5, 1, 1);

	CREATE_COLLECTABLE(2, 7);
	CREATE_COLLECTABLE(-2, -7);
	CREATE_COLLECTABLE(3.5, 3.5);
	CREATE_COLLECTABLE(-3.5, -3.5);
}

void Level2::Update(float dt)
{
	static const f32 GRAVITY_FLIP_TIME = 10.f;
	/*timer -= dt;
	if (timer <= 0.f)
	{
		timer = GRAVITY_FLIP_TIME;
		gravity *= -1.f;
		Dandelion::SetGravity(GRAVITY_MODE_DIRECTION, vec2(gravity, 0));
	}*/

	if (!numCollects)
	{
		SetLevelState(ls_GoToNext);
		levelToGoTo = "Level3";
	}
}
