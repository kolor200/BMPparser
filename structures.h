#ifndef ALICIA_STRUCTURES_H
#define ALICIA_STRUCTURES_H
#include <stdint.h>

typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;

#pragma pack(push, 1) //ensure that the compiler does not insert padding into the data and each member follows the previous one.

typedef struct tagBITMAPFILEHEADER {
    WORD bfType;  //specifies the file type
    DWORD bfSize;  //specifies the size in bytes of the bitmap file
    WORD bfReserved1;  //reserved; must be 0
    WORD bfReserved2;  //reserved; must be 0
    DWORD bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
} BITMAPFILEHEADER;

#pragma pack(pop)


#pragma pack(push, 1) //ensure that the compiler does not insert padding into the data and each member follows the previous one.

typedef struct tagBITMAPINFOHEADER {
    BITMAPFILEHEADER bitmapFileHeader;
    DWORD biSize;  //specifies the number of bytes required by the struct
    LONG biWidth;  //specifies width in pixels
    LONG biHeight;  //species height in pixels
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;//specifies the type of compression
    DWORD biSizeImage;  //size of image in bytes
    LONG biXPelsPerMeter;  //number of pixels per meter in x axis
    LONG biYPelsPerMeter;  //number of pixels per meter in y axis
    DWORD biClrUsed;  //number of colors used by the bitmap
    DWORD biClrImportant;  //number of colors that are important
} BITMAPINFOHEADER;

#pragma pack(pop)


#endif //ALICIA_STRUCTURES_H
