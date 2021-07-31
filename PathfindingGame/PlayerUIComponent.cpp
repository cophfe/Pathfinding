#include "PlayerUIComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "Room.h"
#include "PlayerComponent.h"
#include "ButtonComponent.h"

void PlayerUIComponent::init(int heartAmount, int currentAmount, Room* scene, PlayerComponent* player)
{
	//generate menu
	menu = new GameObject(gameObject, "pauseMenu", true, Vector2(GetScreenWidth() / 2, -GetScreenHeight()), 0, 1);
	auto continueButton = (new GameObject(menu, "button", true, { 0, -100 }, 0, 0.5f))->addComponent<ButtonComponent>();
	auto backToMenuButton = (new GameObject(menu, "button", true, { 0 , 100 }, 0, 0.5f))->addComponent<ButtonComponent>();
	continueButton->init("CONTINUE", WHITE, 25, 35);
	continueButton->onRelease = continueClick;
	continueButton->infoPointer = this;
	backToMenuButton->init("EXIT", WHITE, 40, 35);
	backToMenuButton->onRelease = exitClick;
	backToMenuButton->infoPointer = this;
	menu->disableAndHide();
	//Generate death texture (circle)
	deathTexture = player->getDeathTexture();
	BeginTextureMode(*deathTexture);
	ClearBackground({ 0,0,0,0 });
	EndTextureMode();
	auto blackCircle = new GameObject(gameObject, "missing", true, Vector2(GetScreenWidth()/2, GetScreenHeight()/2), 0, 1);
	blackCircle->setSprite(new Sprite(&deathTexture->texture, blackCircle));
	this->player = player;
	this->room = scene;	
	heartsTotal = heartAmount;
	heartsLeft = currentAmount;

	Vector2 startPosition = { heartPadding, heartPadding };
	hearts.push_back(new GameObject(gameObject, "heart", true, { startPosition.x, startPosition.y }, 0, heartScale));
	((AnimatedSprite*)hearts[0]->getSprite())->setSettings(beatStart, beatEnd, beatStart);
	int offset = ((AnimatedSprite*)hearts[0]->getSprite())->getDestinationRectangle()->width;

	for (int i = 1; i < heartAmount; i++)
	{
		//will add itself as child
		hearts.push_back(new GameObject(gameObject, "heart", true, { startPosition.x + (heartBetween + offset) * i, startPosition.y }, 0, heartScale));
		if (i > currentAmount - 1)
		{
			((AnimatedSprite*)hearts[i]->getSprite())->pauseAt(dieEnd);
		}
		else
		{
			((AnimatedSprite*)hearts[i]->getSprite())->setSettings(beatStart, beatEnd, beatStart + (i * 3) % beatEnd);
		}
		if (i > PlayerComponent::MAX_HEALTH - 1)
		{
			//extra hearts are coloured blue
			hearts[i]->getSprite()->setTint({ extraHeartColorDifference ,extraHeartColorDifference , 0xFF, 0xFF });
		}
	}
	//Generate minimap
	scene->getRoomManager()->generateMiniMap();
	RenderTexture* miniMapTex = scene->getRoomManager()->getMiniMap();
	miniMap = new GameObject(gameObject, "missing", true, Vector2((float)(GetScreenWidth() - miniMapTex->texture.width/2 - mapPadding), miniMapTex->texture.height/2 + mapPadding), 0, 1);
	miniMap->setSprite(new Sprite(&miniMapTex->texture, miniMap));
}

void PlayerUIComponent::update()
{
	if (player->isDying())
	{
		float timer = (player->getDyingTimer() - 0.5f) / 3;
		
		if (timer < 1 && timer > 0)
		{
			auto target = room->getCamera()->getCamera()->target;
			Vector2 playerPosition = (player->getTransform()->getGlobalPosition());
			//convert playerPosition to camera
			BeginTextureMode(*deathTexture);
			ClearBackground(BLACK);
			BeginBlendMode(BLEND_SUBTRACT_COLORS);

			float scale = room->getCamera()->getScale();
			DrawCircle(GetScreenWidth() / 2 + scale * (playerPosition.x - target.x), GetScreenHeight() / 2 - scale * (playerPosition.y - target.y) + PlayerComponent::DRAW_OFFSET, (1 - timer) * (1 - timer) * (1 - timer) * GetScreenWidth() * 1.25f, { 0xFF,0xFF,0xFF,0xFF });
			
			EndBlendMode();
			EndTextureMode();
		}
		else if (timer >= 1)
		{
			BeginTextureMode(*deathTexture);
			ClearBackground(BLACK);
			EndBlendMode();
			timer = -3;
			disableComponent();
			//MOVE SCENE
			Game::getInstance().switchScene(Game::getInstance().createDeathScene(player->getFloor()));
		}
		
	}
	else if (IsKeyDown(KEY_ESCAPE))
	{
		switch (menuState)
		{
		case OPEN:
			menuState = CLOSING;
			menuTimer = 0;
			break;
		case CLOSED:
			menu->enableAndShow();
			player->pause();
			menuState = OPENING;
			menuTimer = 0;
			break;
		}
	}

	switch (menuState)
	{
	case OPENING:
		{
			menuTimer += Game::getUnscaledDeltaTime() / menuTime;
			float x = 1 - (1- menuTimer) * (1 - menuTimer) * (1 - menuTimer);

			menu->getTransform()->setPosition({ menu->getTransform()->getPosition().x, -GetScreenHeight() + x * GetScreenHeight() * 1.5f });
			BeginTextureMode(*deathTexture);
			ClearBackground({ 0,0,0, (unsigned char)(0x80 * x) });
			EndTextureMode();
			if (menuTimer >= 1)
			{
				menu->getTransform()->setPosition({ menu->getTransform()->getPosition().x, GetScreenHeight() / 2.0f});
				menuState = OPEN;
				menuTimer = 0;
				BeginTextureMode(*deathTexture);
				ClearBackground({ 0,0,0, 0x80 });
				EndTextureMode();
			}
		}
		break;
	case CLOSING:
		{
			menuTimer += Game::getUnscaledDeltaTime() / menuTime;
			float x = menuTimer * menuTimer * menuTimer;
			menu->getTransform()->setPosition({ menu->getTransform()->getPosition().x, GetScreenHeight() / 2 - x * GetScreenHeight() * 1.5f });
			
			BeginTextureMode(*deathTexture);
			ClearBackground({ 0,0,0, (unsigned char)(0x80 * (1- x)) });
			EndTextureMode();

			if (menuTimer >= 1)
			{
				menu->getTransform()->setPosition({ (float)menu->getTransform()->getPosition().x, -(float)GetScreenHeight() });
				menuState = CLOSED;
				menu->disableAndHide();
				player->resume();
				menuTimer = 0;
				BeginTextureMode(*deathTexture);
				ClearBackground({ 0,0,0, 0 });
				EndTextureMode();
			}
		}
		break;
	}
}

void PlayerUIComponent::unload()
{
	
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
		auto sprite = (AnimatedSprite*)hearts[heartsLeft]->getSprite();
		sprite->setSettings(reviveStart, reviveEnd, reviveStart);
		sprite->play();
		sprite->setCallback(reviveCallback, sprite);
		heartsLeft++;
		number--;
	}
	player->setHealth(heartsLeft);
}

void PlayerUIComponent::addHeart(int number)
{
	Vector2 startPosition = { heartPadding, heartPadding };
	int offset = ((AnimatedSprite*)hearts[0]->getSprite())->getDestinationRectangle()->width;
	while (number > 0)
	{
		hearts.push_back(new GameObject(gameObject, "heart", true, { startPosition.x + (heartBetween + offset) * (hearts.size()), startPosition.y }, 0, heartScale));
		hearts[hearts.size() - 1]->getSprite()->setTint({ extraHeartColorDifference ,extraHeartColorDifference , 0xFF, 0xFF });
		heartsTotal++;
		number--;
	}
	player->setMaxHealth(hearts.size());
	
}
