/********************************************************************************************************
 * @Filename: ILI9341Cfg.h
 * @Author: Jinyi
 * @Version: 1.0
 * @Date: 2023-03-24
 * @Description: Configuretions for ILI9341 Driver Library
 *********************************************************************************************************/
#ifndef __STM32_ILI9341_LIB_CONFIGURATION_HEADER__
#define __STM32_ILI9341_LIB_CONFIGURATION_HEADER__

#include <gpio.h>
/**
 * @brief Chip Select of FSMC Interface, by default using NE4
 **/
#define ILI9341_FSMC_CHIP_SELECT 4
/**
 * @brief Using which address line controlling RS
 * @details Notice that not all board uses address line to control RS, if your
 *board uses GPIO, Please Modify this library by yourself
 **/
#define ILI9341_FSMC_RS_ADDRESS_LINE_NUMBER 6
/**
 * @brief GPIO Setting For ILI9341's Backlight
 */
#define ILI9341_BACKLIGHT_GPIO_PIN GPIO_PIN_15
#define ILI9341_BACKLIGHT_GPIO_PORT GPIOB
/**
 * @brief Orientation configuration
 * @details 0 for vertical, 1 for horizontal and 2 for vertical mirror, 3 for
 * horizontal mirror
 */
#define ILI9341_SCREEN_ORIENTATION 2
/**
 * @brief Run the test function or not
 */
#define ILI9341_DRIVER_LIBRARY_ENABLE_TEST 1

#endif
