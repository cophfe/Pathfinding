#include "PlayerUIComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "Room.h"
#include "PlayerComponent.h"

void PlayerUIComponent::init(int heartAmount, int currentAmount, Room* scene, PlayerComponent* player)
{
	heartsTotal = heartAmount;
	heartsLeft = currentAmount;
	auto blackCircle = new GameObject(gameObject, "missing", true, Vector2(GetScreenWidth()/2, GetScreenHeight()/2), 0, 1);
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

	deathTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	BeginTextureMode(deathTexture);
	ClearBackground({ 0,0,0,0 });
	EndTextureMode();
	blackCircle->setSprite(new Sprite(&deathTexture.texture, blackCircle));
	this->player = player;
	this->room = scene;
}

void PlayerUIComponent::update()
{
	if (player->isDying())
	{
		float timer = (player->getDyingTimer() - 0.5f) / 3;
		if (timer < 1 && timer > 0)
		{
			auto target = room->getCamera()->getCamera()->target;
			auto offset = room->getCamera()->getCamera()->offset;
			//convert playerPosition to camera
			Vector2 playerPosition = Vector2(offset.x, offset.y + 40) + (room->getPlayerComponent()->getTransform()->getPosition() - Vector2(target.x, target.y)); //+ Vector2(GetScreenWidth() / 2, GetScreenHeight() / 2);
			BeginTextureMode(deathTexture);
			ClearBackground(BLACK);
			BeginBlendMode(BLEND_SUBTRACT_COLORS);
			DrawCircle(playerPosition.x, playerPosition.y, (1 - timer) * (1 - timer) * (1 - timer) * GetScreenWidth(), { 0xFF,0xFF,0xFF,0xFF });
			EndBlendMode();
			EndTextureMode();
		}
		else if (timer >= 1)
		{
			BeginTextureMode(deathTexture);
			ClearBackground(BLACK);
			EndBlendMode();
			
			disableComponent();
			//MOVE SCENE
		}
		
	}
	
}

void PlayerUIComponent::unload()
{
	//just need to delete array, gameObject will delete children
	delete hearts;

	UnloadRenderTexture(deathTexture);
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
