#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#define CHANNELS 3
#define MAX_COLOR 255.999
#define uchar unsigned char

const color BLACK = color(0.0, 0.0, 0.0);
const color WHITE = color(1.0, 1.0, 1.0);
const color RED = color(1.0, 0.0, 0.0);
const color GREEN = color(0.0, 1.0, 0.0);
const color BLUE = color(0.0, 0.0, 1.0);
const color LIGHT_GREY = color(0.8, 0.8, 0.8);
const color SAND = color(0.76, 0.7, 0.5);
const color SKY = color(0.53, 0.81, 0.92);

#endif