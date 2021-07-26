#include <iostream>

#include "enemy2.h"

namespace ENEMY2 {
    enemy2::enemy2 (
        SDL_Renderer* renderer,
        SDL_RendererFlip flp,  // Нужно для рендеринга
        ASSETS::loader &ldr
    ) : gun(&angle, GUN::Gtype::player)
    {
        flip = flp;
        gRenderer = renderer;
        state = MODEL::Mstate::initialized;

        for( auto it = std::make_pair( animWalk.begin(), ASSETS::enemy3_frame1 ) ;
                it.first != animWalk.end();
                ++it.first )
        {
            *it.first = ldr.loadTexture(it.second);
            it.second = static_cast<ASSETS::imageID>( it.second + 1 );
        }

        for( auto it = std::make_pair( animDying.begin(), ASSETS::enemy_death1 ) ;
                it.first != animDying.end();
                ++it.first )
        {
            *it.first = ldr.loadTexture(it.second);
            it.second = static_cast<ASSETS::imageID>( it.second + 1 );
        }
    }

    enemy2::~enemy2()
    {
        for( auto it = animWalk.begin(); it != animWalk.end(); ++it ) {
            delete *it;
        }
        std::cout << typeid(enemy2).name() << " walk sprites freed" << std::endl;

        for( auto it = animDying.begin(); it != animDying.end(); ++it ) {
            delete *it;
        }
        std::cout << typeid(enemy2).name() << " dying sprites freed" << std::endl;
    }

    void enemy2::setInitPosition (
            uint16_t /* screenW */,
            uint16_t /* screenH */
    ) {
        if ( state != MODEL::Mstate::initialized ) {
            std::cout << typeid(enemy2).name() << " already initialized" << std::endl;
            return;
        }
    }

    void enemy2::setPosition (
            uint16_t posX,
            uint16_t posY
    ) {
        tRemForStep = 0.0;
        currAnimFrame = 1; // для инициализации
        state = MODEL::Mstate::alive;

        const uint16_t w = animWalk[currAnimFrame]->width * SCALING_FACTOR;
        const uint16_t h = animWalk[currAnimFrame]->heigth * SCALING_FACTOR;

        halfWidth = w / 2;
        halfHeight =  h / 2;

        movementLimit = ( SCREEN_WIDTH - w * 4 ) / ( 2 * SCALING_FACTOR );
        ms[MODEL::Mdirection::right] = movementLimit; // 50

        position = {
            posX - halfWidth,
            posY - halfHeight,
            w,
            h
        };
    }

    void enemy2::move(double timeDelta)
    {
        if ( state == MODEL::Mstate::initialized ) {
            return;
        }

        static const double msPerMovement = 1.0; // совершаем движение раз секунду

        tRemForStep += timeDelta;
        const long steps = tRemForStep / msPerMovement;
        if ( steps == 0 )
            return;

        const long stepPoints = steps * msPerMovement;

        if ( stepPoints <= tRemForStep)
            tRemForStep -= stepPoints;

        currAnimFrame++;

        switch ( state ) {
        case MODEL::Mstate::alive:
            if (ms[MODEL::Mdirection::left] >= 1) {
                ms[MODEL::Mdirection::left] -= 1;
                position.x -= SCALING_FACTOR; // ( ms[ MODEL::Mdirection::left ] / SCALING_FACTOR + 1 );
                if( ms[MODEL::Mdirection::left] == 0 || ms[MODEL::Mdirection::left] == 1 ) {
                    ms[MODEL::Mdirection::right] = movementLimit;
                }
            }

            if (ms[MODEL::Mdirection::right] >= 1) {
                ms[MODEL::Mdirection::right] -= 1;
                position.x += SCALING_FACTOR; // ( ms[ MODEL::Mdirection::right ] / SCALING_FACTOR + 1 );
                if( ms[MODEL::Mdirection::right] == 0 || ms[MODEL::Mdirection::right] == 1 ) {
                    ms[MODEL::Mdirection::left] = movementLimit;
                }
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

    void enemy2::render()
    {
        switch ( state ) {
        case MODEL::Mstate::appearance:
        case MODEL::Mstate::alive:
            SDL_RenderCopyEx( gRenderer, animWalk[currAnimFrame % 2]->texture, nullptr, &position, angle, nullptr, flip );
            break;
        case MODEL::Mstate::dying:
            SDL_RenderCopyEx( gRenderer, animDying[currAnimFrame]->texture, nullptr, &position, angle, nullptr, flip );
            break;
        default:
            break;
        }
    }
}
