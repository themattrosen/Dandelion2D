#pragma once

enum LevelState
{
	ls_Stay,
	ls_GoToNext,
};

class Level
{
public:
	Level() : 
		state(ls_Stay),
		levelToGoTo(nullptr)
	{}

	virtual void InitArchetypes() = 0;
	virtual void Create() = 0;
	virtual void Update(float dt) = 0;
	virtual void LateUpdate(float dt) {}
	virtual void Exit() {}

	void Init()
	{
		InitArchetypes();
		Create();
	}

	void SetLevelState(LevelState newState)
	{
		state = newState;
	}

	LevelState state;
	const char* levelToGoTo;
	void OnCollect()
	{
		--numCollects;
	}

	unsigned numCollects;
};

#define CREATE(archetype, posX, posY, scaleX, scaleY) \
	instance = Dandelion::Instantiate(archetype);	\
	instance->SetPosition(vec2((float)posX / 2.f, (float)posY / 2.f));	\
	instance->SetScale(vec2((float)scaleX / 2.f, (float)scaleY / 2.f))

#define CREATE_COLLECTABLE(posX, posY) \
	instance = Dandelion::Instantiate("collectable");	\
	instance->SetPosition(vec2((float)posX / 2.f, (float)posY / 2.f));	\
	instance->GetScript("Collectable").refVariables["spawner"] = this;	\
	++numCollects
