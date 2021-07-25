#ifndef GALAGA_H
#define GALAGA_H

// #include <memory>
#include <iostream>
#include <SDL2/SDL.h>

#include "assets.h"

namespace GALAGA {
    class galaga
    {
        /*
        struct SDLCursorDestroyer
        {
            void operator()(SDL_Cursor* c) const
            {
                SDL_FreeCursor(c);
            }
        };

        std::unique_ptr<SDL_Cursor, SDLCursorDestroyer > cursor;
        */
        SDL_Cursor *cursor;
        SDL_Window *gWindow;
        SDL_Renderer *gRenderer;

        bool setCursor(ASSETS::loader &ldr);
    public:
        galaga();
        ~galaga();

        void mainLoop();
    };
}

#endif // GALAGA_H
