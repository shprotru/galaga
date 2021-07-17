#include "galaga.h"
#include "config.h"
#include "misc/defer.h"
#include "models/spaceship/spaceship.h"

namespace GALAGA {
    const char *WINDOW_CAPTION = "Galaga4Playrix";

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
        SDL_RendererFlip flip = SDL_RendererFlip( SDL_FLIP_VERTICAL );

        auto asLdr = ASSETS::loader( gRenderer );

        setCursor( asLdr );

        auto p1 = SPACESHIP::spaceship( gRenderer, flip, asLdr );
        p1.setInitPosition( SCREEN_WIDTH, SCREEN_HEIGHT );

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

            // Initialize renderer color
            SDL_SetRenderDrawColor( gRenderer, 111, 111, 111, 0xff );

            // Clear screen
            SDL_RenderClear( gRenderer );

            // SDL_SetRenderDrawColor( gRenderer, 0xff, 0xff, 0x00, 0xff );
            // SDL_Rect rect = { 0, 0, 248, 248 };
            // SDL_RenderFillRect( gRenderer, &rect );

            // Движемся
            p1.move();
            // Отрисовываемся
            p1.render();

            // Update screen
            SDL_RenderPresent( gRenderer );
        }
    }
}
