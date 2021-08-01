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

	//button callbacks, are public because getters and setters are the worst thing that has happened in human history
	buttonCallback onClick = onClickDefault;
	buttonCallback onHold = onHoldDefault;
	buttonCallback onRelease = onReleaseDefault;
	buttonCallback onCancel = onReleaseDefault;
	buttonCallback onHoverEnter = onHoverEnterDefault;
	buttonCallback onHoverExit = onHoverExitDefault;
	//pointer to be passed into button callbacks
	void* infoPointer;

private:

	//called by custom render sprite, renders text
	static void textDrawCall(void* ptr)
	{
		ButtonComponent* component = (ButtonComponent*)ptr;
		Rectangle& bounds = component->bounds;
		DrawText(component->string.c_str(), bounds.x + component->xPadding, bounds.y + bounds.height/2 - component->fontSize/2, component->fontSize, component->textDrawColor);
	}

	//default button callbacks:

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
	
	//I know technically these should be above the button callbacks to optimise padding but whatever its not worth the confusing public: private: public: private:
	std::string string;
	Rectangle bounds;

	bool pressed = false;
	int fontSize = 50;
	bool hovering = false;
	Color textDrawColor;
	int xPadding;
};

