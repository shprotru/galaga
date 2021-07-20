#include <array>
#include <chrono>

#include "models/background/background.h"
#include "config.h"
#include "misc/defer.h"
#include "models/enemy1/enemy1.h"
#include "models/enemy2/enemy2.h"
#include "galaga.h"
#include "models/spaceship/spaceship.h"

namespace GALAGA {
    const char *WINDOW_CAPTION = "Galaga4Playrix";

    enum Gstate : uint8_t {
        menu = 0,
        level_intro,
        gameplay,
        pause,
        game_over,
    };

    galaga::galaga()
    {
        // Создаём окно
        gWindow = SDL_CreateWindow(
            WINDOW_CAPTION,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
        );
        if( gWindow == nullptr )
        {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        // Создаём рендерер
        gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
        if( gRenderer == nullptr )
        {
            std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return;
        }
    }

    galaga::~galaga()
    {
        SDL_FreeCursor(cursor);
        SDL_DestroyRenderer( gRenderer );
        SDL_DestroyWindow( gWindow );
    }

    bool galaga::setCursor(ASSETS::loader &ldr) {
        SDL_Surface* srfcImage = ldr.loadSurface( ASSETS::imageID::cursor_aim );
        if( srfcImage == nullptr )
        {
            std::cout << "Failure: surface could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }

        defer(
            std::cout << "freeing cursor surface" << std::endl;
            SDL_FreeSurface( srfcImage );
        );

//      Попытка рескейлинга по мотивам https://stackoverflow.com/questions/13744053/faster-way-to-upscale-the-screen-for-a-pixelated-look-in-sdl
//        if ( SCALING_FACTOR > 1 ) {
//            SDL_Surface* srfcRescaled = SDL_CreateRGBSurface( 0, srfcImage->w * SCALING_FACTOR, srfcImage->h * SCALING_FACTOR, 24, 0, 0, 0, 0 );
//            if( srfcRescaled == nullptr )
//            {
//                std::cout << "Failure: rescaled surface could not be created! SDL Error: " << SDL_GetError() << std::endl;
//                return false;
//            }

//            #define GET_PIXEL(surface, x, y) *(Uint32 *)((Uint32 *)surface->pixels + (y * surface->w) + x)
//            for ( int i = 0; i < srfcImage->w; ++i )
//                for ( int j = 0; j < srfcImage->h; ++j ) {
//                    SDL_Rect rect = { j * SCALING_FACTOR, i * SCALING_FACTOR, SCALING_FACTOR, SCALING_FACTOR };
//                    SDL_FillRect( srfcRescaled, &rect, GET_PIXEL( srfcImage, j, i));
//                }

//            SDL_FreeSurface(srfcImage);
//            srfcImage = srfcRescaled;
//        }

        cursor = SDL_CreateColorCursor(
                    srfcImage,
                    srfcImage->w / 2,
                    srfcImage->h / 2);
        if( cursor == nullptr )
        {
            std::cout << "Failure: Cursor could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_SetCursor( cursor );

        return true;
    }

    void galaga::mainLoop()
    {
        SDL_Event ev;

        // Setup flip
        auto flip = SDL_RendererFlip( SDL_FLIP_VERTICAL );

        auto asLdr = ASSETS::loader( gRenderer );

        setCursor( asLdr );

        const long timeScalingFactor = 10000;

        auto bg = BACKGROUND::background( gRenderer, flip, asLdr );
        bg.setInitPosition( SCREEN_WIDTH, SCREEN_HEIGHT );

        auto p1 = SPACESHIP::spaceship( gRenderer, flip, asLdr );
        p1.setInitPosition( SCREEN_WIDTH, SCREEN_HEIGHT );

        static const uint8_t enAmountTopLine = 4;
        std::array<ENEMY1::enemy1 *, enAmountTopLine> enTopLine;    // enemies, верхний ряд
        for ( auto it = std::make_pair( enTopLine.begin(), 0 );
              it.first != enTopLine.end();
              ++it.first
        ) {
            *it.first = new ENEMY1::enemy1( gRenderer, flip, asLdr );
            (*it.first)->setPosition( 90 + 120 * it.second, 60 );
            (*it.first)->setID(it.second);
            it.second++;
        }

        defer(
            for ( auto it = enTopLine.begin();
                  it != enTopLine.end();
                  ++it
            ) {
                delete *it;
            }
        );

        static const uint8_t enAmountBottomLine = 5;
        std::array<ENEMY2::enemy2 *, enAmountBottomLine> enBottomLine; // enemies, нижний ряд
        for ( auto it = std::make_pair( enBottomLine.begin(), 0 );
             it.first != enBottomLine.end();
             ++it.first
        ) {
           *it.first = new ENEMY2::enemy2( gRenderer, flip, asLdr );
           (*it.first)->setPosition( 50 + 120 * it.second, 170 );
           it.second++;
        }

        defer(
           for ( auto it = enBottomLine.begin();
                 it != enBottomLine.end();
                 ++it
           ) {
               delete *it;
           }
        );

        unsigned int t = 0; // Всего прошло времени
        auto currentTime = SDL_GetTicks();

        // Флаг выхода из цикла опроса
        bool quit = false;
        // Пока программа работает
        while( !quit )
        {
            if ( SDL_PollEvent( &ev ) != 0 )
            {
                switch( ev.type ) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch( ev.key.keysym.sym )
                    {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_LEFT:
                    case SDLK_a:
                        p1.onMoveLeft();
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        p1.onMoveRight();
                        break;
                    }

                    break;
                case SDL_MOUSEMOTION:
                    p1.onMouseMove( ev.motion.x, ev.motion.y );
                    break;
                }
            }

            auto newTime = SDL_GetTicks();
            auto frameTime = newTime - currentTime; // времени пришлось на предыдущий фрейм
            currentTime = newTime;

            // Перемещения
            bg.integrate( Gstate::gameplay, t, frameTime );
            p1.integrate( Gstate::gameplay, t, frameTime );
            for ( auto it = enTopLine.begin();
                  it != enTopLine.end();
                  ++it
            ) {
                (*it)->integrate( Gstate::gameplay, t, frameTime );
            }
            for ( auto it = enBottomLine.begin();
                  it != enBottomLine.end();
                  ++it
            ) {
                (*it)->integrate( Gstate::gameplay, t, frameTime );
            }

            t += frameTime;

            // Отрисовываем
            SDL_SetRenderDrawColor( gRenderer, 111, 111, 111, 0xff );
            SDL_RenderClear( gRenderer );

            bg.render();
            p1.render();
            for ( auto it = enTopLine.begin();
                  it != enTopLine.end();
                  ++it
            ) {
                (*it)->render();
            }
            for ( auto it = enBottomLine.begin();
                  it != enBottomLine.end();
                  ++it
            ) {
                (*it)->render();
            }

            // Update screen
            SDL_RenderPresent( gRenderer );
            SDL_Delay(1);
        }
    }
}
