#pragma once
#include "NecessaryHeaders.h"
#include "Component.h"
#include "GameObject.h"
#include "Sprite.h"


class ButtonComponent :
    public Component
{
public:
	typedef void(*buttonCallback)(ButtonComponent*,void*);
	virtual void init(std::string writing, Color textDrawColor, int padding, int fontSize);
	virtual void update();


	buttonCallback onClick = onClickDefault;
	buttonCallback onHold = onHoldDefault;
	buttonCallback onRelease = onReleaseDefault;
	buttonCallback onCancel = onReleaseDefault;
	buttonCallback onHoverEnter = onHoverEnterDefault;
	buttonCallback onHoverExit = onHoverExitDefault;
	void* infoPointer;
private:
	static void textDrawCall(void* ptr)
	{
		ButtonComponent* component = (ButtonComponent*)ptr;
		Rectangle& bounds = component->bounds;
		DrawText(component->string.c_str(), bounds.x + component->xPadding, bounds.y + bounds.height/2 - component->fontSize/2, component->fontSize, component->textDrawColor);
	}

	//defaultHover
	static void onClickDefault(ButtonComponent* button, void* pointer)
	{
		button->getGameObject()->getSprite()->setTint({ 0x80,0x80,0x80, 0xFF });
	}
	static void onReleaseDefault(ButtonComponent* button, void* pointer)
	{
		button->getGameObject()->getSprite()->setTint({ 0xC0, 0xC0, 0xC0, 0xFF });
	}
	static void onCancelDefault(ButtonComponent* button, void* pointer)
	{
		button->getGameObject()->getSprite()->setTint({ 0xC0, 0xC0, 0xC0, 0xFF });
	}
	static void onHoldDefault(ButtonComponent* button, void* pointer)
	{

	}
	static void onHoverEnterDefault(ButtonComponent* button, void* pointer)
	{
		button->getGameObject()->getSprite()->setTint({ 0xC0, 0xC0, 0xC0, 0xFF });
	}
	static void onHoverExitDefault(ButtonComponent* button, void* pointer)
	{
		button->getGameObject()->getSprite()->setTint({ 0xFF, 0xFF, 0xFF, 0xFF });
	}

	
	bool pressed = false;
	int fontSize = 50;
	bool hovering = false;
	std::string string;
	Rectangle bounds;
	Color textDrawColor;
	int xPadding;
};

