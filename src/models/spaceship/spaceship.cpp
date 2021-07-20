#include <iostream>

#include "spaceship.h"
#include "../../misc/defer.h"

namespace SPACESHIP {
    spaceship::spaceship (
            SDL_Renderer* renderer,
            SDL_RendererFlip flp,
            ASSETS::loader &ldr
    )
    {
        flip = flp;
        gRenderer = renderer;
        state = MODEL::Mstate::initialized;

        sprite = ldr.loadTexture( ASSETS::spaceship1 );

        for( auto it = std::make_pair( animDeath.begin(), ASSETS::spaceship_death1 ) ;
                it.first != animDeath.end();
                ++it.first )
        {
            *it.first = ldr.loadTexture(it.second);
            it.second = static_cast<ASSETS::imageID>( it.second + 1 );
        }
    }

    spaceship::~spaceship()
    {
        delete sprite;
        for( auto it = animDeath.begin(); it != animDeath.end(); ++it ) {
            delete *it;
        }
        std::cout << typeid(spaceship).name() << " sprites freed" << std::endl;
    }

    void spaceship::setInitPosition (
            uint16_t screenW,
            uint16_t screenH
    ) {
        if ( state != MODEL::Mstate::initialized ) {
            std::cout << typeid(spaceship).name() << " already initialized" << std::endl;
            return;
        }

        state = MODEL::Mstate::alive;

        const uint16_t w = sprite->width * SCALING_FACTOR;
        const uint16_t h = sprite->heigth * SCALING_FACTOR;

        halfWidth = w / 2;
        halfHeight =  h / 2;

        spriteWidth = w;
        spriteHeight = h;
        limitAreaW = screenW;
        limitAreaH = screenH;

        position = {
            screenW / 2 - halfWidth,
            screenH - h,
            w,
            h
        };
    }

    void spaceship::recalcAngle() {
        const double lenAB = std::sqrt (
            std::pow( lastMouseOx - position.x, 2 ) +
            std::pow( lastMouseOy - position.y, 2 )
        );

        const double lenBC = position.y - lastMouseOy;

        angle = std::acos( lenBC / lenAB ) * 180 / M_PI;
        if ( lastMouseOx < position.x )
            angle *= -1.0;
    }

    void spaceship::onMouseMove( double mouseOx, double mouseOy ) {
        lastMouseOx = mouseOx - halfWidth;
        lastMouseOy = mouseOy - halfHeight;

        recalcAngle();
    }

    void spaceship::onMoveLeft()
    {
        if ( state != MODEL::Mstate::alive ) {
            return;
        }

        if ( ms[ MODEL::Mdirection::right ] > 0 ) {
            ms[ MODEL::Mdirection::right ] -= SCALING_FACTOR;
        } else if ( ms[ MODEL::Mdirection::right ] < 0 ) {
            ms[ MODEL::Mdirection::right ] = 0;
        }

        ms[ MODEL::Mdirection::left ] += SCALING_FACTOR;
        recalcAngle();
    }

    void spaceship::onMoveRight()
    {
        if ( state != MODEL::Mstate::alive ) {
            return;
        }

        if ( ms[ MODEL::Mdirection::left ] > 0 ) {
            ms[ MODEL::Mdirection::left ] -= SCALING_FACTOR;
        } else if ( ms[ MODEL::Mdirection::left ] < 0 ) {
            ms[ MODEL::Mdirection::left ] = 0;
        }

        ms[ MODEL::Mdirection::right ] += SCALING_FACTOR;
        recalcAngle();
    }

    void spaceship::onFire()
    {
        if ( state != MODEL::Mstate::alive ) {
            return;
        }
    }

    // TODO: перемещение сделано отвратительно, переделать
    void spaceship::integrate( uint8_t state, unsigned int time, unsigned int timeDelta )
    {
        const long msPerMovement = 400; // двигаем на 1 пиксель раз в 2000 ми? секунд

        accumForStep += timeDelta;
        const int steps = ( timeDelta + accumForStep ) / msPerMovement;

        //        if ( steps == 0 )
        //            return;

        const long stepPoints = steps * msPerMovement;

        if ( stepPoints < accumForStep)
            accumForStep -= stepPoints;

        if ( ms[ MODEL::Mdirection::left ] >= 1 ) {
            ms[ MODEL::Mdirection::left ] -= 1;
            position.x -= ms[ MODEL::Mdirection::left ]; // ( ms[ MODEL::Mdirection::left ] / SCALING_FACTOR + 1 );
        }

        if ( ms[ MODEL::Mdirection::right ] >= 1 ) {
            ms[ MODEL::Mdirection::right ] -= 1;
            position.x += ms[ MODEL::Mdirection::right ]; // ( ms[ MODEL::Mdirection::right ] / SCALING_FACTOR + 1 );
        }

        if ( position.x < 0 ) {
            ms[ MODEL::Mdirection::right ] += ms[ MODEL::Mdirection::left ];
            ms[ MODEL::Mdirection::left ] = 0;
            position.x = 0;
        } else if ( position.x > static_cast<int>( SCREEN_WIDTH - spriteWidth ) ) {
            ms[ MODEL::Mdirection::left ] += ms[ MODEL::Mdirection::right ];
            ms[ MODEL::Mdirection::right ] = 0;
            position.x = static_cast<int>( SCREEN_WIDTH - spriteWidth );
        }
    }

    void spaceship::render()
    {
        SDL_RenderCopyEx( gRenderer, sprite->texture, nullptr, &position, angle, nullptr, flip );
    }
}
