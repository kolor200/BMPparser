#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ctype.h>
//#include <stdbool.h>
//#include <math.h>
#include "structures.h"



void *PrintBMPFileHeader(BITMAPINFOHEADER *bitmapInfoHeader) {
    printf("BITMAPILEHEADER: \n");
    printf("bfType: %X \n", bitmapInfoHeader->bitmapFileHeader.bfType);
    printf("bfSize: %d \n", bitmapInfoHeader->bitmapFileHeader.bfSize);
    printf("bfReserved1: %d \n", bitmapInfoHeader->bitmapFileHeader.bfReserved1);
    printf("bfReserved2: %d \n", bitmapInfoHeader->bitmapFileHeader.bfReserved2);
    printf("bfOffBits: %d \n\n", bitmapInfoHeader->bitmapFileHeader.bfOffBits);
    return NULL;
}

void *PrintBMPHeader(BITMAPINFOHEADER *bitmapInfoHeader) {
    printf("BITMAPINFOHEADER: \n");
    printf("biSize: %d \n", bitmapInfoHeader->biSize);
    printf("biWidth: %d \n", bitmapInfoHeader->biWidth);
    printf("biHeight: %d \n", bitmapInfoHeader->biHeight);
    printf("biPlanes: %d \n", bitmapInfoHeader->biPlanes);
    printf("biSize: %d \n", bitmapInfoHeader->biSize);
    printf("biBitCount: %d \n", bitmapInfoHeader->biBitCount);
    printf("biCompression: %d \n", bitmapInfoHeader->biCompression);
    printf("biSizeImage: %d \n", bitmapInfoHeader->biSizeImage);
    printf("biXPelsPerMeter: %d \n", bitmapInfoHeader->biXPelsPerMeter);
    printf("biYPelsPerMeter: %d \n", bitmapInfoHeader->biYPelsPerMeter);
    printf("biClrUsed: %d \n", bitmapInfoHeader->biClrUsed);
    printf("biClrImportant: %d \n", bitmapInfoHeader->biClrImportant);
    return NULL;
}

void *ColorGram(unsigned int *color, char *color_name, int x) {
    int number_of_pixels = x; //x
    int k = 0;
    float percentage;

    printf("%s:\n", color_name);

//    for (int i = 0; i <= 224; i += 15)
//    {
//        printf("%d", color[i]);
//    }

    for (int i = 0; i <= 225; i += 15) {
        percentage = (float) color[i / 15] / number_of_pixels;
        printf(" %d-%d: %0.2f%%   \n", i + k, i + k + 15, percentage * 100);
        k++;
    }
    return NULL;
}

int interval(int pvalue) {
    int k = 0;
    for (int i = 0; i <= 225; i += 15) {
        if (pvalue >= i + k && pvalue <= i + k + 15) {
            return k;
        }
        k++;
    }
}

void *GenerateHistogram(BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapImage) {

    int width = bitmapInfoHeader->biWidth;
    int height = bitmapInfoHeader->biHeight;
    //int bitCount = bitmapInfoHeader->biBitCount;
    size_t rowSize = (bitmapInfoHeader->biBitCount * width + 31) / 32 * 4;
    size_t bytesPerPixel = bitmapInfoHeader->biBitCount / 8;
//    printf("%d\n", bitCount);
//    printf("%d\n", width);
//    printf("%d\n", height);
//    printf("%d\n", width * height);
    int k = 0;


    unsigned int red[16], green[16], blue[16];

    for (int i = 0; i <= 15; ++i) {
        red[i] = 0;
        green[i] = 0;
        blue[i] = 0;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            size_t offset = rowSize * i + bytesPerPixel * j;
            blue[interval(bitmapImage[offset + 0])] += 1;
            green[interval(bitmapImage[offset + 1])] += 1;
            red[interval(bitmapImage[offset + 2])] += 1;

        }
    }
//
    ColorGram(red, "RED", width * height);
    ColorGram(green, "GREEN", width * height);
    ColorGram(blue, "BLUE", width * height);
    return NULL;
}

void GraytifyThis(BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapImage, char *path) {
    int width = bitmapInfoHeader->biWidth;
    int height = bitmapInfoHeader->biHeight;
    size_t rowSize = (bitmapInfoHeader->biBitCount * width + 31) / 32 * 4;
    size_t bytesPerPixel = bitmapInfoHeader->biBitCount / 8;

    FILE *out;
    out = fopen(path, "wb");
    if (out == NULL) {
        printf("Unable to open the file \n");
        return;
    } else {
        //printf("File opened successfully. \n");
    }
    fwrite(bitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), out);
    int r, g, b, gray;
    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width - 1; j++) {
            size_t offset = rowSize * i + bytesPerPixel * j;
            b = bitmapImage[offset + 0];
            g = bitmapImage[offset + 1];
            r = bitmapImage[offset + 2];
            gray = (r + g + b) / 3;
            for (size_t k = 0; k <= 3; k++) {
                bitmapImage[offset + k] = gray;
            }
        }
    }
    fseek(out, bitmapInfoHeader->bitmapFileHeader.bfOffBits, SEEK_SET);
    fwrite(bitmapImage, 1, bitmapInfoHeader->biSizeImage, out);
    fclose(out);
}

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c "
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

void *int_to_bin(int value, int *array) {

    int n, k;
    n = value;
    for (int c = 7; c >= 0; c--) {
        k = n >> c;

        if (k & 1) {
            array[c] = 1;
        } else {
            array[c] = 0;
        }
    }
//    for (int c = 7; c >= 0; c--) {
//        printf("%d", array[c]);
//    }
//    printf("\n");
    return NULL;

}

int *str_to_bin(char *str, int *ptr) {

    int counter = 0;
    for (; *str != 0; ++str) {
        for (int i = 7; i >= 0; --i)
            if (*str & 1 << i) {
                ptr[counter] = 1;
                counter = counter + 1;
            } else {
                ptr[counter] = 0;
                counter = counter + 1;
            }
    }

    return ptr;

}

void HideText(BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapImage, char *path, char *str) {
    printf("Encoding text: '%s'\n", str);
    int array[8];
    int_to_bin((int) strlen(str), array);
    int counter = 0;

    for (int c = 7; c >= 0; c--) {

        //printf("%d", array[c]);
        if (array[c] == 1) {
            bitmapImage[counter] = (bitmapImage[counter] & ~(1UL << 0)) | (1 << 0);
        } else if (array[c] == 0) {
            bitmapImage[counter] = (bitmapImage[counter] & ~(1UL << 0)) | (0 << 0);
        }
        counter = counter + 1;
    }
    size_t len = strlen(str);
    int *ptr = calloc(len * 8, sizeof(int));
    str_to_bin(str, ptr);
    unsigned int x = (len * 8) - 1;
    //printf(" %d ", x);
    for (int i = 0; i <= x; i++) {
        if (ptr[i] == 1) {
            bitmapImage[counter] = (bitmapImage[counter] & ~(1UL << 0)) | (1 << 0);
        } else if (ptr[i] == 0) {
            bitmapImage[counter] = (bitmapImage[counter] & ~(1UL << 0)) | (0 << 0);
        }
        counter = counter + 1;
        //printf("%d ", ptr[i]);
    }
    printf("Length of saved string: %d\n", len);
    free(ptr);
    FILE *out;
    out = fopen(path, "wb");
    fwrite(bitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), out);
    fseek(out, bitmapInfoHeader->bitmapFileHeader.bfOffBits, SEEK_SET);
    fwrite(bitmapImage, 1, bitmapInfoHeader->biSizeImage, out);
    fclose(out);

}

int bin_to_in(const int s[]) {
    int n = 0;
    int i;

    for (i = 0; i < 8; ++i) {
        n <<= 1;
        n += s[i] - '0';
    }

    return n;
}

void *printChar(int len, char *stringers) {
    printf("Decoded message: \n");
    int b = 0;
    for (; b <= len - 1; b++) {
        printf("%c", stringers[b]);
    }
    return NULL;
}

int pow(int base, int exponent)
{
    int result=1;
    for (exponent; exponent>0; exponent--)
    {
        result = result * base;
    }
    return result;
}

unsigned char *ShowText(char *path) {
    FILE *in; //our file pointer
    BITMAPINFOHEADER bitmapinfoheader_object;
    BITMAPINFOHEADER *bitmapInfoHeader = &bitmapinfoheader_object;
    unsigned char *bitmapImage;  //store image data

    //open filename in read binary mode
    in = fopen(path, "rb");
    if (in == NULL) {
        printf("Show text: Unable to open the file \n");
        fclose(in);
        free(bitmapInfoHeader);
        return NULL;
    } else {
        //printf("Show text: File opened successfully. \n");
    }
    if (fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, in) == 1) {
        //printf("Show text: File read successfully. \n");
    } else {
        printf("Show text: File not read successfully. \n");
        free(bitmapInfoHeader);
        fclose(in);
        return NULL;
    }
    if (bitmapInfoHeader->bitmapFileHeader.bfType != 0x4D42) {
        printf("Show text: File is not a BMP. \n");
        fclose(in);
        free(bitmapInfoHeader);
        return NULL;
    }

    if ((bitmapInfoHeader->biBitCount != 24) || (bitmapInfoHeader->biCompression != 0)) {
        printf("Show text: Such BMP is unsupported\n");
        fclose(in);
        free(bitmapInfoHeader); //do zwolenia pamiec todo do każdego returna,
        return NULL;
    }

    fseek(in, bitmapInfoHeader->bitmapFileHeader.bfOffBits, SEEK_SET);
    bitmapImage = (unsigned char *) malloc(bitmapInfoHeader->biSizeImage); // (unsigned char*)

    if (!bitmapImage) {
        printf("Failed when allocating memory \n");
        free(bitmapInfoHeader);
        free(bitmapImage);
        fclose(in);
        return NULL;
    }

    fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1, in);

//    for (int i = 0; i <= 34; ++i) { //bitmapInfoHeader->biSizeImage - 1
//        //bitmapImage[i] = (bitmapImage[i] & ~(1UL << 0)) | (0 << 0);
//        printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(bitmapImage[i]));
//        if ((i + 1) % 4 == 0)
//            printf("\n");
//        //printf("%d ", bitmapImage[i]);
//    }

    int counter = 0;
    int len = 0;
    for (int c = 7; c >= 0; c--) {

        int bit = (bitmapImage[counter] >> 0) & 1U;
        if (bit == 1) {
            len = len + (int) pow(2, c);
        }
        //printf(" %d ", bit);
        counter = counter + 1;
    }
    printf("Length of decoded text: %d", len);
    char *stringers = calloc(len, sizeof(char));
    for (int x = 0; x <= len - 1; x++) {
        int k = 0;
        for (int c = 7; c >= 0; c--) {

            int bit = (bitmapImage[counter] >> 0) & 1U;
            if (bit == 1) {
                k = k + (int) pow(2, c);
                stringers[x] = (char) k;
            }
            //printf(" %d ", bit);
            counter = counter + 1;
        }
        //printf(" %c ", k);

    }
    printf("\n");
    printChar(len, stringers);

    fclose(in);
    free(bitmapImage);
    return bitmapImage;
}

