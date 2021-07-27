/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "NecessaryHeaders.h"
#include "Game.h"
#include <filesystem>
#include <chrono>

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS


int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// Initialization
	//--------------------------------------------------------------------------------------
	srand(time(0));

	Game& game = Game::getInstance();
	//The game can be initiated with a GameProperties struct with game info (it just feels cleaner than a long list of values being passed in)
	//it will use default values if this is not passed in
	GameProperties gP;
	//sprite directory found using filesystem
	auto sprDirectory = std::filesystem::weakly_canonical(std::filesystem::path(argv[0])).parent_path().parent_path().parent_path().string();
	gP.spriteLocation = sprDirectory + "\\Images";
	gP.shaderLocation = sprDirectory + "\\Shaders";
	game.init(&gP);

	game.gameLoop();

	game.shutdown();

	return 0;
}