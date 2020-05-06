#ifndef ALICIA_FUNCTIONS_H
#define ALICIA_FUNCTIONS_H
#include "structures.h"


void *PrintBMPFileHeader(BITMAPINFOHEADER *bitmapInfoHeader);
void *PrintBMPHeader(BITMAPINFOHEADER *bitmapInfoHeader);
void *ColorGram(unsigned int *color, char *color_name, int x);
int interval(int pvalue);
void *GenerateHistogram(BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapImage);
void GraytifyThis(BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapImage, char *path);
void *int_to_bin(int value, int *array);
int *str_to_bin(char *str, int *ptr);
void HideText(BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapImage, char *path, char *str);
int bin_to_in(const int s[]);
void *printChar(int len, char *stringers);
unsigned char *ShowText(char *path);




#endif //ALICIA_FUNCTIONS_H
