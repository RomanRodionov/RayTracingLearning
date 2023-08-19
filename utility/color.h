#ifndef COLOR_H
#define COLOR_H

#define CHANNELS 3
#define MAX_COLOR 255.999
#define uchar unsigned char
#define UCHAR_COLOR(d) ((uchar)(d * MAX_COLOR))
#define ARRAY_FROM_COLOR(color) {UCHAR_COLOR(color[0]), UCHAR_COLOR(color[1]), UCHAR_COLOR(color[2])}

const color BLACK = color(0.0, 0.0, 0.0);
const color WHITE = color(1.0, 1.0, 1.0);
const color RED = color(1.0, 0.0, 0.0);
const color GREEN = color(0.0, 1.0, 0.0);
const color BLUE = color(0.0, 0.0, 1.0);
const color BROWN = color(0.64, 0.16, 0.16);
const color LIGHT_GREY = color(0.8, 0.8, 0.8);
const color SAND = color(0.76, 0.7, 0.5);
const color SKY = color(0.53, 0.81, 0.92);
const color LIGHT_AQUA = color(0.67, 0.93, 0.9);
const color PURPLE_PIZZA = color(1.0, 0.0, 0.8);
const color PINK = color(1.0, 0.75, 0.79);

#endif