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

    void background::move(double timeDelta) {
        const long msPerMovement = 2000; // совершаем движение раз в x ми? секунд

//        tRemForStep += timeDelta % msPerMovement;
        const int steps = ( timeDelta + tRemForStep ) / msPerMovement;

        if ( steps == 0 )
            return;

        const long stepPoints = steps * msPerMovement;

        if ( stepPoints < tRemForStep)
            tRemForStep -= stepPoints;

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
