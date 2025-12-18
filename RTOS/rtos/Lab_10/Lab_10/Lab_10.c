#include <stdlib.h>
#include "pico/stdlib.h"
#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "OLED_1in5.h"
#include "Debug.h"

int main(void)
{
    DEV_Delay_ms(100);

    if (DEV_Module_Init() != 0)
    {
        while (1)
        {
            printf("END\r\n");
        }
    }
    printf("OELD_test Demo\r\n");
    /* Init */
    OLED_1in5_Init();
    OLED_1in5_Clear(0);

    UBYTE *BlackImage;
    UWORD Imagesize = ((OLED_1in5_WIDTH % 2 == 0) ? (OLED_1in5_WIDTH / 2) : (OLED_1in5_WIDTH / 2 + 1)) * OLED_1in5_HEIGHT;
    if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL)
    {
        while (1)
        {
            printf("Failed to apply for black memory...\r\n");
        }
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, OLED_1in5_WIDTH, OLED_1in5_HEIGHT, 0,
                   WHITE);
    Paint_SetScale(16);

    printf("Drawing\r\n");
    // 1.Select Image
    Paint_SelectImage(BlackImage);
    DEV_Delay_ms(500);
    Paint_Clear(BLACK);

    // 2.Drawing on the image
    printf("Drawing:page 1\r\n");
    Paint_DrawPoint(20, 10, WHITE, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(30, 10, WHITE, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(40, 10, WHITE, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(10, 10, 10, 20, WHITE, DOT_PIXEL_1X1,
                   LINE_STYLE_SOLID);
    Paint_DrawLine(20, 20, 20, 30, WHITE, DOT_PIXEL_1X1,
                   LINE_STYLE_SOLID);
    Paint_DrawLine(30, 30, 30, 40, WHITE, DOT_PIXEL_1X1,
                   LINE_STYLE_DOTTED);
    Paint_DrawLine(40, 40, 40, 50, WHITE, DOT_PIXEL_1X1,
                   LINE_STYLE_DOTTED);
    Paint_DrawCircle(60, 30, 15, WHITE, DOT_PIXEL_1X1,
                     DRAW_FILL_EMPTY);
    Paint_DrawCircle(100, 40, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawRectangle(50, 30, 60, 40, WHITE, DOT_PIXEL_1X1,
                        DRAW_FILL_EMPTY);
    Paint_DrawRectangle(90, 30, 110, 50, BLACK, DOT_PIXEL_1X1,
                        DRAW_FILL_FULL);
    // 3. Show image on page1

    OLED_1in5_Display(BlackImage);
    DEV_Delay_ms(10000);
    Paint_Clear(BLACK);
    Paint_DrawString_EN(5, 10, "128 x 128 Pixels", &Font12, WHITE, BLACK);
    Paint_DrawString_EN(5, 25, "OLED-1.5-Demo", &Font12, WHITE, BLACK);
    Paint_DrawString_EN(5, 40, "Waveshare", &Font12, WHITE, BLACK);
    Paint_DrawRectangle(0, 55, OLED_1in5_WIDTH - 1, 65, 0x01,
                        DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawRectangle(0, 65, OLED_1in5_WIDTH - 1, 75, 0x02,
                        DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawRectangle(0, 75, OLED_1in5_WIDTH - 1, 85, 0x04,
                        DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawRectangle(0, 85, OLED_1in5_WIDTH - 1, 95, 0x08,
                        DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawRectangle(0, 95, OLED_1in5_WIDTH - 1, 105, 0x0A,
                        DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawRectangle(0, 105, OLED_1in5_WIDTH - 1, 115, 0x0C,
                        DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawRectangle(0, 115, OLED_1in5_WIDTH - 1, 125, 0x0F,
                        DOT_PIXEL_1X1, DRAW_FILL_FULL);
    OLED_1in5_Display(BlackImage);
    DEV_Module_Exit();
    while (true)
    {
    }
}