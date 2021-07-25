#include <iostream>

#include "enemy1.h"

namespace ENEMY1 {
    enemy1::enemy1 (
        SDL_Renderer* renderer,
        SDL_RendererFlip flp,  // Нужно для рендеринга
        ASSETS::loader &ldr
    )
    {
        flip = flp;
        gRenderer = renderer;
        state = MODEL::Mstate::initialized;

        for ( auto it = std::make_pair( animWalk.begin(), ASSETS::enemy1_frame1 ) ;
                it.first != animWalk.end();
                ++it.first )
        {
            *it.first = ldr.loadTexture(it.second);
            it.second = static_cast<ASSETS::imageID>( it.second + 1 );
        }

        for ( auto it = std::make_pair( animDying.begin(), ASSETS::enemy_death1 ) ;
                it.first != animDying.end();
                ++it.first )
        {
            *it.first = ldr.loadTexture(it.second);
            it.second = static_cast<ASSETS::imageID>( it.second + 1 );
        }
    }

    enemy1::~enemy1()
    {
        for ( auto it = animWalk.begin(); it != animWalk.end(); ++it ) {
            delete *it;
        }
        std::cout << typeid(enemy1).name() << " walk sprites freed" << std::endl;

        for ( auto it = animDying.begin(); it != animDying.end(); ++it ) {
            delete *it;
        }
        std::cout << typeid(enemy1).name() << " dying sprites freed" << std::endl;
    }

    void enemy1::setInitPosition (
            uint16_t /* screenW */,
            uint16_t /* screenH */
    ) {
        if ( state != MODEL::Mstate::initialized ) {
            std::cout << typeid(enemy1).name() << " already initialized" << std::endl;
            return;
        }
    }

    void enemy1::setPosition (
            uint16_t posX,
            uint16_t posY
    ) {
        currAnimFrame = 1; // для инициализации
        state = MODEL::Mstate::alive;

        const uint16_t w = animWalk[currAnimFrame]->width * SCALING_FACTOR;
        const uint16_t h = animWalk[currAnimFrame]->heigth * SCALING_FACTOR;

        halfWidth = w / 2;
        halfHeight =  h / 2;

        position = {
            posX - halfWidth,
            posY - halfHeight,
            w,
            h
        };
    }

    void enemy1::integrate( SDL_Rect & /* pos */, double time, double timeDelta )
    {
        if ( state == MODEL::Mstate::initialized ) {
            return;
        }

        static const unsigned int msPerMovement = 1000; // совершаем движение раз в 1.5 секунды

//        accumForStep += timeDelta;
//        const unsigned int steps = accumForStep / msPerMovement;
//        if ( steps == 0 )
//            return;

//        const unsigned int stepPoints = steps * msPerMovement;

//        if ( stepPoints <= accumForStep)
//            accumForStep -= stepPoints;

//        currAnimFrame++;
//        if ( currAnimFrame == anim_amount_frames )
//            currAnimFrame = 0;
        currAnimFrame = static_cast<int>( time / 10 ) % 2;

        switch ( state ) {
        case MODEL::Mstate::alive:
            if ( ms[ MODEL::Mdirection::left ] >= 1 ) {
                ms[ MODEL::Mdirection::left ] -= 1;
                position.x -= SCALING_FACTOR; // ( ms[ MODEL::Mdirection::left ] / SCALING_FACTOR + 1 );
            }

            if ( ms[ MODEL::Mdirection::right ] >= 1 ) {
                ms[ MODEL::Mdirection::right ] -= 1;
                position.x += SCALING_FACTOR; // ( ms[ MODEL::Mdirection::right ] / SCALING_FACTOR + 1 );
            }

            break;
        case MODEL::Mstate::appearance:
            break;
        case MODEL::Mstate::dying:
            break;
        default:
            break;
        }
    }

    void enemy1::render( SDL_Rect /* pos */ )
    {
        switch ( state ) {
        case MODEL::Mstate::appearance:
        case MODEL::Mstate::alive:
            SDL_RenderCopyEx( gRenderer, animWalk[currAnimFrame]->texture, nullptr, &position, angle, nullptr, flip );
            break;
        case MODEL::Mstate::dying:
            SDL_RenderCopyEx( gRenderer, animDying[currAnimFrame]->texture, nullptr, &position, angle, nullptr, flip );
            break;
        default:
            break;
        }
    }
}
