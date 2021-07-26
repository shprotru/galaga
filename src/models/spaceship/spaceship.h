#ifndef PLAYER_H
#define PLAYER_H

#include "../model.h"
#include "../gun/gun.h"

namespace SPACESHIP {
    class spaceship : MODEL::Basic, MODEL::PModel, MODEL::IModel
    {
        int limitAreaW;
        int limitAreaH;
        uint16_t spriteWidth; // ширина видимого спрайта
        uint16_t spriteHeight; // высота видимого спрайта
        double lastMouseOx;
        double lastMouseOy;
        long tRemForStep; // остаток времени от дельты при движении
        static const uint16_t framesAmountAnimDeath = 4;
        ASSETS::spriteInfo *sprite;
        std::array<ASSETS::spriteInfo *, framesAmountAnimDeath> animDeath;
        GUN::gun gun;
    public:
        spaceship(
            SDL_Renderer* renderer,
            SDL_RendererFlip flp,  // Нужно для рендеринга
            ASSETS::loader &ldr
        );
        ~spaceship();

        void move(double timeDelta);
        void render();
        void setInitPosition( uint16_t screenW, uint16_t screenH );

        void recalcAngle();
        void onMouseMove( double mouseOx, double mouseOy );
        void onMoveLeft();
        void onMoveRight();
        void onFire();
    };
}

#endif // PLAYER_H
