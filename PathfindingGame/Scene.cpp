#include "Scene.h"

void Scene::load()
{
}

void Scene::draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}

void Scene::update()
{
	
}

void Scene::unload()
{
}
