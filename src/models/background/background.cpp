#include "background.h"
#include "../../config.h"

namespace BACKGROUND {
    background::background (
        SDL_Renderer* renderer,
        SDL_RendererFlip flp,
        ASSETS::loader &ldr
    )
    {
        flip = flp;
        gRenderer = renderer;

        for( auto it = sprites.begin();
                it != sprites.end();
                ++it )
        {
            (*it).first = ldr.loadBackground();
        }
    }

    background::~background()
    {

    }

    void background::setInitPosition (
            uint16_t screenW,
            uint16_t /* screenH */
    ) {
        tRemForStep = 0;
        double windowSF = ( double ) screenW / ( double ) sprites[0].first->width; // window scaling factor
        const int w = sprites[0].first->width * windowSF;
        assetHeight = sprites[0].first->heigth * windowSF;
        // Расставляем на начальные позиции
        sprites[0].second = SDL_Rect {
                0,
                0,
                w,
                assetHeight
        };
        sprites[1].second = SDL_Rect {
                0,
                -assetHeight,
                w,
                assetHeight
        };
    }

    void background::move(long timeDelta) {
        const long onePxPer100ms = 2000; // совершаем движение раз в x ми? секунд

        tRemForStep += timeDelta % onePxPer100ms;
        const int steps = ( timeDelta + tRemForStep ) / onePxPer100ms;

        if ( steps == 0 )
            return;

        if ( steps * onePxPer100ms < tRemForStep)
            tRemForStep -= steps * onePxPer100ms;

        for( auto it = sprites.begin();
                it != sprites.end();
                ++it )
        {
            if ( (*it).second.y >= assetHeight ) {
                (*it).second.y = -assetHeight;
            }

            (*it).second.y += steps;
        }
    }

    void background::render() {
        for( auto it = sprites.begin();
                it != sprites.end();
                ++it )
        {
            SDL_RenderCopyEx( gRenderer, (*it).first->texture, nullptr, &(*it).second, angle, nullptr, flip );
        }
    }
}
