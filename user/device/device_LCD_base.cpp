#include "device_LCD_base.hpp"

namespace Device
{
    namespace LCD
    {
        uint16_t getRGB565(stdColor_e color)
        {
            switch (color)
            {
            case WHITE:
                return 0xFFFF;
            case BLACK:
                return 0x0000;
            case BLUE:
                return 0x001F;
            case BRED:
                return 0XF81F;
            case GRED:
                return 0XFFE0;
            case GBLUE:
                return 0X07FF;
            case RED:
                return 0xF800;
            case MAGENTA:
                return 0xF81F;
            case GREEN:
                return 0x07E0;
            case CYAN:
                return 0x7FFF;
            case YELLOW:
                return 0xFFE0;
            case BROWN:
                return 0XBC40;
            case BRRED:
                return 0XFC07;
            case GRAY:
                return 0X8430;
            case DARKBLUE:
                return 0X01CF;
            case LIGHTBLUE:
                return 0X7D7C;
            case GRAYBLUE:
                return 0X5458;
            case LIGHTGREEN:
                return 0X841F;
            case LGRAY:
                return 0XC618;
            case LGRAYBLUE:
                return 0XA651;
            case LBBLUE:
                return 0X2B12;
            }
        }

        uint16_t getRGB565(uint8_t r, uint8_t g, uint8_t b)
        {
            return (r << 11) + (g << 5) + (b);
        }
    } // namespace LCD
} // namespace Device