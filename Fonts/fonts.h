/**
 * @file fonts.h
 * @author Jinyi
 * @brief Font header file
 * @version 1.0
 * @date 2023-03-24
 */
#ifndef __STM32_ILI9341_LIB_FONTS_HEADER__
#define __STM32_ILI9341_LIB_FONTS_HEADER__

#include <stdint.h>

typedef struct {
    uint8_t height;
    const uint8_t width;
    const uint16_t *fontData;
} FontDef_s;

extern FontDef_s Font_07x10;
extern FontDef_s Font_11x18;
extern FontDef_s Font_16x26;

#endif