#ifndef PLAYER_H
#define PLAYER_H

#include "../model.h"

namespace SPACESHIP {
    class spaceship : MODEL::Basic, MODEL::PModel, MODEL::IModel
    {
        int limitAreaW;
        int limitAreaH;
        uint16_t spriteWidth; // ширина видимого спрайта
        uint16_t spriteHeight; // высота видимого спрайта
        double lastMouseOx;
        double lastMouseOy;
        static const uint16_t framesAmountAnimDeath = 4;
        ASSETS::spriteInfo *sprite;
        std::array<ASSETS::spriteInfo *, framesAmountAnimDeath> animDeath;
    public:
        spaceship(
            SDL_Renderer* renderer,
            SDL_RendererFlip flp,  // Нужно для рендеринга
            ASSETS::loader &ldr
        );
        ~spaceship();

        void integrate( SDL_Rect &pos, double time, double timeDelta );
        void render( SDL_Rect pos );
        void setInitPosition( uint16_t screenW, uint16_t screenH );

        void recalcAngle();
        void onMouseMove( double mouseOx, double mouseOy );
        void onMoveLeft();
        void onMoveRight();
        void onFire();
    };
}

#endif // PLAYER_H
