#pragma once
#include "NecessaryHeaders.h"
#include "Component.h"
#include "Sprite.h"
class Room;
class PlayerComponent;

//generates all player UI
class PlayerUIComponent :
    public Component
{
public:
	void init(int heartAmount, int currentAmount, Room* scene, PlayerComponent* player);
	void unload();
	void update();
	int getHealth() { return heartsLeft; }
	void hit(int number);
	void heal(int number);
private:
	
	//heart frames:
	static constexpr int beatStart = 0;
	static constexpr int beatEnd = 28;
	static constexpr int dieStart = 29;
	static constexpr int dieEnd = 35;
	static constexpr int reviveStart = 36;
	static constexpr int reviveEnd = 38;
	//heart settings:
	static constexpr float heartPadding = 40;
	static constexpr float heartBetween = 10;
	static constexpr float heartScale = 1.0f;
	//minimap settings:
	static constexpr int mapPadding = 10;


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
		spriteData->setCallback(nullptr, nullptr);
	}

	GameObject** hearts;
	GameObject* miniMap;
	int heartsTotal;
	int heartsLeft;
	RenderTexture2D deathTexture;
	PlayerComponent* player;
	Room* room;
};


