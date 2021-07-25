#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <chrono>
#include <SDL2/SDL.h>

#include "../model.h"
#include "../../assets.h"

namespace BACKGROUND {
    class background : MODEL::Basic, MODEL::IModel
    {
        int assetHeight;  // высота изображения
        static const uint16_t framesAmountCntBg = 2;
        //                                       si       pos
        std::array< std::pair< ASSETS::spriteInfo *, SDL_Rect >, framesAmountCntBg > sprites;
    public:
        background(
            SDL_Renderer* renderer,
            SDL_RendererFlip flp,
            ASSETS::loader &ldr
        );
        ~background();

        void integrate( SDL_Rect &pos, double time, double timeDelta );
        void render( SDL_Rect pos );
        void setInitPosition( uint16_t screenW, uint16_t screenH );
    };
}

#endif // BACKGROUND_H
