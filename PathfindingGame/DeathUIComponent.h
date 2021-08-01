#pragma once
#include "Component.h"
#include "Room.h"
#include "Sprite.h"
#include "ButtonComponent.h"
#include "Game.h"

//holds all the UI for when you die
class DeathUIComponent :
    public Component
{
public:
	void init(int numberOfFloors, Scene* scene);
	void update();
private:
	static void exitClick(ButtonComponent* component, void* ptr)
	{
		Game::getInstance().setTimeScale(1);
		Game::getInstance().switchScene(Game::getInstance().createMenuScene());
	}
	static void retryClick(ButtonComponent* component, void* ptr)
	{
		Game::getInstance().setTimeScale(1);
		Game& game = Game::getInstance();
		auto rM = game.getRoomManager();
		if (rM->checkMapHasBeenGenerated())
		{
			rM->generateNewMap(rand(), false);
			game.switchScene(rM->createFirstRoom());
		}
		else
			game.switchScene(rM->createFirstRoom());
	}

	static constexpr float enterTime = 0.5f;
	float enterTimer = 0;
	bool entered = false;
};

