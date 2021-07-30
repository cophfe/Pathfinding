#include "PlayerUIComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "Room.h"

void PlayerUIComponent::init(int heartAmount, int currentAmount, Room* scene)
{
	heartsTotal = heartAmount;
	heartsLeft = currentAmount;
	hearts = new GameObject * [heartAmount];

	Vector2 startPosition = { heartPadding, heartPadding };
	
	hearts[0] = new GameObject(gameObject, "heart", true, { startPosition.x, startPosition.y }, 0, heartScale);
	((AnimatedSprite*)hearts[0]->getSprite())->setSettings(beatStart, beatEnd, beatStart);
	int offset = ((AnimatedSprite*)hearts[0]->getSprite())->getDestinationRectangle()->width;

	for (int i = 1; i < heartAmount; i++)
	{
		//will add itself as child
		hearts[i] = new GameObject(gameObject, "heart", true, { startPosition.x + (heartBetween + offset) * i, startPosition.y }, 0, heartScale);
		if (i > currentAmount - 1)
		{
			((AnimatedSprite*)hearts[i]->getSprite())->pauseAt(dieEnd);
		}
		else
		{
			((AnimatedSprite*)hearts[i]->getSprite())->setSettings(beatStart, beatEnd, beatStart + (i * 3) % beatEnd);
		}
	}
	//Generate minimap
	scene->getRoomManager()->generateMiniMap();
	RenderTexture* miniMapTex = scene->getRoomManager()->getMiniMap();
	miniMap = new GameObject(gameObject, "missing", true, Vector2((float)(GetScreenWidth() - miniMapTex->texture.width/2 - mapPadding), miniMapTex->texture.height/2 + mapPadding), 0, 1);
	miniMap->setSprite(new Sprite(&miniMapTex->texture, miniMap));
}

void PlayerUIComponent::unload()
{
	//just need to delete array, gameObject will delete children
	delete hearts;
}

void PlayerUIComponent::hit(int number)
{
	while (number > 0 && heartsLeft > 0)
	{
		auto sprite = (AnimatedSprite*)hearts[heartsLeft - 1]->getSprite();
		sprite->setSettings(dieStart, dieEnd, dieStart);
		sprite->play();
		sprite->setCallback(dieCallback, sprite);
		heartsLeft--;
		number--;
	}
}

void PlayerUIComponent::heal(int number)
{
	while (number > 0 && heartsLeft < heartsTotal)
	{
		auto sprite = (AnimatedSprite*)hearts[heartsLeft - 1]->getSprite();
		sprite->setSettings(reviveStart, reviveEnd, reviveStart);
		sprite->play();
		sprite->setCallback(reviveCallback, sprite);
		heartsLeft++;
		number--;
	}
}
