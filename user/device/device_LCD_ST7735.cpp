#include "device_LCD_ST7735.hpp"

#include "stm32f1xx_hal_spi.h"
#include "algorithm_mathLib.hpp"

#include "data_LCD_ASCII.h"

using namespace Algorithm::MathLib;

#define USE_ST7735_INSTRUCTIONS (1U)
#if USE_ST7735_INSTRUCTIONS
#define ST7735_NOP (0x00)
#define ST7735_SWRESET (0x01)
#define ST7735_RDDID (0x04)
#define ST7735_RDDST (0x09)
#define ST7735_RDDPM (0x0A)
#define ST7735_RDDMADCTL (0x0B)
#define ST7735_RDDCOLCOM (0x0C)
#define ST7735_RDDIM (0x0D)
#define ST7735_RDDSM (0x0E)
#define ST7735_SLPIN (0x10)
#define ST7735_SLPOUT (0x11)
#define ST7735_PLTON (0x12)
#define ST7735_NORON (0x13)
#define ST7735_INVOFF (0x20)
#define ST7735_INVON (0x21)
#define ST7735_GAMSET (0x26)
#define ST7735_DISPOFF (0x28)
#define ST7735_DISPON (0x29)
#define ST7735_CASET (0x2A)
#define ST7735_RASET (0x2B)
#define ST7735_RAMWR (0x2C)
#define ST7735_RAMRD (0x2E)
#define ST7735_PTLAR (0x30)
#define ST7735_TEOFF (0x34)
#define ST7735_TEON (0x35)
#define ST7735_MADCTL (0x36)
#define ST7735_IDMOFF (0x38)
#define ST7735_IDMON (0x39)
#define ST7735_COLMOD (0x3A)
#define ST7735_RDID1 (0xDA)
#define ST7735_RDID2 (0xDB)
#define ST7735_RDID3 (0xDC)

#define ST7735_FRMCTR1 (0xB1)
#define ST7735_FRMCTR2 (0xB2)
#define ST7735_FRMCTR3 (0xB3)
#define ST7735_INVCTR (0xB4)
#define ST7735_DISSET5 (0xB6)
#define ST7735_PWCTR1 (0xC0)
#define ST7735_PWCTR2 (0xC1)
#define ST7735_PWCTR3 (0xC2)
#define ST7735_PWCTR4 (0xC3)
#define ST7735_PWCTR5 (0xC4)
#define ST7735_VMCTR1 (0xC5)
#define ST7735_VMOFCTR (0xC7)
#define ST7735_WRID2 (0xD1)
#define ST7735_WRID3 (0xD2)
#define ST7735_PWCTR6 (0xFC)
#define ST7735_NVCTR1 (0xD9)
#define ST7735_NVCTR2 (0xDE)
#define ST7735_NVCTR3 (0xDF)
#define ST7735_GMCTRP1 (0xE0)
#define ST7735_GMCTRN1 (0xE1)
#define ST7735_EXTCTRL (0xF0)
#define ST7735_VCOM4L (0xFF)
#endif // USE_ST7735_INSTRUCTIONS

namespace Device
{
    namespace LCD
    {
        LCD_ST7735 *LCD_ST7735::_reset()
        {
            HAL_GPIO_WritePin(RES_GPIOx, RES_GPIO_Pin, GPIO_PIN_RESET);
            HAL_Delay(100);
            HAL_GPIO_WritePin(RES_GPIOx, RES_GPIO_Pin, GPIO_PIN_SET);
            HAL_Delay(100);
            return this;
        }
        LCD_ST7735 *LCD_ST7735::_writeByte(uint8_t byte, bool isCommand)
        {
            HAL_GPIO_WritePin(DC_GPIOx, DC_GPIO_Pin, isCommand ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(CS_GPIOx, CS_GPIO_Pin, GPIO_PIN_RESET);
            HAL_SPI_Transmit(hSPI, &byte, 1, HAL_MAX_DELAY);
            HAL_GPIO_WritePin(CS_GPIOx, CS_GPIO_Pin, GPIO_PIN_SET);
            return this;
        }
        LCD_ST7735 *LCD_ST7735::_writeBytes(uint8_t *pbyte, uint16_t byteLen, bool isCommand)
        {
            HAL_GPIO_WritePin(DC_GPIOx, DC_GPIO_Pin, isCommand ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(CS_GPIOx, CS_GPIO_Pin, GPIO_PIN_RESET);
            // HAL_SPI_Transmit_DMA(hSPI, pbyte, byteLen);
            HAL_SPI_Transmit(hSPI, pbyte, byteLen, HAL_MAX_DELAY);
            HAL_GPIO_WritePin(CS_GPIOx, CS_GPIO_Pin, GPIO_PIN_SET);
            return this;
        }
        LCD_ST7735 *LCD_ST7735::_setSubWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
        {
            uint16_t minX = Min<uint16_t>(x1, x2);
            uint16_t maxX = Max<uint16_t>(x1, x2);
            uint16_t minY = Min<uint16_t>(y1, y2);
            uint16_t maxY = Max<uint16_t>(y1, y2);

            uint8_t winX[] = {(uint8_t)(minX >> 8), (uint8_t)(minX & 0xFF), (uint8_t)(maxX >> 8), (uint8_t)(maxX & 0xFF)};
            uint8_t winY[] = {(uint8_t)(minY >> 8), (uint8_t)(minY & 0xFF), (uint8_t)(maxY >> 8), (uint8_t)(maxY & 0xFF)};

            _writeByte(ST7735_CASET, true);
            _writeBytes((uint8_t *)winX, sizeof(winX));
            _writeByte(ST7735_RASET, true);
            _writeBytes((uint8_t *)winY, sizeof(winY));
            return this;
        }
        LCD_ST7735 *LCD_ST7735::_posConv(uint16_t &x, uint16_t &y, dir_e dir)
        {
            uint16_t tempX = x, tempY = y;
            switch (dir)
            {
            case ROTATE_0:
                x = tempX + 2;
                y = tempY + 1;
                break;
            case ROTATE_90:
                x = width - tempY - 1 < 0 ? 0 : width - tempY - 1 + 2;
                y = tempX + 1;
                break;
            case ROTATE_180:
                x = width - tempX - 1 < 0 ? 0 : width - tempX - 1 + 2;
                y = height - tempY - 1 < 0 ? 0 : height - tempY - 1 + 1;
                break;
            case ROTATE_270:
                x = tempY + 2;
                y = height - tempX - 1 < 0 ? 0 : height - x - 1 + 1;
                break;
            }
            return this;
        }
        LCD_ST7735::LCD_ST7735(SPI_HandleTypeDef *hSPI,
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
                               bool isColorInverse) : LCD_Base(width, height, isColorInverse),
                                                      hSPI(hSPI),
                                                      RES_GPIOx(RES_GPIOx),
                                                      RES_GPIO_Pin(RES_GPIO_Pin),
                                                      DC_GPIOx(DC_GPIOx),
                                                      DC_GPIO_Pin(DC_GPIO_Pin),
                                                      CS_GPIOx(CS_GPIOx),
                                                      CS_GPIO_Pin(CS_GPIO_Pin),
                                                      BL_TIM(BL_TIM),
                                                      BL_TIM_Channel(BL_TIM_Channel) {}
        LCD_ST7735 *LCD_ST7735::init()
        {
            HAL_TIM_Base_Start(BL_TIM);
            HAL_TIM_PWM_Start(BL_TIM, BL_TIM_Channel);
            setBackLight(64);
            _reset();
            /* Initial Sequence */
            _writeByte(ST7735_SLPOUT, true);
            HAL_Delay(120);
            /* Frame Rate */
            _writeByte(ST7735_FRMCTR1, true);
            _writeByte(0x05);
            _writeByte(0x3C);
            _writeByte(0x3C);
            _writeByte(ST7735_FRMCTR2, true);
            _writeByte(0x05);
            _writeByte(0x3C);
            _writeByte(0x3C);
            _writeByte(ST7735_FRMCTR3, true);
            _writeByte(0x05);
            _writeByte(0x3C);
            _writeByte(0x3C);
            _writeByte(0x05);
            _writeByte(0x3C);
            _writeByte(0x3C);
            /*  */
            _writeByte(ST7735_INVCTR, true);
            _writeByte(0x03);

            /* Power Sequence */
            _writeByte(ST7735_PWCTR1, true);
            _writeByte(0x28);
            _writeByte(0x08);
            _writeByte(0x04);
            _writeByte(ST7735_PWCTR2, true);
            _writeByte(0xC0);
            _writeByte(ST7735_PWCTR3, true);
            _writeByte(0x0D);
            _writeByte(0x00);
            _writeByte(ST7735_PWCTR4, true);
            _writeByte(0x8D);
            _writeByte(0x2A);
            _writeByte(ST7735_PWCTR5, true);
            _writeByte(0x8D);
            _writeByte(0xEE);
            /*  */
            _writeByte(ST7735_VMCTR1, true);
            _writeByte(0x1A);
            _writeByte(ST7735_MADCTL, true);
            _writeByte(0x00); /* horizontal 0 */

            /* Gamma Sequence */
            _writeByte(ST7735_GMCTRP1, true);
            _writeByte(0x04);
            _writeByte(0x22);
            _writeByte(0x07);
            _writeByte(0x0A);
            _writeByte(0x2E);
            _writeByte(0x30);
            _writeByte(0x25);
            _writeByte(0x2A);
            _writeByte(0x28);
            _writeByte(0x26);
            _writeByte(0x2E);
            _writeByte(0x3A);
            _writeByte(0x00);
            _writeByte(0x01);
            _writeByte(0x03);
            _writeByte(0x13);

            _writeByte(ST7735_GMCTRN1, true);
            _writeByte(0x04);
            _writeByte(0x16);
            _writeByte(0x06);
            _writeByte(0x0D);
            _writeByte(0x2D);
            _writeByte(0x26);
            _writeByte(0x23);
            _writeByte(0x27);
            _writeByte(0x27);
            _writeByte(0x25);
            _writeByte(0x2D);
            _writeByte(0x3B);
            _writeByte(0x00);
            _writeByte(0x01);
            _writeByte(0x04);
            _writeByte(0x13);
            /*  */
            _writeByte(isColorInverse ? ST7735_INVON : ST7735_INVOFF, true);
            /*  */
            _writeByte(ST7735_COLMOD, true);
            _writeByte(0x05);
            _writeByte(ST7735_DISPON, true);
            HAL_Delay(150);
            return this;
        }

        LCD_ST7735 *LCD_ST7735::on()
        {
            _writeByte(ST7735_DISPON, true);
            HAL_Delay(150);
            return this;
        }
        LCD_ST7735 *LCD_ST7735::off()
        {
            _writeByte(ST7735_DISPOFF, true);
            HAL_Delay(150);
            return this;
        }
        LCD_ST7735 *LCD_ST7735::setDir(dir_e dir)
        {
            this->dir = dir;
            return this;
        }
        LCD_ST7735 *LCD_ST7735::setBackLight(uint8_t level)
        {
            __HAL_TIM_SET_COMPARE(BL_TIM, BL_TIM_Channel, level * 16);
            return this;
        }

        LCD_ST7735 *LCD_ST7735::drawPoint(uint16_t x, uint16_t y, uint16_t color)
        {
            _posConv(x, y, dir);
            _setSubWindow(x, y, x, y);
            _writeByte(ST7735_RAMWR, true);
            _writeByte((color >> 8) & 0xFF);
            _writeByte((color) & 0xFF);
            return this;
        }
        LCD_ST7735 *LCD_ST7735::drawPoints(uint16_t *px, uint16_t *py, uint16_t *pcolor, uint16_t dotNum)
        {
            for (uint16_t i = 0; i < dotNum; i++)
            {
                _posConv(px[i], py[i], dir);
                _setSubWindow(px[i], py[i], px[i], py[i]);
                _writeByte(ST7735_RAMWR, true);
                _writeByte((pcolor[i] >> 8) & 0xFF);
                _writeByte((pcolor[i]) & 0xFF);
            }
            return this;
        }
        LCD_ST7735 *LCD_ST7735::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
        {
            uint16_t t;
            int xerr = 0, yerr = 0, delta_x, delta_y, distance;
            int incx, incy, uRow, uCol;
            delta_x = x2 - x1;
            delta_y = y2 - y1;
            uRow = x1;
            uCol = y1;
            if (delta_x > 0)
                incx = 1;
            else if (delta_x == 0)
                incx = 0;
            else
            {
                incx = -1;
                delta_x = -delta_x;
            }
            if (delta_y > 0)
                incy = 1;
            else if (delta_y == 0)
                incy = 0;
            else
            {
                incy = -1;
                delta_y = -delta_y;
            }
            if (delta_x > delta_y)
                distance = delta_x;
            else
                distance = delta_y;
            for (t = 0; t < distance + 1; t++)
            {
                drawPoint(uRow, uCol, color);
                xerr += delta_x;
                yerr += delta_y;
                if (xerr > distance)
                {
                    xerr -= distance;
                    uRow += incx;
                }
                if (yerr > distance)
                {
                    yerr -= distance;
                    uCol += incy;
                }
            }
            return this;
        }
        LCD_ST7735 *LCD_ST7735::drawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
        {
            drawLine(x1, y1, x2, y1, color);
            drawLine(x1, y1, x1, y2, color);
            drawLine(x1, y2, x2, y2, color);
            drawLine(x2, y1, x2, y2, color);
            return this;
        }
        LCD_ST7735 *LCD_ST7735::fillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
        {
            _posConv(x1, y1, dir);
            _posConv(x2, y2, dir);

            _setSubWindow(x1, y1, x2, y2);
            _writeByte(ST7735_RAMWR, true);
            // Write the color data
            for (uint16_t i = 0; i < (Abs<int32_t>(x1 - x2) + 1) * (Abs<int32_t>(y1 - y2) + 1); i++)
            {
                _writeByte((color >> 8) & 0xFF);
                _writeByte((color) & 0xFF);
            }
            return this;
        }
        LCD_ST7735 *LCD_ST7735::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
        {
            int a, b;
            a = 0;
            b = r;
            while (a <= b)
            {
                drawPoint(x0 - b, y0 - a, color); // 3
                drawPoint(x0 + b, y0 - a, color); // 0
                drawPoint(x0 - a, y0 + b, color); // 1
                drawPoint(x0 - a, y0 - b, color); // 2
                drawPoint(x0 + b, y0 + a, color); // 4
                drawPoint(x0 + a, y0 - b, color); // 5
                drawPoint(x0 + a, y0 + b, color); // 6
                drawPoint(x0 - b, y0 + a, color); // 7
                a++;
                if ((a * a + b * b) > (r * r))
                {
                    b--;
                }
            }
            return this;
        }
        LCD_ST7735 *LCD_ST7735::fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
        {
            return this;
        }
        LCD_ST7735 *LCD_ST7735::fillScreen(uint16_t color)
        {
            dir_e temp_dir = dir;
            dir = ROTATE_0;
            fillRect(0, 0, width - 1, height - 1, color);
            dir = temp_dir;
            return this;
        }

        LCD_ST7735 *LCD_ST7735::drawAscii_6x12(uint16_t x,
                                               uint16_t y,
                                               uint16_t ascii,
                                               uint16_t color,
                                               uint16_t bgColor,
                                               bool isSuperposed)
        {
            if (!isSuperposed){
                drawRect(x, y, x + 6 - 1, y + 12 - 1, bgColor);
            }
            for (uint16_t i = 0; i < 12; i++)
            {
                uint8_t asciiByte = ascii_1206[ascii - ' '][i];
                for (uint16_t j = 0; j < 6; j++)
                {
                    if (asciiByte & (0x01 << j))
                        drawPoint(x + j, y + i, color);
                }
            }
            return this;
        }

        LCD_ST7735 *LCD_ST7735::LCD_ShowString_6x12(uint16_t x,
                                                    uint16_t y,
                                                    std::string asciiStr,
                                                    uint16_t color,
                                                    uint16_t bgColor,
                                                    bool isSuperposed)
        {
            if (!isSuperposed)
            {
                fillRect(x-1, y, x + (6 + 2) * asciiStr.length() - 2, y + 12 - 1, bgColor);
            }
            for (auto c : asciiStr){
                drawAscii_6x12(x, y, c, color, bgColor, true);
                x += 6 + 2;
            }
            return this;
        }

    } // namespace LCD
} // namespace Device
