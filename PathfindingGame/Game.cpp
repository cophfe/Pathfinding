#include "Game.h"

#include "Scene.h"
#include "Room.h"
#include "GameObject.h"
#include "Transform.h"

#include "AgentComponent.h"
#include "AgentDataComponent.h"
#include "PlayerComponent.h"
#include "PathfinderComponent.h"
#include "ButtonComponent.h"
#include "DeathUIComponent.h"

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

	timeAtStart = time(0);
	timeScale = 1;
	
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
	textureManager->loadTexturesFromFolder(properties->spriteLocation);
	textureManager->loadShadersFromFolder(properties->shaderLocation);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	   Generate Rooms
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	roomManager = new RoomManager(time(0));
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 	   Create Menu Scene
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	scene = createMenuScene();
	
	//	Start
	scene->start();
}

void Game::gameLoop()
{
    while (!checkWindowShouldClose())    // Detect window close button or close key
    {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        // update Scene
		scene->update();
	
		// do fixedUpdate (only happens every PHYSICS_TIME_STEP)
		scene->fixedUpdate();
        
        // draw Scene
		BeginDrawing();
		scene->draw();
		EndDrawing();

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		deltaTime = (float)std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

		if (deltaTime > maxDeltaTime)
		{
			deltaTime = maxDeltaTime;
		}
		//have to wait until end of gameLoop to delete scene
		//this happens independant of deltaTime
		if (nextScene != nullptr)
		{
			scene->beforeDelete(nextScene);
			delete scene;
			scene = nextScene;
			scene->start();
			nextScene = nullptr;
		}
    }
}

void Game::shutdown()
{
    CloseWindow();
	delete scene;

	delete roomManager;
	delete textureManager;
}

void Game::switchScene(Scene* newScene)
{
	nextScene = newScene;
}

float Game::getDeltaTime()
{
	return getInstance().deltaTime * getInstance().timeScale;
}

float Game::getUnscaledDeltaTime()
{
	return getInstance().deltaTime;
}

TextureManager* Game::getTextureManager()
{
	return textureManager;
}

Game& Game::getInstance()
{
	//constructor is only called once
	static Game instance = Game(); 
	return instance;
}

Scene* Game::createMenuScene()
{
	//create scene
	Scene* menuScene = new Scene();
	SceneProperties sceneProperties;
	sceneProperties.randomSeed = time(0);
	menuScene->load(&sceneProperties);
	menuScene->setBackground(RAYWHITE);
	//make escape key exit game
	SetExitKey(KEY_ESCAPE);

	//Create buttons
	int buttonWidth = textureManager->getTextureInfo("sideButton")->width;
	auto enter = (new GameObject(menuScene, "sideButton", true, { (buttonWidth / 2 - 10) * 0.6f , (float)GetScreenHeight() / 2 + 50 }, 0, 0.6f, SORTING::UI))->addComponent<ButtonComponent>();
	enter->init("PLAY", WHITE, 100, 50);
	enter->onRelease = [](ButtonComponent* comp, void* ptr)
	{
		Game& game = Game::getInstance();
		auto rM = game.getRoomManager();
		if (rM->checkMapHasBeenGenerated())
		{
			rM->generateNewMap(rand(), false);
			game.switchScene(rM->createFirstRoom());
		}
		else
		{
			rM->generateMap();
			game.switchScene(rM->createFirstRoom());
		}

	};
	auto exit = (new GameObject(menuScene, "sideButton", true, { (buttonWidth / 2 - 10) * 0.6f , (float)GetScreenHeight() / 2 + 200 }, 0, 0.6f, SORTING::UI))->addComponent<ButtonComponent>();
	exit->init("EXIT", WHITE, 100, 50);
	exit->onRelease = [](ButtonComponent* comp, void* ptr)
	{
		Game::getInstance().endGameLoop();
	};

	//create moving tiled background
	menuScene->addGameObject(new GameObject(new CustomRenderSprite([](void* ptr)
		{
			static Texture2D* backgroundTexture = Game::getInstance().getTextureManager()->getTextureInfo("background");
			static Rectangle srcRect = Rectangle{ 0,0, (float)backgroundTexture->width, (float)backgroundTexture->height };
			static float timer = 0;
			timer += Game::getDeltaTime() * 10;
			float timerX = fmodf(timer, srcRect.width);
			float timerY = fmodf(timer / 2, srcRect.height);
			DrawTextureTiled(*backgroundTexture, srcRect, Rectangle{ -srcRect.width, -srcRect.height, (float)GetScreenWidth() + srcRect.width * 2, (float)GetScreenHeight() + srcRect.height * 2 }, { -timerX, -timerY }, 0, 1, { 0xD0,0xD0,0xD0,0xFF });

		}, nullptr)), SORTING::UI);

	return menuScene;
}

Scene* Game::createDeathScene(int floorCount)
{
	Scene* deathScene = new Scene();

	SceneProperties sceneProperties;
	sceneProperties.randomSeed = time(0);
	deathScene->load(&sceneProperties);

	//Create death ui object
	//it takes care of all ui
	(new GameObject(deathScene, "pauseMenu", true, { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 0, 1, SORTING::UI))->addComponent<DeathUIComponent>()->init(floorCount, deathScene);

	return deathScene;
}
