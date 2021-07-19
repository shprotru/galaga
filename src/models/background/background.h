#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <chrono>
#include <SDL2/SDL.h>

#include "../model.h"
#include "../../assets.h"

namespace BACKGROUND {
    class background : MODEL::Basic, MODEL::IModel
    {
        double tRemForStep; // остаток времени от дельты при движении
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

        void move(double timeDelta);
        void render();
        void setInitPosition( uint16_t screenW, uint16_t screenH );
    };
}

#endif // BACKGROUND_H
