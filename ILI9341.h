/********************************************************************************************************
 * @Filename: ILI9341.h
 * @Author: Jinyi
 * @Version: 1.0
 * @Date: 2023-03-24
 * @Description: Main header of ILI9341 Driver Library
 *********************************************************************************************************/
#ifndef __STM32_ILI9341_LIB_MAIN_HEADER__
#define __STM32_ILI9341_LIB_MAIN_HEADER__

#include "Fonts/fonts.h"
#include "ILI9341Cfg.h"
#include <main.h>

/**
 * @brief RGB 565 Color Table
 */
#define RGB565_BLACK 0x0000
#define RGB565_BLUE 0x001F
#define RGB565_GREEN 0x07E0
#define RGB565_CYAN 0x07FF
#define RGB565_RED 0xF800
#define RGB565_MAGENTA 0xF81F
#define RGB565_YELLOW 0xFFE0
#define RGB565_WHITE 0xFFFF
#define RGB565_ORANGE 0xFD20
#define RGB565_GREENYELLOW 0xAFE5
#define RGB565_PINK 0xF81F
#define RGB565_DARKGREEN 0x0320
#define RGB565_DARKCYAN 0x03E0

/**
 * @brief Converting RGB888 to RGB565
 * @param r uint8_t red value
 * @param g uint8_t green value
 * @param b uint8_t blue value
 * @return uint16_t RGB565 value
 */
#define RGB888ToRGB565(r, g, b)                                                \
  (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))
/**
 * @brief Backlight control
 * @param backlightOn 0: backlight off, 1: backlight on
 * @return None
 */
void ILI9341BacklightControl(uint8_t backlightOn);
/**
 * @brief Initializes ILI9341, make sure always call this function before using
 * any other functions
 * @return None
 */
void ILI9341Initialize(void);
/**
 * @brief Draw a pixel on screen
 * @param x x coordinate of the pixel
 * @param y y coordinate of the pixel
 * @param color color of the pixel
 * @return None
 */
void ILI9341DrawPixel(uint16_t x, uint16_t y, uint16_t color);
/**
 * @brief Draw a string on the screen
 * @param x left x coordinate of the string
 * @param y up y coordinate of the string
 * @param string string to draw
 * @param font font of the string
 * @param color font color of the string
 * @param bgColor background color of the string
 * @return None 
 */
void ILI9341DrawString(uint16_t x, uint16_t y, const char *string,
                        FontDef_s font, uint16_t color, uint16_t bgColor);
/**
 * @brief Fill a rectangle with the specified color
 * @param x left x coordinate
 * @param y up y coordinate
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param color color of the rectangle
 * @return None
 */
void ILI9341FillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                            uint16_t color);
/**
 * @brief Fill screen with pure color
 * @param color color to fill with
 * @return None
 */
void ILI9341FillScreen(uint16_t color);
/**
 * @brief draw line with specified color
 * @param x0 start point's x coordinate
 * @param y0 start point's y coordinate
 * @param x1 end point's x coordinate
 * @param y1 end point's y coordinate
 * @param color color of line
 * @return None
 */
void ILI9341DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                        uint16_t color);
/**
 * @brief Draw circle with specified color(Not filled)
 * @param x x coordinate of circle center
 * @param y y coordinate of circle center
 * @param radius radius of circle
 * @param color color of line
 * @return None
 */
void ILI9341DrawCircle(uint16_t x, uint16_t y, uint8_t radius, uint16_t color);
/**
 * @brief Fill a circle aera with specified color
 * @param x x coordinate of circle center
 * @param y y coordinate of circle center
 * @param radius radius of circle
 * @param color color of area
 * @return None
 */
void ILI9341FillCircle(uint16_t x, uint16_t y, uint8_t radius, uint16_t color);
/**
 * @brief 
 * @param xy coordinates of top points 
 * @param color color of lines
 * @return None 
 */
void ILI9341DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                            uint16_t x3, uint16_t y3, uint16_t color);
/**
 * @brief Fill a triangle area with specified color
 * @param xy coordinates of top points
 * @param color color of the area
 * @return None
 */
void ILI9341FillTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                            uint16_t x3, uint16_t y3, uint16_t color);
/**
 * @brief Draw bezier curve with specified control points
 * @param controlPointXArray start address of control point's x coordinate array
 * @param controlPointYArray start address of control point's y coordinate array
 * @param controlPointNum number of control point, Cautiont that control point number must be less than 64
 * @param end range of bezier curve
 * @return * void 
 */
void ILI9341DrawBezierCurve(uint16_t x, uint16_t y,
                            uint8_t *controlPointXArray, uint8_t *controlPointYArray,
                            uint8_t controlPointNum, uint16_t color, uint16_t end);
/**
 * @brief Draw a image on screen
 * @param x left coordinate of the image
 * @param y up coordinate of the image
 * @param width width of the image
 * @param height height of the image
 * @param image rgb565 points array
 * @return None
 */
void ILI9341DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                        const uint16_t *image);
/**
 * @brief Control inverting color of whole screen
 * @param invert invert or not
 * @return None
 */
void ILI9341ColorInvert(uint8_t invert);

#if ILI9341_DRIVER_LIBRARY_ENABLE_TEST == 1
void ILI9341TestFunction(void);
#endif

#endif