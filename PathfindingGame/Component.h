#pragma once
#include "NecessaryHeaders.h"
class GameObject;
#include "Transform.h"

class Component
{
public:
	virtual void init(GameObject* connected);
	void start();
	void update();
	void onDisable();
	void onEnable();
	void unload();

	void disableComponent();
	void enableComponent();

	inline bool isEnabled() { return enabled; }

protected:
	GameObject* gameObject;
	Transform* transform;
	bool enabled;

};

