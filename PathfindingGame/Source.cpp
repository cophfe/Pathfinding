//MADE WITH RAYLIB, BOX2D, AND NLOHMANN'S JSON PARSER

#include "NecessaryHeaders.h"
#include "Game.h"
#include <filesystem>
#include <chrono>

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	srand(time(0));

	Game& game = Game::getInstance();
	//The game can be initiated with a GameProperties struct with game info (it just feels cleaner than a long list of values being passed in)
	//it will use default values if this is not passed in
	GameProperties gP;
	
	//sprite directory found using filesystem
#ifdef _DEBUG
	auto sprDirectory = std::filesystem::weakly_canonical(std::filesystem::path(argv[0])).parent_path().parent_path().parent_path().string();
#else
	auto sprDirectory = std::filesystem::weakly_canonical(std::filesystem::path(argv[0])).parent_path().string();
#endif // DEBUG

	gP.spriteLocation = sprDirectory + "\\Images";
	gP.shaderLocation = sprDirectory + "\\Shaders";
	game.init(&gP);

	game.gameLoop();

	game.shutdown();

	return 0;
}