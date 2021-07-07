#include "Window.h"
#include "../Game/Game.h"

void DrawWindow() {
	//init game
	Game game;

	//Game Loop 
	while (game.GetWindowIsOpen())
	{
		//Update
		game.Update();

		//Render
		game.Render();
	}
}