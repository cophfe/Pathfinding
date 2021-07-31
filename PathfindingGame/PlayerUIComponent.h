#pragma once
#include "NecessaryHeaders.h"
#include "Component.h"
#include "Sprite.h"
#include "Game.h"
#include "ButtonComponent.h"
class Room;
class PlayerComponent;

//generates all player UI
class PlayerUIComponent :
    public Component
{
public:
	//heart frames:
	static constexpr int beatStart = 0;
	static constexpr int beatEnd = 28;
	static constexpr int dieStart = 29;
	static constexpr int dieEnd = 35;
	static constexpr int reviveStart = 35;
	static constexpr int reviveEnd = 38;
	//heart settings:
	static constexpr float heartPadding = 40;
	static constexpr float heartBetween = 10;
	static constexpr float heartScale = 1.0f;
	static constexpr unsigned char extraHeartColorDifference = 0x40;
	//minimap settings:
	static constexpr int mapPadding = 10;

	void init(int heartAmount, int currentAmount, Room* scene, PlayerComponent* player);
	void unload();
	void update();
	int getHealth() { return heartsLeft; }
	void hit(int number);
	void heal(int number);
	void addHeart(int number);
private:
	
	

	static void exitClick(ButtonComponent* component, void* ptr)
	{
		auto self = (PlayerUIComponent*)ptr;
		Game::getInstance().setTimeScale(1);
		Game::getInstance().switchScene(Game::getInstance().createMenuScene());
	}
	static void continueClick(ButtonComponent* component, void* ptr)
	{
		auto self = (PlayerUIComponent*)ptr;
		self->menuState = CLOSING;
		self->menuTimer = 0;
	}

	static void dieCallback(void* ptr)
	{
		auto spriteData = (AnimatedSprite*)ptr;
		spriteData->pauseAt(dieEnd);
		spriteData->setCallback(nullptr, nullptr);
	}
	static void reviveCallback(void* ptr)
	{
		auto spriteData = (AnimatedSprite*)ptr;
		spriteData->setSettings(beatStart, beatEnd, beatStart);
		spriteData->playAt(beatStart);
		spriteData->setCallback(nullptr, nullptr);
	}

	enum : char
	{
		OPENING,
		OPEN,
		CLOSING,
		CLOSED
	};
	char menuState = CLOSED;
	float menuTimer = 0;
	static constexpr float menuTime = 0.5f;

	std::vector<GameObject*> hearts;
	GameObject* miniMap;
	GameObject* menu;
	int heartsTotal;
	int heartsLeft;
	RenderTexture2D* deathTexture;
	PlayerComponent* player;
	Room* room;
};


