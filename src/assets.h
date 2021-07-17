#ifndef ASSETS_H
#define ASSETS_H

#include <tuple>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>

#include "misc/format_bmp.h"

extern const unsigned char gameAssets[];
extern const unsigned char gameBg[];

namespace ASSETS {
    enum imageID : uint16_t {
        spaceship1 = 0,
        spaceship2,

        spaceship_death1,
        spaceship_death2,
        spaceship_death3,
        spaceship_death4,

        enemy1_frame1,
        enemy1_frame2,

        enemy2_frame1,
        enemy2_frame2,

        enemy3_frame1,
        enemy3_frame2,

        enemy4_frame1,
        enemy4_frame2,

        enemy_death1,
        enemy_death2,
        enemy_death3,
        enemy_death4,
        enemy_death5,

        bullet_bottom_top,
        bullet_top_bottom,

        cursor_aim,
        icon_clock,

        unexisting,
    };

    enum axisWH : uint16_t {
        idOX = 0,
        idOY,
        idXW,
        idYH,
    };

    //                                           x         y       x+w       y+h
    static const std::vector< std::tuple <uint16_t, uint16_t, uint16_t, uint16_t> > asset_info = {
        std::make_tuple( 109, 1,  124, 17), // spaceship1
        std::make_tuple( 109, 19, 124, 35), // spaceship2

        std::make_tuple( 145, 1, 177, 33), // spaceship_death1
        std::make_tuple( 179, 1, 211, 33), // spaceship_death2
        std::make_tuple( 213, 1, 245, 33), // spaceship_death3
        std::make_tuple( 247, 1, 279, 33), // spaceship_death4

        std::make_tuple( 110, 37, 125, 53), // enemy1_frame1
        std::make_tuple( 128, 37, 143, 53), // enemy1_frame2

        std::make_tuple( 110, 55, 125, 71), // enemy2_frame1
        std::make_tuple( 128, 55, 143, 71), // enemy2_frame2

        std::make_tuple( 111, 76, 124, 86), // enemy3_frame1
        std::make_tuple( 129, 76, 142, 86), // enemy3_frame2

        std::make_tuple( 111, 94, 124, 104), // enemy4_frame1
        std::make_tuple( 129, 94, 142, 104), // enemy4_frame2

        std::make_tuple( 289, 1, 321, 33), // enemy_death1
        std::make_tuple( 323, 1, 355, 33), // enemy_death2
        std::make_tuple( 357, 1, 389, 33), // enemy_death3
        std::make_tuple( 391, 1, 423, 33), // enemy_death4
        std::make_tuple( 425, 1, 457, 33), // enemy_death5

        std::make_tuple( 313, 122, 316, 130), // bullet_bottom_top
        std::make_tuple( 313, 140, 316, 148), // bullet_top_bottom

        std::make_tuple( 126, 1, 140, 15),    // cursor_aim
        std::make_tuple( 409, 166, 456, 213), // icon_clock
    };

    struct spriteInfo {
        SDL_Texture *texture;
        uint16_t width;
        uint16_t heigth;
        spriteInfo(SDL_Texture *t, uint16_t w, uint16_t h);
        ~spriteInfo();
    };

    class loader
    {
        static const uint8_t BYTES_PER_PIXEL = 4;
        static const uint8_t PIXEL_ENC_AMOUNT = 3;

        BITMAPFILEHEADER *bmpHeader;
        BITMAPINFOHEADER *dibHeader;

        SDL_Renderer* gRenderer;
        SDL_Surface *srfcAssets; // главный ассет из которого будем копировать спрайты
        SDL_Surface *srfcBg;     // фон
    public:
        loader(SDL_Renderer* renderer);
        ~loader();
        spriteInfo *loadTexture( imageID id );
        spriteInfo *loadBackground();
        // loadSurface - не забыть после использования высвободить surface
        SDL_Surface *loadSurface( imageID id );
    };
}

#endif // ASSETS_H
