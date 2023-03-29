/********************************************************************************************************
 * @Filename: ILI9341.c
 * @Author: Jinyi
 * @Version: 1.0
 * @Date: 2023-03-24
 * @Description: Implementation of ILI9341 Driver
 *********************************************************************************************************/
#include "ILI9341.h"
/**
 * @brief MADCTL Values
 */
#define ILI9341_MADCTL_MY   0x80
#define ILI9341_MADCTL_MX   0x40
#define ILI9341_MADCTL_MV   0x20
#define ILI9341_MADCTL_ML   0x10
#define ILI9341_MADCTL_BGR  0x08
#define ILI9341_MADCTL_MH   0x04
#define ILI9341_MADCTL_RGB  0x00
/**
 * @brief Address Handling
 */
#if ILI9341_FSMC_CHIP_SELECT == 4
    #define ILI9341_COMMAND_ADDRESS 0x6C000000
#elif ILI9341_FSMC_CHIP_SELECT == 3
    #define ILI9341_COMMAND_ADDRESS 0x68000000
#elif ILI9341_FSMC_CHIP_SELECT == 2
    #define ILI9341_COMMAND_ADDRESS 0x64000000
#elif ILI9341_FSMC_CHIP_SELECT == 1
    #define ILI9341_COMMAND_ADDRESS 0x60000000
#endif
#define ILI9341_DATA_ADDRESS (*(volatile uint16_t *)(ILI9341_COMMAND_ADDRESS | (1 << (ILI9341_FSMC_RS_ADDRESS_LINE_NUMBER + 1))))
/**
 * @brief Rotation handling
 */
#if ILI9341_SCREEN_ORIENTATION == 0
  #define ILI9341_WIDTH   240
  #define ILI9341_HEIGHT  320
  #define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR)
#elif ILI9341_SCREEN_ORIENTATIONS == 1
  #define ILI9341_WIDTH   320
  #define ILI9341_HEIGHT  240
  #define ILI9341_ROTATION (ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR | ILI9341_MADCTL_MX | ILI9341_MADCTL_MV)
#elif ILI9341_SCREEN_ORIENTATION == 2
  #define ILI9341_WIDTH   240
  #define ILI9341_HEIGHT  320
  #define ILI9341_ROTATION (ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR)
#elif ILI9341_SCREEN_ORIENTATION == 3
  #define ILI9341_WIDTH   320
  #define ILI9341_HEIGHT  240
  #define ILI9341_ROTATION (ILI9341_MADCTL_MV|  ILI9341_MADCTL_BGR)
#endif
/**
 * @brief Absolute value function
 * @param x Input
 * @return Absolute value
 */
#define ABS(x) (x > 0 ? x : -x)
/**Some Black Magic Here**/
typedef struct {
  volatile uint16_t Register;
  volatile uint16_t Data;
} ILI9341_t; 

ILI9341_t* LCDPtr = (ILI9341_t*)(ILI9341_COMMAND_ADDRESS | ((1 << (ILI9341_FSMC_RS_ADDRESS_LINE_NUMBER + 1)) - 2));
/**
 * @brief Private Function for Writing ILI9341's Register
 * @param regValue Value to be written
 * @return None
 **/
// void writeRegister(uint16_t regValue) { ILI9341_COMMAND_ADDRESS = regValue; }
void writeRegister(uint16_t regValue) { LCDPtr->Register = regValue; }
/**
 * @brief Private function for writing ILI9341's Graphics RAM
 * @param Data Data to be written(Only 1 uint16_t value)
 * @return None
 **/
// void writeGraphicsRAM(uint16_t Data) { ILI9341_DATA_ADDRESS = Data; }
void writeGraphicsRAM(uint16_t Data) { LCDPtr->Data = Data; }
/**
 * @brief Private function for writing array into ILI9341's Graphics RAM
 * @param arrayPtr Start poniter of the array
 * @param arraySize Length of the array
 * @return None
 */
void writeArrayIntoGraphicsRAM(uint16_t *arrayPtr, uint32_t arraySize) {
  while (arraySize--)
    ILI9341_DATA_ADDRESS = *arrayPtr++;
}
/**
 * @brief Private function for reading Graphics RAM
 * @param arrayPtr Read buffer's starting address
 * @param arraySize Read buffer's size
 * @return None
 */
void readArrayFromGraphicsRAM(uint16_t *arrayPtr, uint32_t arraySize) {
  while (arraySize--)
    *arrayPtr++ = ILI9341_DATA_ADDRESS;
}
/**
 * @brief Private function for setting the address window of ILI9341
 * @param x0 top-left corner's x coordinate
 * @param y0 top-left corner's y coordinate
 * @param x1 buttom-right corner's x coordinate
 * @param y1 buttom-right corner's y coordinate
 * @return None
 */
void setAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
  writeRegister(0x2A);{
    writeGraphicsRAM(x0 >> 8);
    writeGraphicsRAM(x0 & 0x00FF);
    writeGraphicsRAM(x1 >> 8);
    writeGraphicsRAM(x1 & 0x00FF);
  }
  // column address set
  writeRegister(0x2B);{
    writeGraphicsRAM(y0 >> 8);
    writeGraphicsRAM(y0 & 0x00FF);
    writeGraphicsRAM(y1 >> 8);
    writeGraphicsRAM(y1 & 0x00FF);
  }
  // row address set
  writeRegister(0x2C);
  // RAM Write/Read
}

void ILI9341BacklightControl(uint8_t backlightOn) {
  if (backlightOn > 0) {
    HAL_GPIO_WritePin(ILI9341_BACKLIGHT_GPIO_PORT, ILI9341_BACKLIGHT_GPIO_PIN,
                      1);
  } else {
    HAL_GPIO_WritePin(ILI9341_BACKLIGHT_GPIO_PORT, ILI9341_BACKLIGHT_GPIO_PIN,
                      0);
  }
}

void ILI9341Initialize(void) {
  ILI9341BacklightControl(0);
  writeRegister(0x01);
  HAL_Delay(500);
  // Power control A configuration
  writeRegister(0xCB);
  {
    uint16_t regValue[] = {0x39, 0x2C, 0x00, 0x34, 0x02};
    writeArrayIntoGraphicsRAM(regValue, 5);
  }
  // Power control B configuration
  writeRegister(0xCF);
  {
    uint16_t regValue[] = {0x00, 0xC1, 0x30};
    writeArrayIntoGraphicsRAM(regValue, 3);
  }
  // Driver Timing A configuration
  writeRegister(0xE8);
  {
    uint16_t regValue[] = {0x85, 0x00, 0x78};
    writeArrayIntoGraphicsRAM(regValue, 3);
  }
  // Driver Timing B configuration
  writeRegister(0xEA);
  {
    uint16_t regValue[] = {0x00, 0x00};
    writeArrayIntoGraphicsRAM(regValue, 2);
  }
  // Power on sequence configuration
  writeRegister(0xED);
  {
    uint16_t regValue[] = {0x04, 0x03, 0x12, 0x81};
    writeArrayIntoGraphicsRAM(regValue, 4);
  }
  // Charge pump ratio configuration
  writeRegister(0xF7);
  {
    uint16_t regValue[] = {0x20};
    writeArrayIntoGraphicsRAM(regValue, 1);
  }
  // Power configuration, VRH[5:0]
  writeRegister(0xC0);
  {
    uint16_t regValue[] = {0x23};
    writeArrayIntoGraphicsRAM(regValue, 1);
  }
  // Power configuration, SAP[2:0] BT[3:0]
  writeRegister(0xC1);
  {
    uint16_t regValue[] = {0x10};
    writeArrayIntoGraphicsRAM(regValue, 1);
  }
  // Power configuration, VCM
  writeRegister(0xC5);
  {
    uint16_t regValue[] = {0x3E, 0x28};
    writeArrayIntoGraphicsRAM(regValue, 2);
  }
  // Power configuration, VCM2
  writeRegister(0xC7);
  {
    uint16_t regValue[] = {0x86};
    writeArrayIntoGraphicsRAM(regValue, 1);
  }
  // Memory address configuration
  writeRegister(0x36);
  {
    uint16_t regValue[] = {0x48};
    writeArrayIntoGraphicsRAM(regValue, 1);
  }
  // Pixel format configuration
  writeRegister(0x3A);
  {
    uint16_t regValue[] = {0x55};
    writeArrayIntoGraphicsRAM(regValue, 1);
  }
  // Frame ratio configuration
  writeRegister(0xB1);
  {
    uint16_t regValue[] = {0x00, 0x18};
    writeArrayIntoGraphicsRAM(regValue, 1);
  }
  // Display function configuration
  writeRegister(0xB6);
  {
    uint16_t regValue[] = {0x08, 0x82, 0x27};
    writeArrayIntoGraphicsRAM(regValue, 3);
  }
  // Gamma function disable
  writeRegister(0xF2);
  {
    uint16_t regValue[] = {0x00};
    writeArrayIntoGraphicsRAM(regValue, 1);
  }
  // Gamma curve selection
  writeRegister(0x26);
  {
    uint16_t regValue[] = {0x01};
    writeArrayIntoGraphicsRAM(regValue, 1);
  }
  // Positive gamma correction
  writeRegister(0xE0);
  {
    uint16_t regValue[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
                            0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
    writeArrayIntoGraphicsRAM(regValue, 15);
  }
  // Negative gamma correction
  writeRegister(0xE1);
  {
    uint16_t regValue[] = {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
                            0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F};
    writeArrayIntoGraphicsRAM(regValue, 15);
  }
  // Exit sleep mode
  writeRegister(0x11);
  HAL_Delay(120);
  // Display on
  writeRegister(0x29);
  // MADCTL
  writeRegister(0x36);
  {
    uint16_t regValue[] = {ILI9341_ROTATION};
    writeArrayIntoGraphicsRAM(regValue, 1);
  }
  ILI9341BacklightControl(1);
}
void ILI9341DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
  if((x > ILI9341_WIDTH) || (y > ILI9341_HEIGHT))
    return;
  setAddressWindow(x, y, x + 1, y + 1);
  writeGraphicsRAM(color);
}
/**
 * @brief Draw char on screen
 * @param x left coordinate of character
 * @param y top coordinate of character
 * @param ch character to draw
 * @param font font of character
 * @param color color of character
 * @param bgcolor background color of character
 * @return * void 
 */
void drawChar(uint16_t x, uint16_t y, char ch, FontDef_s font,
              uint16_t color, uint16_t bgcolor){
  uint32_t i, b, j;
  setAddressWindow(x, y, font.width+x-1, font.height+y-1);
  for(i=0; i<font.height; i++){
    b = font.fontData[(ch-32)*font.height+i];
    for(j=0; j<font.width; j++){
      if ((b << j) & 0x8000)
        ILI9341DrawPixel(x+j, y+i, color);
      else
        ILI9341DrawPixel(x+j, y+i, bgcolor);
    }
  }
}

void ILI9341DrawString(uint16_t x, uint16_t y, const char *string,
                        FontDef_s font, uint16_t color, uint16_t bgColor){
  while(*string) {
    if(x + font.width >= ILI9341_WIDTH) {
      x = 0;
      y += font.height;
      if(y + font.height >= ILI9341_HEIGHT)
        break;
      if(*string == ' ') {
        string++;
        continue;
      }
    }
    drawChar(x, y, *string, font, color, bgColor);
    x += font.width;
    string++;
  }
}
void ILI9341FillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                            uint16_t color){
  if(x >= ILI9341_WIDTH || y >= ILI9341_HEIGHT)
    return;
  if(x+width+1 >= ILI9341_WIDTH)
    width = ILI9341_WIDTH - x;
  if(y+height+1 >= ILI9341_HEIGHT)
    height = ILI9341_HEIGHT - y;
  setAddressWindow(x, y, x+width-1, y+height-1);
  for(y = height; y >0; y--) {
    for(x = width; x >0; x--) {
      writeGraphicsRAM(color);
    }
  }
}
void ILI9341FillScreen(uint16_t color){
  ILI9341FillRectangle(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
}
void ILI9341DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                        uint16_t color){
  uint16_t swapBuffer;
  uint16_t steep = (y1 > y0 ? y1 - y0 : y0 - y1) > (x1 > x0 ? x1 - x0 : x0 - x1);
  if(steep) {
    swapBuffer = x0;
    x0 = y0;
    y0 = swapBuffer;
    swapBuffer = x1;
    x1 = y1;
    y1 = swapBuffer;
  }
  if(x0 > x1) {
    swapBuffer = x0;
    x0 = x1;
    x1 = swapBuffer;
    swapBuffer = y0;
    y0 = y1;
    y1 = swapBuffer;
  }
  //Judge the direction of scaning and drawing

  int16_t diffX, diffY;
  diffX = x1 - x0;
  diffY = (y1 > y0 ? y1 - y0 : y0 - y1);
  int32_t err = diffX / 2;
  int16_t ystep = y0 < y1 ? 1 : -1;
  for(; x0<=x1; x0++) {
    if(steep)
      ILI9341DrawPixel(y0, x0, color);
    else
      ILI9341DrawPixel(x0, y0, color);
    err -= diffY;
    if(err < 0) {
      y0 += ystep;
      err += diffX;
    }
  }
}
void ILI9341DrawCircle(uint16_t x, uint16_t y, uint8_t radius, uint16_t color){
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x0 = 0;
  int16_t y0 = radius;
  ILI9341DrawPixel(x, y+radius,color);
  ILI9341DrawPixel(x, y-radius,color);
  ILI9341DrawPixel(x+radius, y,color);
  ILI9341DrawPixel(x-radius, y,color);
  while(x0 < y0) {
    if(f >= 0) {
      y0--;
      ddF_y += 2;
      f += ddF_y;
    }
    x0++;
    ddF_x += 2;
    f += ddF_x;
    ILI9341DrawPixel(x+x0, y+y0,color);
    ILI9341DrawPixel(x-x0, y+y0,color);
    ILI9341DrawPixel(x+x0, y-y0,color);
    ILI9341DrawPixel(x-x0, y-y0,color);
    ILI9341DrawPixel(x+y0, x+x0,color);
    ILI9341DrawPixel(x-y0, x+x0,color);
    ILI9341DrawPixel(x+y0, x-x0,color);
    ILI9341DrawPixel(x-y0, x-x0,color);
  }
}
void ILI9341FillCircle(uint16_t x, uint16_t y, uint8_t radius, uint16_t color){
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x0 = 0;
  int16_t y0 = radius;
  ILI9341DrawPixel(x, y+radius, color);
  ILI9341DrawPixel(x, y-radius, color);
  ILI9341DrawPixel(x+radius, y, color);
  ILI9341DrawPixel(x-radius, y, color);
  ILI9341DrawLine(x-radius, y, x+radius, y, color);
  while(x0 < y0){
    if(f >= 0) {
      y0--;
      ddF_y += 2;
      f += ddF_y;
    }
    x0++;
    ddF_x += 2;
    f += ddF_x;
    ILI9341DrawLine(x-x0, y+y0, x+x0, y+y0, color); // OK
    ILI9341DrawLine(x-x0, y-y0, x+x0, y-y0, color); // 
    ILI9341DrawLine(x+y0, y+x0, x-y0, y+x0, color); // OK
    ILI9341DrawLine(x-y0, y-x0, x+y0, y-x0, color); // OK
  }
}

void ILI9341DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                            uint16_t x3, uint16_t y3, uint16_t color){
  ILI9341DrawLine(x1, y1, x2, y2, color);
  ILI9341DrawLine(x2, y2, x3, y3, color);
  ILI9341DrawLine(x3, y3, x1, y1, color);
}

void ILI9341FillTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                            uint16_t x3, uint16_t y3, uint16_t color) {
  int16_t diffX = 0, diffY = 0, x = 0, y = 0, increaseX0 = 0, increaseX1 = 0,
          increaseY0 = 0, increaseY1 = 0, den = 0, num = 0, addNum = 0,
          pixelsNum = 0, curPixel = 0;
  diffX = x2 > x1 ? x2 - x1 : x1 - x2;
  diffY = y2 > y1 ? y2 - y1 : y1 - y2;
  x = x1;
  y = y1;
  if(x2 >= x1) {
    increaseX0 = 1;
    increaseX1 = 1;
  } else {
    increaseX0 = -1;
    increaseX1 = -1;
  }
  if(y2 >= y1) {
    increaseY0 = 1;
    increaseY1 = 1;
  } else {
    increaseY0 = -1;
    increaseY1 = -1;
  }
  if(diffX >= diffY) {
    increaseX0 = 0;
    increaseY1 = 0;
    den = diffX;
    num = diffX / 2;
    addNum = diffY;
    pixelsNum = diffX;
  } else {
    increaseX1 = 0;
    increaseY1 = 0;
    den = diffY;
    num = diffY / 2;
    addNum = diffX;
    pixelsNum = diffY;
  }
  for(curPixel = 0; curPixel <= pixelsNum; ++curPixel) {
    ILI9341DrawLine(x, y, x3, y3, color);
    num += addNum;
    if(num >= den) {
      num -= den;
      x += increaseX0;
      y += increaseY0;
    }
    x += increaseX1;
    y += increaseY1;
  }
}
typedef struct {
  float x;
  float y;
} PointF_s;
PointF_s bezierInterpolate(float t, PointF_s* points, uint32_t pointNum){
  PointF_s pointBuffer[64] = {0};
  if(pointNum<=0) return pointBuffer[0];
  pointNum = pointNum <= 64 ? pointNum : 64;
  for(uint32_t i = 1; i < pointNum; i++){
    for(uint32_t j = 0; j < pointNum - i; j++){
      if(i == 1) {
        pointBuffer[j].x = points[j].x * (1 - t) + points[j + 1].x * t;
        pointBuffer[j].y = points[j].y * (1 - t) + points[j + 1].y * t;
        continue;
      }
      pointBuffer[j].x = (float)(pointBuffer[j].x * (1 - t) + points[j + 1].x * t);
      pointBuffer[j].y = (float)(pointBuffer[j].y * (1 - t) + points[j + 1].y * t);
    }
  }
  return pointBuffer[0];
}

void ILI9341DrawBezierCurve(uint16_t x, uint16_t y,
                            uint8_t *controlPointXArray, uint8_t *controlPointYArray,
                            uint8_t controlPointNum, uint16_t color, uint16_t end){
  float step = 0.001F, t = 0.000F;
  PointF_s controlPoint[64] = {0};
  controlPointNum = controlPointNum <= 64 ? controlPointNum : 64;
  for(uint8_t i = 0; i < controlPointNum; i++){
    controlPoint[i].x = controlPointXArray[i];
    controlPoint[i].y = controlPointYArray[i];
  }
  PointF_s drawPoint;
  while(t < end) {
    drawPoint = bezierInterpolate(t, controlPoint, controlPointNum);
    t = t + step;
    if(((((uint16_t)(drawPoint.x + 0.5)) + x) >= ILI9341_WIDTH)
    || ((((uint16_t)(drawPoint.y + 0.5)) + y) >= ILI9341_HEIGHT))
      continue;
    ILI9341DrawPixel((uint16_t)(drawPoint.x + 0.5) + x,
                      (uint16_t)(drawPoint.y + 0.5) + y,
                      color);
  }
}

void ILI9341DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                        const uint16_t *image){
  if(x >= ILI9341_WIDTH || y >= ILI9341_HEIGHT)
    return;
  if(x + width - 1 >= ILI9341_WIDTH)
    return;
  if(y + height - 1 >= ILI9341_HEIGHT)
    return;
  setAddressWindow(x, y, x + width - 1, y + height - 1);
  writeArrayIntoGraphicsRAM((uint16_t*)image, width * height);
}

void ILI9341ColorInvert(uint8_t invert) {
  writeRegister(invert ? 0x21 : 0x20);
}
