#include <SDL.h>
#include "Game.h"

#undef main

int main(int argc, char* argv[])
{

	SDL_Init(SDL_INIT_EVERYTHING);

	int flags = IMG_INIT_PNG; 
	int initted = IMG_Init(flags);

	SDL_Window* window = SDL_CreateWindow(WINDOW_ARGS);

	Game game(window);

	SDL_Event e;
	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	Move move;
	bool quit = false;
	bool shoot = false;

	while (!quit)
	{
		SDL_PollEvent(&e);
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
		else
		{
			shoot = false;

			if (keystates[SDL_SCANCODE_UP])
				move = UP;

			else if (keystates[SDL_SCANCODE_DOWN])
				move = DOWN;

			else if (keystates[SDL_SCANCODE_LEFT])
				move = LEFT;

			else if (keystates[SDL_SCANCODE_RIGHT])
				move = RIGHT;

			if (keystates[SDL_SCANCODE_SPACE])
				shoot = true;
			
			game.update(move, shoot);
			move.reset();
		}
		Delay();
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
