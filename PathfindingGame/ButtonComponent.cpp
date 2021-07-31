#include "ButtonComponent.h"
#include "GameObject.h"

void ButtonComponent::init(std::string writing, Color textDrawColor, int padding, int fontSize)
{
	this->fontSize = fontSize;
	xPadding = padding;
	this->textDrawColor = textDrawColor;
	string = writing;
	auto text = new GameObject(new CustomRenderSprite(textDrawCall, this));
	transform->addChild(text->getTransform());
	bounds = *gameObject->getSprite()->getDestinationRectangle();
	bounds.x -= bounds.width/2;
	bounds.y -= bounds.height/2;
	infoPointer = nullptr;
}

void ButtonComponent::update()
{
	Vector2 mousePosition = Vector2::fromRL(GetMousePosition());
	
	bounds = *gameObject->getSprite()->getDestinationRectangle();
	bounds.x -= bounds.width / 2;
	bounds.y -= bounds.height / 2;

	if (mousePosition.x > bounds.x && mousePosition.x < bounds.x + bounds.width
		&& mousePosition.y > bounds.y && mousePosition.y < bounds.y + bounds.height)
	{
		if (!hovering)
		{
			onHoverEnter(this, infoPointer);
			hovering = true;
		}
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			if (pressed)
			{
				onHold(this, infoPointer);
			}
			else
			{
				onClick(this, infoPointer);
				pressed = true;
			}
		}
		else if (pressed)
		{
			onRelease(this, infoPointer);

			pressed = false;
		}
	}
	else
	{
		if (pressed && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
		{
			onCancel(this, infoPointer);
			pressed = false;
		}
		if (!pressed && hovering)
		{
			onHoverExit(this, infoPointer);
			hovering = false;
		}
		//outside bounds
	}
}
