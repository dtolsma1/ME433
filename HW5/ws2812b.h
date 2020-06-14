#ifndef WS2812B_H__
#define WS2812B_H__

#include<xc.h> // processor SFR definitions

#define NUMLEDS 4
// link three 8bit colors together
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} wsColor; 

void ws2812b_setup();
void ws2812b_setColor(wsColor*,int);
wsColor HSBtoRGB(float hue, float sat, float brightness);

void ws2812b_setAllSame(wsColor *c, unsigned char red, unsigned char green, unsigned char blue);
void ws2812b_initColor(wsColor *c, int LEDnum, unsigned char red, unsigned char green, unsigned char blue);

#endif