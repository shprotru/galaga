#include <iostream>

#include "spaceship.h"
#include "../../config.h"
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

    void spaceship::setInitPosition(
            uint16_t screen_w,
            uint16_t screen_h
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

        position = {
            screen_w / 2 - halfWidth,
            screen_h - h,
            w,
            h
        };
    }

    void spaceship::onMouseMove( double mouseOx, double mouseOy ) {
        mouseOx -= halfWidth;
        mouseOy -= halfHeight;

        const double lenAB = std::sqrt (
            std::pow( mouseOx - position.x, 2 ) +
            std::pow( mouseOy - position.y, 2 )
        );

        const double lenBC = position.y - mouseOy;

        angle = std::acos( lenBC / lenAB ) * 180 / M_PI;
        if ( mouseOx < position.x )
            angle *= -1.0;
    }

    void spaceship::onMoveLeft()
    {
        if ( state != MODEL::Mstate::alive ) {
            return;
        }

        if ( ms[ MODEL::Mdirection::right ] > 0 ) {
            ms[ MODEL::Mdirection::right ] -= 1;
        } else if ( ms[ MODEL::Mdirection::right ] < 0 ) {
            ms[ MODEL::Mdirection::right ] = 0;
        }

        ms[ MODEL::Mdirection::left ] += SCALING_FACTOR;
    }

    void spaceship::onMoveRight()
    {
        if ( state != MODEL::Mstate::alive ) {
            return;
        }

        if ( ms[ MODEL::Mdirection::left ] > 0 ) {
            ms[ MODEL::Mdirection::left ] -= 1;
        } else if ( ms[ MODEL::Mdirection::left ] < 0 ) {
            ms[ MODEL::Mdirection::left ] = 0;
        }

        ms[ MODEL::Mdirection::right ] += SCALING_FACTOR;
    }

    void spaceship::onFire()
    {
        if ( state != MODEL::Mstate::alive ) {
            return;
        }
    }

    void spaceship::move()
    {
        if ( ms[ MODEL::Mdirection::left ] >= 1 ) {
            ms[ MODEL::Mdirection::left ] -= 1;
            position.x -= 1; // ( ms[ MODEL::Mdirection::left ] / SCALING_FACTOR + 1 );
        }

        if ( ms[ MODEL::Mdirection::right ] >= 1 ) {
            ms[ MODEL::Mdirection::right ] -= 1;
            position.x += 1; // ( ms[ MODEL::Mdirection::right ] / SCALING_FACTOR + 1 );
        }
    }

    void spaceship::render()
    {
        SDL_RenderCopyEx( gRenderer, sprite->texture, nullptr, &position, angle, nullptr, flip );
    }
}
