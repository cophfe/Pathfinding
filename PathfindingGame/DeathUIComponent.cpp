#include "DeathUIComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"

void DeathUIComponent::init(int numberOfFloors, Scene* scene)
{
	//generate children
	transform->setPosition({ GetScreenWidth()/2.0f, GetScreenHeight() * -1.0f});
	//making death text a button just because that's the only component I have made that can render text and I can't be bothered
	(new GameObject(gameObject, "missing", false, { -130, -170 }, 0, 0.5f))->addComponent<ButtonComponent>()->init("YOU DIED", RED, 0, 75);
	std::string string("YOU LASTED ");
	string += std::to_string(numberOfFloors - 1);
	string += " FLOOR";
	if (numberOfFloors - 1 != 1)
		string += "S";
	(new GameObject(gameObject, "missing", false, { -125, -70 }, 0, 0.5f))->addComponent<ButtonComponent>()->init(string, RED, 0, 30);
	
	//make retry and exit buttons
	auto retryButton = (new GameObject(gameObject, "button", true, { 0, 70 }, 0, 0.5f))->addComponent<ButtonComponent>();
	auto backToMenuButton = (new GameObject(gameObject, "button", true, { 0 , 170 }, 0, 0.5f))->addComponent<ButtonComponent>();
	retryButton->init("RETRY?", WHITE, 40, 35);
	retryButton->onRelease = retryClick;
	backToMenuButton->init("EXIT", WHITE, 40, 35);
	backToMenuButton->onRelease = exitClick;
	backToMenuButton->infoPointer = this;
}

void DeathUIComponent::update()
{
	//move death menu through this easing function (moved from off the screen to the centre)
	if (enterTimer < 1)
	{
		enterTimer += Game::getUnscaledDeltaTime() / enterTime;
		float x = 1 - (1 - enterTimer) * (1 - enterTimer) * (1 - enterTimer);

		transform->setPosition({ transform->getPosition().x, -GetScreenHeight() + x * GetScreenHeight() * 1.5f });
	}
	else if (!entered)
	{
		entered = true;
		transform->setPosition({ transform->getPosition().x, GetScreenHeight() / 2.0f });
	}
	
}
