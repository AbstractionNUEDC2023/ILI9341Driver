#include "ILI9341.h"

void ILI9341TestFunction(void){
    ILI9341Initialize();
    ILI9341BacklightControl(1);
    ILI9341FillScreen(RGB565_CYAN);
    ILI9341FillRectangle(0, 0, 200, 200, RGB565_PINK); // Verfied
    ILI9341FillCircle(100, 100, 50, RGB565_GREEN); // Verified
    ILI9341DrawCircle(100, 100, 50, RGB565_BLUE); // Verified
    ILI9341FillTriangle(200, 200, 0, 200, 0, 319, RGB565_BLACK); // Verified
    ILI9341DrawTriangle(200, 200, 0, 200, 0, 319, RGB565_WHITE); // Verified
    ILI9341DrawString(100, 300, "Hello,ILI9341!", Font_07x10, RGB565_BLUE, RGB565_CYAN); // Verified
    ILI9341DrawLine(0, 0, 240, 320, RGB565_RED); // Verified
    while(1) {
        
    }
}