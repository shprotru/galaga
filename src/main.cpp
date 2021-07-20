#include <iostream>
#include <SDL2/SDL.h>

#include "galaga.h"

int main() {
	if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
	}

    GALAGA::galaga game;
    game.mainLoop();

	SDL_Quit();

    return EXIT_SUCCESS;
}
