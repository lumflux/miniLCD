#ifndef _DEVICE_LCD_BASE_HPP
#define _DEVICE_LCD_BASE_HPP

#include <stdint.h>
namespace Device
{
    namespace LCD
    {

        typedef enum
        {
            WHITE,
            BLACK,
            BLUE,
            BRED,
            GRED,
            GBLUE,
            RED,
            MAGENTA,
            GREEN,
            CYAN,
            YELLOW,
            BROWN,
            BRRED,
            GRAY,
            DARKBLUE,
            LIGHTBLUE,
            GRAYBLUE,
            LIGHTGREEN,
            LGRAY,
            LGRAYBLUE,
            LBBLUE,
        } stdColor_e;

        uint16_t getRGB565(stdColor_e color);
        uint16_t getRGB565(uint8_t r, uint8_t g, uint8_t b);

        /* LCD驱动公共父类 */
        template <class T>
        class LCD_Base
        {
        public: /* typedef */
            typedef enum
            {
                ROTATE_0,
                ROTATE_90,
                ROTATE_180,
                ROTATE_270,
            } dir_e;

        protected: /* typedef */
            typedef enum
            {
                UNINITIALIZED,
                INIT,
            } status_e;

        protected:
            status_e status; /* 驱动器状态 */
            dir_e dir;       /* 显示方向 */
            bool isColorInverse;
            uint16_t width;
            uint16_t height;

        public: /* 接口 */
            LCD_Base(uint16_t width,
                     uint16_t height,
                     bool isColorInverse = false) : status(UNINITIALIZED),
                                                    dir(ROTATE_0),
                                                    width(width),
                                                    height(height),
                                                    isColorInverse(isColorInverse) {}
            /* system */
            virtual T *init() = 0;

            /* 绘图设置 */
            virtual T *on() = 0;
            virtual T *off() = 0;
            virtual T *setDir(dir_e) = 0; /* 设置显示方向 */
            virtual T *setBackLight(uint8_t) = 0;

            /* 绘图函数 */
            virtual T *drawPoint(uint16_t x, uint16_t y, uint16_t color) = 0;
            virtual T *drawPoints(uint16_t *px, uint16_t *py, uint16_t *pcolor, uint16_t dotNum) = 0;
            virtual T *drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) = 0;
            virtual T *drawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) = 0;
            virtual T *fillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) = 0;
            virtual T *drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) = 0;
            virtual T *fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) = 0;
            virtual T *fillScreen(uint16_t color) = 0;
        };

        /* 颜色转换 */
        uint16_t getRGB565(uint8_t r, uint8_t g, uint8_t b);
    } // namespace LCD
} // namespace Device

#endif //_DEVICE_LCD_BASE_HPP