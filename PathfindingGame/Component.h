#pragma once
#include "NecessaryHeaders.h"
class GameObject;
#include "Transform.h"

class Component
{
public:
	Component();
	virtual ~Component() = default;
	virtual void init();
	virtual void start();
	virtual void update();
	virtual void fixedUpdate();
	virtual void onDisable();
	virtual void onEnable();
	virtual void unload();

	void setGameObject(GameObject* connected);

	void disableComponent();
	void enableComponent();

	inline bool isEnabled() { return enabled; }

protected:
	GameObject* gameObject;
	Transform* transform;
	bool enabled;
};

