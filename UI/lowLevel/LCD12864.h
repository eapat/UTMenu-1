#ifndef __LCD12864
#define __LCD12864

#include "stm32f4xx_hal.h"
#include "canvas.h"

#define LCD_DB0_PORT    GPIOC
#define LCD_DB0_PIN     GPIO_PIN_9

#define LCD_DB1_PORT    GPIOC
#define LCD_DB1_PIN     GPIO_PIN_8

#define LCD_DB2_PORT    GPIOC
#define LCD_DB2_PIN     GPIO_PIN_7

#define LCD_DB3_PORT    GPIOC
#define LCD_DB3_PIN     GPIO_PIN_6

#define LCD_DB4_PORT    GPIOD
#define LCD_DB4_PIN     GPIO_PIN_15

#define LCD_DB5_PORT    GPIOD
#define LCD_DB5_PIN     GPIO_PIN_14

#define LCD_DB6_PORT    GPIOD
#define LCD_DB6_PIN     GPIO_PIN_13

#define LCD_DB7_PORT    GPIOD
#define LCD_DB7_PIN     GPIO_PIN_12

//CS#
#define LCD_E1_PORT     GPIOD
#define LCD_E1_PIN      GPIO_PIN_11

#define LCD_E2_PORT     GPIOD
#define LCD_E2_PIN      GPIO_PIN_11

#define LCD_RES_PORT    GPIOD
#define LCD_RES_PIN     GPIO_PIN_10

#define LCD_RW_PORT     GPIOD
#define LCD_RW_PIN      GPIO_PIN_9

//D/C#
#define LCD_A0_PORT     GPIOD
#define LCD_A0_PIN      GPIO_PIN_8

#define LCD_E_PORT      GPIOB
#define LCD_E_PIN       GPIO_PIN_15

#define LCD_DISP_PORT   GPIOB
#define LCD_DISP_PIN    GPIO_PIN_14

void LCD_init();
void LCD_reset();
void LCD_draw(Canvas* canvas);

#endif
