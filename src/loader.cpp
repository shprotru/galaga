#include <climits> // CHAR_BIT
#include <cstring> // memcpy
#include <iostream>

#include "assets.h"
#include "misc/defer.h"

namespace ASSETS {
    loader::loader (
            SDL_Renderer* renderer
    ) {
        gRenderer = renderer;
        bmpHeader = const_cast< BITMAPFILEHEADER * > ( reinterpret_cast< const BITMAPFILEHEADER * > ( gameAssets ) );
        dibHeader = const_cast< BITMAPINFOHEADER * > ( reinterpret_cast< const BITMAPINFOHEADER * >( &gameAssets[ sizeof( BITMAPFILEHEADER ) ] ) );

        if ( dibHeader->biCompression != 0 ) {
            std::cout << "Failure: image contains compression" << std::endl;
            return;
        }

        if ( dibHeader->biBitCount != 24 ) {
            std::cout << "Failure: assets loader supports only 24bpp images" << std::endl;
            return;
        }

        auto assetFormat = dibHeader->biBitCount / CHAR_BIT;

        auto assetPitch = assetFormat * dibHeader->biWidth +             // pitch
            ( assetFormat * dibHeader->biWidth ) % BYTES_PER_PIXEL; // padding

//        assetPitch = assetFormat * dibHeader->biWidth +             // pitch
//            ( (BYTES_PER_PIXEL - ( dibHeader->biWidth * assetFormat ) % BYTES_PER_PIXEL) & PIXEL_ENC_AMOUNT ); // padding

//        assetPitch = ( dibHeader->biWidth * assetFormat + assetFormat) & // pitch
//                    (~assetFormat);                                      // padding

        srfcAssets = SDL_CreateRGBSurfaceWithFormatFrom (
            (unsigned char *)&gameAssets[bmpHeader->bfOffBits],
            dibHeader->biWidth,
            dibHeader->biHeight,
            dibHeader->biBitCount,
            assetPitch,
            SDL_PIXELFORMAT_BGR24
        );
        if ( srfcAssets == nullptr )
        {
            std::cout << "Failure: assets surface could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return;
        }

//        std::cout << bmpHeader->bfOffBits << std::endl;
//        std::cout << uint32_t(gameAssets[FBMP_HEADER_BID]) << std::endl;
//        std::printf("%0x\n", dibHeader->biRedMask);
    }

    loader::~loader() {
        defer (
            if ( srfcAssets != nullptr ) {
                std::cout << "freeing " << typeid(loader).name() << " assets surface" << std::endl;
                SDL_FreeSurface( srfcAssets );
            }
        );
    }

    spriteInfo *loader::loadBackground() {
        BITMAPFILEHEADER * bmpHdr = const_cast< BITMAPFILEHEADER * > ( reinterpret_cast< const BITMAPFILEHEADER * > ( gameBg ) );
        BITMAPINFOHEADER * dibHdr = const_cast< BITMAPINFOHEADER * > ( reinterpret_cast< const BITMAPINFOHEADER * >( &gameBg[ sizeof( BITMAPFILEHEADER ) ] ) );

        if ( dibHdr->biCompression != 0 ) {
            std::cout << "Failure: image contains compression" << std::endl;
            return nullptr;
        }

        auto assetFormat = dibHdr->biBitCount / CHAR_BIT;

        auto assetPitch = assetFormat * dibHdr->biWidth +             // pitch
            ( assetFormat * dibHdr->biWidth ) % BYTES_PER_PIXEL; // padding

//        assetPitch = assetFormat * dibHeader->biWidth +             // pitch
//            ( (BYTES_PER_PIXEL - ( dibHeader->biWidth * assetFormat ) % BYTES_PER_PIXEL) & PIXEL_ENC_AMOUNT ); // padding

//        assetPitch = ( dibHeader->biWidth * assetFormat + assetFormat) & // pitch
//                    (~assetFormat);                                      // padding

        srfcBg = SDL_CreateRGBSurfaceWithFormatFrom (
            (unsigned char *)&gameBg[bmpHdr->bfOffBits],
            dibHdr->biWidth,
            dibHdr->biHeight,
            dibHdr->biBitCount,
            assetPitch,
            SDL_PIXELFORMAT_BGR24
        );
        if ( srfcBg == nullptr )
        {
            std::cout << "Failure: bg surface could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        defer(
            std::cout << "freeing " << typeid(spriteInfo).name() << " bg surface" << std::endl;
            SDL_FreeSurface( srfcBg );
        );

        SDL_Texture *txtreBg = SDL_CreateTextureFromSurface( gRenderer, srfcBg );
        if( txtreBg == nullptr )
        {
            std::cout <<  "Unable to create texture from asset! SDL Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        return new spriteInfo( txtreBg, srfcBg->w, srfcBg->h );
    }

    SDL_Surface *loader::loadSurface (
            imageID id
    ) {
        if ( id >=  unexisting ) {
            std::cout <<  "Failure: request for unexisting image " << id << std::endl;
            return nullptr;
        }

        const uint16_t asWidth = std::get<idXW>( asset_info[id] ) - std::get<idOX>( asset_info[id] );
        const uint16_t asHeigth = std::get<idYH>( asset_info[id] ) - std::get<idOY>( asset_info[id] );

        //        const uint16_t imgPitch = assetFormat * asWidth + // pitch
        //                asWidth % BYTES_PER_PIXEL; // padding
        //        const uint16_t imgPitch = ( asWidth * assetFormat + assetFormat) & // pitch
        //                ( ~assetFormat ); // padding

        //        unsigned char *imgData = ( unsigned char * ) malloc( asHeigth * imgPitch );
        //        defer( free( imgData ) );

        //        for ( uint16_t i = 0; i < asHeigth; ++i ) {
        //            const uint32_t offX = std::get<idOX>( asset_info[id] ) * assetFormat;
        //            const uint32_t offY = assetPitch * ( std::get<idOY>( asset_info[id] ) + i );
        //            const uint32_t off = bmpHeader->bfOffBits +                           // начало данных о пикселях
        //                    offX +        // смещение по OX
        //                    offY ; // смещение по OY

        //            memcpy(
        //                    (unsigned char *)&imgData[i * asWidth],
        //                    (unsigned char *)&gameAssets[ off ],
        //                    imgPitch
        //            );
        //        }

        SDL_Surface* srfcImage = SDL_CreateRGBSurface( 0, asWidth, asHeigth, dibHeader->biBitCount, 0, 0, 0, 0 );
        if( srfcImage == nullptr )
        {
            std::cout << "Failure: surface could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        SDL_Rect rect = {
            std::get<idOX>( asset_info[id] ),
            int ( dibHeader->biHeight - std::get<idOY>( asset_info[id] ) - asHeigth ),
            asWidth,
            asHeigth
        };
        if ( SDL_BlitSurface( srfcAssets, &rect, srfcImage, nullptr) != 0 )
        {
            std::cout << "Failure: surface could not be blitted! SDL Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        if( SDL_SetColorKey( srfcImage, SDL_TRUE, SDL_MapRGB( srfcImage->format, 0, 0, 0 ) ) != 0 )
        {
            std::cout << "Failure: set surface color key! SDL Error: " << SDL_GetError() << std::endl;
        }

        return srfcImage;
    }

    spriteInfo *loader::loadTexture (
            imageID id
    ) {
        if ( id >=  unexisting ) {
            std::cout <<  "Failure: request for unexisting image " << id << std::endl;
            return nullptr;
        }

        const uint16_t asWidth = std::get<idXW>( asset_info[id] ) - std::get<idOX>( asset_info[id] );
        const uint16_t asHeigth = std::get<idYH>( asset_info[id] ) - std::get<idOY>( asset_info[id] );

        SDL_Surface* srfcImage = loadSurface( id );
        if ( srfcImage == nullptr ) {
            std::cout << "Failure: surface could not be loaded!" << std::endl;
            return nullptr;
        }

        defer(
            std::cout << "freeing " << typeid(spriteInfo).name() << " image surface" << std::endl;
            SDL_FreeSurface( srfcImage );
        );

        SDL_Texture *txtreAssets = SDL_CreateTextureFromSurface( gRenderer, srfcImage );
        if( txtreAssets == nullptr )
        {
            std::cout <<  "Unable to create texture from asset! SDL Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        return new spriteInfo( txtreAssets, asWidth, asHeigth );
    }
}
