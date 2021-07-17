#ifndef GALAGA_H
#define GALAGA_H

#include <iostream>
#include <SDL2/SDL.h>

#include "assets.h"

namespace GALAGA {
    class galaga
    {
        SDL_Cursor* cursor;
        SDL_Window* gWindow;
        SDL_Renderer* gRenderer;

        bool setCursor(ASSETS::loader &ldr);
    public:
        galaga();
        ~galaga();

        void mainLoop();
    };
}

#endif // GALAGA_H
