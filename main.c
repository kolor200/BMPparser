#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <ctype.h>
//#include <stdbool.h>
//#include <math.h>
#include "structures.h"
#include "functions.h"

unsigned char *LoadBitmapFile(int argc, char *argv[], BITMAPINFOHEADER *bitmapInfoHeader) {
    FILE *in;
    unsigned char *bitmapImage;  //store image data

    //open filename in read binary mode
    in = fopen(argv[1], "rb");
    if (in == NULL) {
        printf("Unable to open the file \n");
        fclose(in);
        free(bitmapInfoHeader);
        return NULL;
    } else {
        //printf("File opened successfully. \n");
    }
    if (fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, in) == 1) {
        //printf("File read successfully. \n");
    } else {
        printf("File not read successfully. \n");
        fclose(in);
        free(bitmapInfoHeader);
        return NULL;
    }
    if (bitmapInfoHeader->bitmapFileHeader.bfType != 0x4D42) {

        printf("File is not a BMP. \n");
        fclose(in);
        free(bitmapInfoHeader);
        return NULL;
    }

    PrintBMPFileHeader(bitmapInfoHeader);
    PrintBMPHeader(bitmapInfoHeader);

    printf("Size of a file: %u bytes \n\n", sizeof(BITMAPINFOHEADER));
    if ((bitmapInfoHeader->biBitCount != 24) || (bitmapInfoHeader->biCompression != 0)) {
        printf("That histogram calculation is unsupported\n");
        fclose(in);
        free(bitmapInfoHeader);
        return NULL;
    }

    fseek(in, bitmapInfoHeader->bitmapFileHeader.bfOffBits, SEEK_SET);
    bitmapImage = (unsigned char *) malloc(bitmapInfoHeader->biSizeImage); // (unsigned char*)

    if (!bitmapImage) {
        printf("Failed when allocating memory \n");
        free(bitmapImage);
        fclose(in);
        return NULL;
    }

    fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1, in);
    GenerateHistogram(bitmapInfoHeader, bitmapImage);
    if (argc == 2) {
        GraytifyThis(bitmapInfoHeader, bitmapImage, argv[2]);
    }
    if (argc == 3) {
        HideText(bitmapInfoHeader, bitmapImage, argv[2], argv[3]);
    }
    fclose(in);
    free(bitmapImage);

    printf("Decode steganography? 1/0 \n");
    int variable = 0;
    scanf("%d", &variable);
    if (variable ==1)
    {
        ShowText(argv[1]);
    }
    else if (variable == 0)
    {
        printf("Okay, hidden message won't be decoded.");
    } else
    {
        printf("Wrong argument value.");
    }



    return bitmapImage;
}

int main(int argc, char *argv[]) {

    //printf("%d", argc);
    if (argc > 4) {
        printf("Wrong number of arguments");
        return 0;
    }

    BITMAPINFOHEADER bitmapInfoHeader;

    LoadBitmapFile((argc - 1), argv, &bitmapInfoHeader);

}

