
#include "Level\Level.h"
#include "Level\LevelInclude.h"

#include <Dandelion.h>

#include <iostream>
#include <string>
#include <unordered_map>

Application* APP;
AudioVoice* CURRENT_MUSIC;

class Game : public Application
{
public:
	Game() {}
	~Game() {}

	std::unordered_map<string, Level *> levels;
	string currentLevel;

protected:
	void Init() override
	{
		levels.insert_or_assign("Level1", new Level1);
		levels.insert_or_assign("Level2", new Level2);
		levels.insert_or_assign("Level3", new Level3);
		levels.insert_or_assign("GameWin", new GameWin);
		levels.insert_or_assign("GameOver", new GameOver);
		
		currentLevel = "Level1";
		levels[currentLevel]->Init();

		Dandelion::LoadWave("assets/Sounds/jump.wav", "on_jump");
		Dandelion::LoadWave("assets/Sounds/land.wav", "on_land");
		Dandelion::LoadWave("assets/Sounds/level_music.wav", "level_music");
		Dandelion::LoadWave("assets/Sounds/lose_music.wav", "lose_music");
		Dandelion::LoadWave("assets/Sounds/on_collect.wav", "on_collect");
		Dandelion::LoadWave("assets/Sounds/player_death.wav", "on_death");
		Dandelion::LoadWave("assets/Sounds/win_music.wav", "win_music");

		CURRENT_MUSIC = Dandelion::PlaySound("level_music", true);
	}

	void Update(f32 dt) override
	{
		levels[currentLevel]->Update(dt);
	}

	void Exit() override
	{
		levels[currentLevel]->Exit();
		for (auto pair : levels)
		{
			delete pair.second;
			pair.second = nullptr;
		}

		levels.clear();
	}

	void LateUpdate(f32 dt) override
	{
		levels[currentLevel]->LateUpdate(dt);
		if (levels[currentLevel]->state != ls_Stay)
		{
			string temp = nullptr;
			switch (levels[currentLevel]->state)
			{
			case ls_GoToNext:
				temp = levels[currentLevel]->levelToGoTo;
				break;
			}

			if (temp)
			{
				levels[currentLevel]->Exit();
				levels[currentLevel]->state = ls_Stay;
				currentLevel = temp;
				Dandelion::DeleteAllGameObjects();
				Dandelion::DestroyAllArchetypes();
				levels[currentLevel]->Init();
			}
		}
	}
};

Application* CreateApplication()
{
	return new Game();
}

void DestroyApplication(Application* app)
{
	delete app;
}

#include "Application/Main.h"
