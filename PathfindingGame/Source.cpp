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

#define PHYSAC_IMPLEMENTATION
#include "NecessaryHeaders.h"
#include "Game.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS


int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Initialization
	//--------------------------------------------------------------------------------------
	Game& game = Game::getInstance();
	
	//The game can be initiated with a GameProperties struct with game info (it just feels cleaner than a long list of values being passed in)
	//it will use default values if this is not passed in
	GameProperties gP;
	game.init(&gP);

	game.gameLoop();

	game.shutdown();

	return 0;
}