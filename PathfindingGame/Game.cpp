#include "Game.h"

#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

#include "AgentComponent.h"
#include "AgentDataComponent.h"

void Game::init(GameProperties* properties )
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 		Initialize Game
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (properties->enableAntiAliasing)
		SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    //initialize window with initial properties
    InitWindow(properties->windowWidth, properties->windowHeight, properties->windowName.c_str());
    
	if (properties->targetFPS != 0)
		SetTargetFPS(properties->targetFPS);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   draw Loading Screen
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// since loading the textures can take a while first a loading screen is rendered
	BeginDrawing();
	ClearBackground(BLACK);
	DrawText("Loading...", properties->windowWidth/2 - 170, properties->windowHeight / 2 - 40, 80, RAYWHITE);
	EndDrawing();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Initialize TextureManager
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	textureManager = new TextureManager();
	textureManager->LoadTexturesFromFolder(properties->spriteLocation);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Create Scene
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	currentScene = 0;
	scenes.push_back(new Scene());
	SceneProperties scene1Properties;
	scene1Properties.backgroundTiling = "background";
	scene1Properties.backgroundScale = 1.436781609195402f;
	scenes[currentScene]->load(scene1Properties);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Initialize Player
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	auto target = new GameObject(scenes[currentScene], "beearBody");
	auto armSprite = new GameObject(target, "beearAttack", true, { 3,24 });
	((AnimatedSprite*)armSprite->getSprite())->setSettings(0, 11,0);
	PlayerComponent* pC = target->addComponent<PlayerComponent>();
	pC->init(4, 90.0f, armSprite);
	b2BodyDef bDef = RigidBodyComponent::genBodyDef(b2_dynamicBody, true);
	b2FixtureDef fDef = RigidBodyComponent::genFixtureDef(RigidBodyComponent::PLAYER);
	b2CircleShape playerShape;
	target->getSprite()->setDrawOffset(target->getSprite()->getDestinationRectangle()->height / 2 - 80);
	armSprite->getSprite()->setDrawOffset(target->getSprite()->getDrawOffset());
	playerShape.m_radius = 0.7f;
	fDef.shape = &playerShape;
	target->addComponent<RigidBodyComponent>()->init(scenes[currentScene]->getCollisionManager(), bDef, fDef);
	scenes[0]->getCamera()->Target(target->getTransform());
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Initialize Actors
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	///first blackboard is made
	AgentDataComponent* blackboard = (new GameObject(scenes[currentScene], nullptr, false, { 0 }, 0, 1, SORTING::BACKGROUND))->addComponent<AgentDataComponent>();
	blackboard->init(target, scenes[currentScene]->getPathfinder());
	//then actors
	b2BodyDef actorbDef = RigidBodyComponent::genBodyDef(b2_dynamicBody, true);
	b2FixtureDef actorfDef = RigidBodyComponent::genFixtureDef(RigidBodyComponent::ENEMY);
	b2CircleShape actorShape = b2CircleShape();
	actorShape.m_radius = 0.5f;
	actorfDef.shape = &actorShape;
	GameObject* actor = new GameObject(scenes[currentScene], "beeBody", true, { 300,400 }, 0, 1.0f);
	auto actorFace = new GameObject(actor, "beeFace", true, { 0,0 });
	((AnimatedSprite*)actor->getSprite())->setSettings(0, 5, 0);
	actor->getSprite()->setDrawOffset(actor->getSprite()->getDestinationRectangle()->height/2 - 60);
	actor->addComponent<RigidBodyComponent>()->init(scenes[currentScene]->getCollisionManager(), actorbDef, actorfDef);
	actor->addComponent<AgentComponent>()->init(blackboard, actorFace);
	
	scenes[0]->start();
}

void Game::gameLoop()
{
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        // update Scene
        scenes[currentScene]->update();
        
        // draw Scene
        scenes[currentScene]->draw();

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		deltaTime = (float)std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
    }
}

void Game::shutdown()
{
    CloseWindow();

	for (size_t i = 0; i < scenes.size(); i++)
	{
		delete scenes[i];
	}

	delete textureManager;
}

float Game::getDeltaTime()
{
	return getInstance().deltaTime;
}

TextureManager* Game::getTextureManager()
{
	return textureManager;
}

void Game::changeScene(int sceneIndex)
{
	//move gameobjects from one scene to other IF needed (done before this function is called)
	
	//unload previous scene
	
	//load in new scene info 
	//scenes[currentScene]->start();
	//success?
}

Game& Game::getInstance()
{
	static Game instance = Game(); 
	return instance;
}
