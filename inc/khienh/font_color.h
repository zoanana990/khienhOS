#ifndef FONT_COLOR_H
#define FONT_COLOR_H

typedef enum
{
    FC_BLACK = 0,
    FC_BLUE,
    FC_GREEN,
    FC_CYAN,
    FC_RED,
    FC_PURPLE,
    FC_BROWN,
    FC_GRAY,
    FC_DARK_GRAY,
    FC_LIGHT_BLUE,
    FC_LIGHT_GREEN,
    FC_LIGHT_RED,
    FC_LIGHT_PURPLE,
    FC_YELLOW,
    FC_WHITE,
    FC_LAST
}font_color_t;

#define FC_ERROR    FC_RED
#define FC_WARNING  FC_YELLOW
#define FC_TEXT     FC_WHITE

#endif