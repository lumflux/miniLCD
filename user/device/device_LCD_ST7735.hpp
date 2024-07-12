#ifndef _DEVICE_LCD_ST7735_HPP
#define _DEVICE_LCD_ST7735_HPP

#include "device_LCD_base.hpp"
#include "spi.h"
#include "gpio.h"
#include "tim.h"
#include <string>

namespace Device
{
    namespace LCD
    {
        class LCD_ST7735 : public LCD_Base<LCD_ST7735>
        {
        public: /* typedef */
        protected:
            SPI_HandleTypeDef *hSPI;

            GPIO_TypeDef *RES_GPIOx;
            uint16_t RES_GPIO_Pin;

            GPIO_TypeDef *DC_GPIOx;
            uint16_t DC_GPIO_Pin;

            GPIO_TypeDef *CS_GPIOx;
            uint16_t CS_GPIO_Pin;

            TIM_HandleTypeDef *BL_TIM;
            uint32_t BL_TIM_Channel;

        protected:
            LCD_ST7735 *_reset();
            LCD_ST7735 *_writeByte(uint8_t byte, bool isCommand = false);
            LCD_ST7735 *_writeBytes(uint8_t *pbyte, uint16_t byteLen, bool isCommand = false);
            LCD_ST7735 *_setSubWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
            LCD_ST7735 *_posConv(uint16_t &x, uint16_t &y, dir_e dir);

        public:
            LCD_ST7735(
                SPI_HandleTypeDef *hSPI,
                GPIO_TypeDef *RES_GPIOx,
                uint16_t RES_GPIO_Pin,
                GPIO_TypeDef *DC_GPIOx,
                uint16_t DC_GPIO_Pin,
                GPIO_TypeDef *CS_GPIOx,
                uint16_t CS_GPIO_Pin,
                TIM_HandleTypeDef *BL_TIM,
                uint32_t BL_TIM_Channel,
                uint16_t width,
                uint16_t height,
                bool isColorInverse = false);
            LCD_ST7735 *init();
            /* 绘图设置 */
            LCD_ST7735 *on();
            LCD_ST7735 *off();
            LCD_ST7735 *setDir(dir_e);
            LCD_ST7735 *setBackLight(uint8_t);

            /* 绘图函数 */
            LCD_ST7735 *drawPoint(uint16_t x, uint16_t y, uint16_t color);
            LCD_ST7735 *drawPoints(uint16_t *px, uint16_t *py, uint16_t *pcolor, uint16_t dotNum);
            LCD_ST7735 *drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
            LCD_ST7735 *drawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
            LCD_ST7735 *fillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
            LCD_ST7735 *drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
            LCD_ST7735 *fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
            LCD_ST7735 *fillScreen(uint16_t color);
            /* 字库 */
            LCD_ST7735 *drawAscii_6x12(uint16_t x,
                                       uint16_t y,
                                       uint16_t ascii,
                                       uint16_t color,
                                       uint16_t bgColor,
                                       bool isSuperposed);
            LCD_ST7735 *LCD_ShowString_6x12(uint16_t x,
                                            uint16_t y,
                                            std::string asciiStr,
                                            uint16_t color,
                                            uint16_t bgColor,
                                            bool isSuperposed);
        };
    } // namespace LCD
} // namespace Device

#endif //_DEVICE_LCD_ST7735_HPP
