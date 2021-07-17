#ifndef FORMAT_BMP_H
#define FORMAT_BMP_H

#include <stdint.h>

// CIEXYZTRIPLE stuff
typedef uint32_t FXPT2DOT30;

#pragma pack(push, 1)
typedef struct {
    FXPT2DOT30 ciexyzX;
    FXPT2DOT30 ciexyzY;
    FXPT2DOT30 ciexyzZ;
} CIEXYZ;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    CIEXYZ  ciexyzRed;
    CIEXYZ  ciexyzGreen;
    CIEXYZ  ciexyzBlue;
} CIEXYZTRIPLE;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push, 1)
// bitmap info header
typedef struct {
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t  biPlanes;
    uint16_t  biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
    uint32_t biRedMask;
    uint32_t biGreenMask;
    uint32_t biBlueMask;
    uint32_t biAlphaMask;
    uint32_t biCSType;
    CIEXYZTRIPLE biEndpoints;
    uint32_t biGammaRed;
    uint32_t biGammaGreen;
    uint32_t biGammaBlue;
    uint32_t biIntent;
    uint32_t biProfileData;
    uint32_t biProfileSize;
    uint32_t biReserved;
} BITMAPINFOHEADER;
#pragma pack(pop)

const uint32_t FBMP_HEADER_BID = 0x0a; // offset for bitmap image data
const uint32_t FBMP_HEADER_DIB = 0x0e; // offset for DIB

#endif // FORMAT_BMP_H
