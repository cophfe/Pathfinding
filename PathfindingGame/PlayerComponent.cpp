#include "PlayerComponent.h"

void PlayerComponent::update()
{
	if (IsKeyPressed(KEY_D))
	{
		transform->addPosition(4 * Game::getDeltaTime())
	}
	if (IsKeyPressed(KEY_A))
	{

	}
}
