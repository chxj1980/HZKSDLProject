#include "font_12x24.h"
#include "sdl_font24.h"
#include "sdl_graphic.h"

// 点阵大小
#define CHARACTER_WIDTH 24
#include "font_size.h"

// ==========================================================================
//  各种取模方式
// ==========================================================================

// 逐行式，高位在前
static void display_lattice_row_high(int x, int y, unsigned char* lattice, int len)
{
    int tmp = 0;
    unsigned char* p = lattice;
    int column = len/HALFWIDTH_SIZE;
    for (int i = 0; i < column; i++)
    {
        for (int j = 0; j < CHARACTER_HALFWIDTH; j++)
        {
            // 3的由来：24/8 = 3
            tmp = (p[j*3+i*HALFWIDTH_SIZE + 2] << 0) | (p[j*3 +i*HALFWIDTH_SIZE + 1] << 8) | 
                (p[j*3 +i*HALFWIDTH_SIZE + 0] << 16);

            for (int k = 0; k < CHARACTER_HEIGHT; k++)
            {
                /* 逐位相与，为1者打印 */
                if(tmp & (0x800000>>k))
                    pixel(x + j + i*CHARACTER_HALFWIDTH, y + k, 0xffffff);
                else
                    pixel(x + j + i*CHARACTER_HALFWIDTH, y + k, 0x000000);
            }
        }
    }

    myrefresh();
}

// 逐行式，低位在前
static void display_lattice_row_low(int x, int y, unsigned char* lattice, int len)
{
    int tmp = 0;
    unsigned char* p = lattice;
    int column = len/HALFWIDTH_SIZE;
    for (int i = 0; i < column; i++)
    {
        for (int j = 0; j < CHARACTER_HALFWIDTH; j++)
        {
            // 3的由来：24/8 = 3
            tmp = (p[j*3+i*HALFWIDTH_SIZE + 0] << 0) | (p[j*3 +i*HALFWIDTH_SIZE + 1] << 8) | 
                (p[j*3 +i*HALFWIDTH_SIZE + 2] << 16);

            for (int k = 0; k < CHARACTER_HEIGHT; k++)
            {
                /* 逐位相与，为1者打印 */
                if(tmp & (0x000001<<k))
                    pixel(x + j + i*CHARACTER_HALFWIDTH, y + k, 0xffffff);
                else
                    pixel(x + j + i*CHARACTER_HALFWIDTH, y + k, 0x000000);
            }
        }
    }

    myrefresh();
}

// 逐列式，高位在前
static void display_lattice_column_high(int x, int y, unsigned char* lattice, int len)
{
    int tmp = 0;
    unsigned char* p = lattice;
    int column = len/HALFWIDTH_SIZE;
    for (int i = 0; i < column; i++)
    {
        for (int j = 0; j < CHARACTER_HALFWIDTH; j++)
        {
            // 3的由来：24/8 = 3
            tmp = (p[j*3+i*HALFWIDTH_SIZE + 2] << 0) | (p[j*3 +i*HALFWIDTH_SIZE + 1] << 8) | 
                (p[j*3 +i*HALFWIDTH_SIZE + 0] << 16);
            for (int k = 0; k < CHARACTER_HEIGHT; k++)
            {
                /* 逐位相与，为1者打印 */
                if(tmp & (0x800000>>k))
                    pixel(x + j + i*CHARACTER_HALFWIDTH, y + k, 0xffffff);
                else
                    pixel(x + j + i*CHARACTER_HALFWIDTH, y + k, 0x000000);
            }
        }
    }

    myrefresh();
}

// 逐列式，低位在前
static void display_lattice_column_low(int x, int y, unsigned char* lattice, int len)
{
    int tmp = 0;
    unsigned char* p = lattice;
    int column = len/HALFWIDTH_SIZE;
    for (int i = 0; i < column; i++)
    {
        for (int j = 0; j < CHARACTER_HALFWIDTH; j++)
        {
            // 3的由来：24/8 = 3
            tmp = (p[j*3+i*HALFWIDTH_SIZE + 0] << 0) | (p[j*3 +i*HALFWIDTH_SIZE + 1] << 8) | 
                (p[j*3 +i*HALFWIDTH_SIZE + 2] << 16);

            for (int k = 0; k < CHARACTER_HEIGHT; k++)
            {
                /* 逐位相与，为1者打印 */
                if(tmp & (0x000001<<k))
                    pixel(x + j + i*CHARACTER_HALFWIDTH, y + k, 0xffffff);
                else
                    pixel(x + j + i*CHARACTER_HALFWIDTH, y + k, 0x000000);
            }
        }
    }

    myrefresh();
}

// ==========================================================================
//  显示ASCII码(将ASCII保存成数组形式)/汉字
//  取模方式：逐列取模，低位在前
// ==========================================================================

// 全角24*24
// 一次打印一列
// 单个汉字
static void __display_font_column_24(int x, int y, unsigned char *mat)
{
    int i, j;
    int tmp = 0;
    unsigned char* p = mat;

    for(i = 0; i < CHARACTER_WIDTH; i++)
    {
        // 得到一列数据
        tmp = (p[i*3 + 0]) | (p[i*3 + 1] << 8) | (p[i*3 + 2] << 16);
        for (j = 0; j < CHARACTER_HEIGHT; j++)
        {
            /* 逐位相与，为1者打印 */
            if(tmp & (0x0000001<<j))
                pixel(x + i, y + j, 0xffffff);
            else
                pixel(x + i, y + j, 0x000000);
        }
    }

    myrefresh();
}

void display_font_column_24(int x, int y, unsigned char *mat, int num)
{
    while(num--)
    {
        __display_font_column_24(x, y, mat);
        mat += FULLWIDTH_SIZE;
        x += CHARACTER_WIDTH;
    }
}

// 半角24*12
// 一次打印一列
// 单个ASCII码
static void __display_ascii_column_24(int x, int y, unsigned char *mat)
{
    int i, j;
    int tmp = 0;
    unsigned char* p = (unsigned char *)mat;

    for(i = 0; i < CHARACTER_WIDTH/2; i++)
    {
        // 得到一列数据
        tmp = ((p[i*3 + 0]) | (p[i*3 + 1] << 8) | (p[i*3 + 2] << 16)) & 0x00ffffff;
        for (j = 0; j < CHARACTER_HEIGHT; j++)
        {
            /* 逐位相与，为1者打印 */
            if(tmp & (0x0000001<<j))
                pixel(x + i, y + j, 0xffffff);
            else
                pixel(x + i, y + j, 0x000000);
        }
    }

    myrefresh();
}

// 多个ASCII码
void display_ascii_column_24(int x, int y, char *ascii)
{
    int offset;
    unsigned char* p = (unsigned char*)ascii;
    unsigned char* p_ascii;
    while (*p != 0)
    {
        offset = (*p - 0x20) * HALFWIDTH_SIZE;
        p_ascii = ascii_fontdata24 + offset;
        __display_ascii_column_24(x, y, p_ascii);
        x += CHARACTER_WIDTH/2;
        p++;
    }
}

// ==========================================================================
//  测试示例
// ==========================================================================
static unsigned char p1[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x08,0x00,0x18,0x0C,0xFF,0xE0,0x04,
0x0C,0x00,0x00,0x08,0x00,0x00,0x14,0x18,0x00,0x62,0x30,0x04,0x86,0x40,0x3F,0x09,
0x80,0x04,0x11,0x80,0x04,0x63,0xE0,0x04,0x86,0x90,0x04,0x08,0x88,0x04,0x30,0x8C,
0x04,0x41,0x80,0x05,0x81,0x00,0x0E,0x0F,0x00,0x31,0x02,0x00,0x60,0xC0,0x00,0x00,
0x3F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,/*"逐",0*/

0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x03,0x00,0x18,0x06,0x1F,0xE0,0x0C,
0x00,0x00,0x19,0x00,0x00,0x21,0x80,0x00,0x01,0x00,0x0C,0x02,0x7F,0xFE,0x06,0x00,
0x80,0x07,0x00,0x80,0x0A,0x00,0x80,0x12,0x00,0x80,0x22,0x00,0x80,0x42,0x00,0x80,
0x02,0x00,0x80,0x02,0x00,0x80,0x02,0x00,0x80,0x02,0x00,0x80,0x02,0x00,0x80,0x02,
0x0F,0x80,0x02,0x03,0x80,0x00,0x00,0x00,/*"行",1*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x80,0x00,0x04,0x60,0x00,0x04,0x30,0x00,
0x04,0x30,0x00,0x04,0x0C,0x3F,0xFF,0xF0,0x00,0x06,0x00,0x00,0x06,0x00,0x00,0x06,
0x00,0x00,0x32,0x00,0x1F,0xC2,0x00,0x01,0x02,0x00,0x01,0x03,0x00,0x01,0x03,0x00,
0x01,0x01,0x00,0x01,0x01,0x84,0x01,0x38,0x84,0x03,0xC0,0xC4,0x7C,0x00,0x64,0x20,
0x00,0x3C,0x00,0x00,0x0C,0x00,0x00,0x00,/*"式",2*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x1E,0x00,0x00,0x1E,0x00,0x00,0x0C,0x00,0x00,0x04,0x00,0x00,0x08,0x00,0x00,0x10,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"，",3*/

0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x10,0x00,0x00,0x18,0x00,0x00,0x00,0x0C,0x1F,
0xFF,0xF0,0x02,0x00,0x80,0x03,0xFF,0xC0,0x03,0x00,0x80,0x03,0x00,0x80,0x03,0xFF,
0x80,0x02,0x00,0x80,0x10,0x00,0x08,0x1F,0xFF,0xF8,0x18,0x01,0x08,0x18,0xFF,0x08,
0x18,0x81,0x08,0x18,0x81,0x08,0x18,0x81,0x08,0x18,0xFF,0x08,0x18,0x00,0x08,0x18,
0x00,0xF8,0x18,0x00,0x18,0x00,0x00,0x00,/*"高",4*/

0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x08,0x00,0x03,0x0C,0x00,0x02,0x06,0x00,0x06,
0x02,0x00,0x04,0x02,0x18,0x04,0xFF,0xFC,0x0C,0x00,0x00,0x0C,0x00,0x60,0x14,0x40,
0x60,0x14,0x20,0x60,0x24,0x20,0x40,0x44,0x30,0x40,0x04,0x10,0x40,0x04,0x18,0x80,
0x04,0x18,0x80,0x04,0x18,0x80,0x04,0x11,0x00,0x04,0x01,0x00,0x04,0x01,0x04,0x05,
0xFF,0xFE,0x04,0x00,0x00,0x00,0x00,0x00,/*"位",5*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x30,0x00,0x00,0x30,0x00,0x00,
0x20,0x0C,0x3F,0xFF,0xF0,0x00,0x44,0x00,0x00,0x83,0x00,0x01,0x82,0x00,0x01,0x02,
0x00,0x06,0x02,0x00,0x06,0x02,0x18,0x0E,0xFF,0xF8,0x1E,0x02,0x00,0x26,0x02,0x00,
0x46,0x02,0x00,0x06,0x02,0x00,0x06,0x02,0x00,0x06,0x02,0x00,0x06,0x02,0x0C,0x07,
0xFF,0xFE,0x06,0x00,0x00,0x00,0x00,0x00,/*"在",6*/

0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x80,0x00,0xC3,0x00,0x00,0xC2,0x00,0x00,
0x44,0x0C,0x7F,0xBB,0xF0,0x00,0x00,0x00,0x00,0x00,0x30,0x0F,0xE2,0x30,0x08,0x23,
0x30,0x08,0x22,0x30,0x08,0x22,0x30,0x0F,0xE2,0x30,0x08,0x22,0x30,0x08,0x22,0x30,
0x08,0x22,0x30,0x0F,0xE2,0x30,0x08,0x23,0x30,0x08,0x22,0x30,0x08,0x20,0x30,0x09,
0x61,0xE0,0x08,0xC0,0x60,0x00,0x00,0x00,/*"前",7*/
};

static unsigned char p2[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x10,0x00,0x18,0x30,0xFF,0x07,0x20,
0x30,0x00,0x00,0x10,0x00,0x00,0x28,0x18,0x00,0x46,0x0C,0x20,0x61,0x02,0xFC,0x90,
0x01,0x20,0x88,0x01,0x20,0xC6,0x07,0x20,0x61,0x09,0x20,0x10,0x11,0x20,0x0C,0x31,
0x20,0x82,0x01,0xA0,0x81,0x00,0x70,0xF0,0x00,0x8C,0x40,0x00,0x06,0x03,0x00,0x00,
0xFC,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,/*"逐",0*/

0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0xC0,0x00,0x18,0x60,0xF8,0x07,0x30,
0x00,0x00,0x98,0x00,0x00,0x84,0x01,0x00,0x80,0x00,0x30,0x40,0xFE,0x7F,0x60,0x00,
0x01,0xE0,0x00,0x01,0x50,0x00,0x01,0x48,0x00,0x01,0x44,0x00,0x01,0x42,0x00,0x01,
0x40,0x00,0x01,0x40,0x00,0x01,0x40,0x00,0x01,0x40,0x00,0x01,0x40,0x00,0x01,0x40,
0xF0,0x01,0x40,0xC0,0x01,0x00,0x00,0x00,/*"行",1*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x01,0x00,0x20,0x06,0x00,0x20,0x0C,0x00,
0x20,0x0C,0x00,0x20,0x30,0xFC,0xFF,0x0F,0x00,0x60,0x00,0x00,0x60,0x00,0x00,0x60,
0x00,0x00,0x4C,0x00,0xF8,0x43,0x00,0x80,0x40,0x00,0x80,0xC0,0x00,0x80,0xC0,0x00,
0x80,0x80,0x00,0x80,0x80,0x21,0x80,0x1C,0x21,0xC0,0x03,0x23,0x3E,0x00,0x26,0x04,
0x00,0x3C,0x00,0x00,0x30,0x00,0x00,0x00,/*"式",2*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x78,0x00,0x00,0x78,0x00,0x00,0x30,0x00,0x00,0x20,0x00,0x00,0x10,0x00,0x00,0x08,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"，",3*/

0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x40,0x00,0x0E,0x60,0xC0,0x03,0x20,
0xFE,0x00,0x30,0xC2,0x00,0x10,0x82,0x00,0x38,0x82,0x00,0x38,0x82,0x00,0x34,0x82,
0x10,0x32,0xFE,0x3F,0x32,0x82,0x00,0x30,0x82,0x00,0x30,0x82,0x01,0x30,0x02,0x01,
0x30,0x02,0x01,0x30,0x42,0x03,0x30,0x32,0x22,0x30,0x0A,0x26,0x30,0x36,0x2C,0x30,
0x42,0x38,0x30,0x40,0x20,0x00,0x00,0x00,/*"低",4*/

0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x10,0x00,0xC0,0x30,0x00,0x40,0x60,0x00,0x60,
0x40,0x00,0x20,0x40,0x18,0x20,0xFF,0x3F,0x30,0x00,0x00,0x30,0x00,0x06,0x28,0x02,
0x06,0x28,0x04,0x06,0x24,0x04,0x02,0x22,0x0C,0x02,0x20,0x08,0x02,0x20,0x18,0x01,
0x20,0x18,0x01,0x20,0x18,0x01,0x20,0x88,0x00,0x20,0x80,0x00,0x20,0x80,0x20,0xA0,
0xFF,0x7F,0x20,0x00,0x00,0x00,0x00,0x00,/*"位",5*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x0C,0x00,0x00,0x0C,0x00,0x00,
0x04,0x30,0xFC,0xFF,0x0F,0x00,0x22,0x00,0x00,0xC1,0x00,0x80,0x41,0x00,0x80,0x40,
0x00,0x60,0x40,0x00,0x60,0x40,0x18,0x70,0xFF,0x1F,0x78,0x40,0x00,0x64,0x40,0x00,
0x62,0x40,0x00,0x60,0x40,0x00,0x60,0x40,0x00,0x60,0x40,0x00,0x60,0x40,0x30,0xE0,
0xFF,0x7F,0x60,0x00,0x00,0x00,0x00,0x00,/*"在",6*/

0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x01,0x00,0xC3,0x00,0x00,0x43,0x00,0x00,
0x22,0x30,0xFE,0xDD,0x0F,0x00,0x00,0x00,0x00,0x00,0x0C,0xF0,0x47,0x0C,0x10,0xC4,
0x0C,0x10,0x44,0x0C,0x10,0x44,0x0C,0xF0,0x47,0x0C,0x10,0x44,0x0C,0x10,0x44,0x0C,
0x10,0x44,0x0C,0xF0,0x47,0x0C,0x10,0xC4,0x0C,0x10,0x44,0x0C,0x10,0x04,0x0C,0x90,
0x86,0x07,0x10,0x03,0x06,0x00,0x00,0x00,/*"前",7*/
};

static unsigned char p3[] = {
0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x20,0x18,0x20,0x20,0x10,0x18,0x20,0x20,0x0C,
0x7F,0xE0,0x00,0x20,0x20,0x00,0x20,0x50,0x08,0x44,0x48,0x08,0x88,0x88,0x08,0x89,
0x04,0x09,0x11,0x04,0x0E,0x22,0x24,0x0D,0x44,0x24,0x08,0xCC,0x34,0x08,0x38,0xE4,
0x08,0x3F,0x84,0x08,0x48,0x04,0x08,0x88,0x04,0x11,0x84,0x04,0x11,0x03,0x04,0x00,
0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x00,/*"逐",0*/

0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x02,0x02,0x08,0x04,0x04,0x08,0x18,0x04,0x08,
0x70,0x08,0x0B,0xCC,0x10,0x0E,0x47,0x20,0x08,0x40,0xC0,0x08,0x43,0x80,0x08,0x5E,
0x00,0x08,0xF0,0x00,0x10,0xC0,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x60,
0x03,0xFF,0xC0,0x00,0x00,0x04,0x00,0x00,0x04,0x00,0x00,0x06,0x3F,0xFF,0xFE,0x20,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"列",1*/

0x00,0x00,0x00,0x00,0x00,0x08,0x01,0x00,0x0C,0x01,0x08,0x08,0x01,0x08,0x08,0x01,
0x08,0x08,0x01,0x08,0x10,0x01,0x0F,0xF0,0x01,0x08,0x10,0x01,0x08,0x10,0x01,0x10,
0x20,0x01,0x10,0x20,0x01,0x00,0x20,0x3F,0xE0,0x00,0x21,0xFF,0x00,0x01,0x03,0xC0,
0x21,0x00,0x70,0x11,0x00,0x18,0x1D,0x00,0x0C,0x0D,0x00,0x04,0x02,0x00,0x06,0x02,
0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,/*"式",2*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC4,0x00,0x00,0xE8,0x00,
0x00,0xF0,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"，",3*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x0F,0xFE,0x04,0x07,0xFE,0x04,
0x04,0x00,0x07,0xF4,0x00,0x05,0xE4,0x00,0x05,0x25,0xF0,0x05,0x25,0x10,0x45,0x25,
0x10,0x35,0x25,0x10,0x15,0x25,0x10,0x05,0x25,0x10,0x05,0x25,0x10,0x05,0x27,0xF0,
0x07,0xF4,0x04,0x05,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x06,0x08,0x0F,0xFE,0x08,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"高",4*/

0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x08,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x07,
0xFF,0xFE,0x3C,0x00,0x00,0x30,0x00,0x04,0x01,0x00,0x04,0x01,0x20,0x04,0x01,0x1C,
0x04,0x01,0x07,0xE4,0x31,0x01,0xC4,0x19,0x00,0x04,0x0F,0x00,0x04,0x01,0x00,0x3C,
0x01,0x01,0xC4,0x01,0x7E,0x04,0x01,0x70,0x04,0x03,0x00,0x04,0x03,0x00,0x04,0x01,
0x00,0x0C,0x00,0x00,0x04,0x00,0x00,0x00,/*"位",5*/

0x00,0x00,0x00,0x00,0x00,0x80,0x02,0x01,0x00,0x02,0x02,0x00,0x02,0x06,0x00,0x02,
0x1F,0xFE,0x02,0x1F,0xFE,0x02,0x60,0x04,0x02,0xC4,0x04,0x03,0x04,0x04,0x1E,0x04,
0x04,0x3A,0x04,0x04,0x22,0x04,0x04,0x03,0x04,0x04,0x02,0xFF,0xFC,0x02,0x84,0x04,
0x02,0x04,0x04,0x02,0x04,0x04,0x02,0x04,0x04,0x02,0x0C,0x04,0x04,0x0C,0x0C,0x04,
0x00,0x0C,0x00,0x00,0x04,0x00,0x00,0x00,/*"在",6*/

0x00,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,0x7F,0xFE,0x02,
0x44,0x40,0x02,0x44,0x40,0x22,0x44,0x44,0x1A,0x44,0x42,0x1C,0x44,0x46,0x02,0x7F,
0xFC,0x02,0x00,0x00,0x02,0x00,0x00,0x04,0x00,0x00,0x1A,0x7F,0xF0,0x32,0x20,0x24,
0x22,0x00,0x04,0x02,0x00,0x06,0x02,0xFF,0xFE,0x02,0xFF,0xF8,0x04,0x00,0x00,0x04,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"前",7*/
};

static unsigned char p4[] = {
0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x04,0x18,0x04,0x04,0x08,0x18,0x04,0x04,0x30,
0xFE,0x07,0x00,0x04,0x04,0x00,0x04,0x0A,0x10,0x22,0x12,0x10,0x11,0x11,0x10,0x91,
0x20,0x90,0x88,0x20,0x70,0x44,0x24,0xB0,0x22,0x24,0x10,0x33,0x2C,0x10,0x1C,0x27,
0x10,0xFC,0x21,0x10,0x12,0x20,0x10,0x11,0x20,0x88,0x21,0x20,0x88,0xC0,0x20,0x00,
0x80,0x20,0x00,0x00,0x00,0x00,0x00,0x00,/*"逐",0*/

0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x40,0x40,0x10,0x20,0x20,0x10,0x18,0x20,0x10,
0x0E,0x10,0xD0,0x33,0x08,0x70,0xE2,0x04,0x10,0x02,0x03,0x10,0xC2,0x01,0x10,0x7A,
0x00,0x10,0x0F,0x00,0x08,0x03,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x06,
0xC0,0xFF,0x03,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x60,0xFC,0xFF,0x7F,0x04,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"列",1*/

0x00,0x00,0x00,0x00,0x00,0x10,0x80,0x00,0x30,0x80,0x10,0x10,0x80,0x10,0x10,0x80,
0x10,0x10,0x80,0x10,0x08,0x80,0xF0,0x0F,0x80,0x10,0x08,0x80,0x10,0x08,0x80,0x08,
0x04,0x80,0x08,0x04,0x80,0x00,0x04,0xFC,0x07,0x00,0x84,0xFF,0x00,0x80,0xC0,0x03,
0x84,0x00,0x0E,0x88,0x00,0x18,0xB8,0x00,0x30,0xB0,0x00,0x20,0x40,0x00,0x60,0x40,
0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,/*"式",2*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x23,0x00,0x00,0x17,0x00,
0x00,0x0F,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"，",3*/

0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x04,0x00,0x00,0x03,0x00,0xC0,0xFF,0x7F,0x70,
0xFF,0x7F,0x1C,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0x3F,0x20,0x08,
0x10,0x20,0x08,0x08,0x20,0x08,0x14,0x20,0x08,0x14,0x70,0x08,0x62,0xF0,0x7F,0x00,
0x10,0xC8,0x03,0x18,0x08,0x0E,0x08,0x08,0x18,0x08,0x08,0x30,0x00,0x0C,0x20,0x00,
0x08,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,/*"低",4*/

0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x10,0x00,0x00,0x0C,0x00,0x00,0x03,0x00,0xE0,
0xFF,0x7F,0x3C,0x00,0x00,0x0C,0x00,0x20,0x80,0x00,0x20,0x80,0x04,0x20,0x80,0x38,
0x20,0x80,0xE0,0x27,0x8C,0x80,0x23,0x98,0x00,0x20,0xF0,0x00,0x20,0x80,0x00,0x3C,
0x80,0x80,0x23,0x80,0x7E,0x20,0x80,0x0E,0x20,0xC0,0x00,0x20,0xC0,0x00,0x20,0x80,
0x00,0x30,0x00,0x00,0x20,0x00,0x00,0x00,/*"位",5*/

0x00,0x00,0x00,0x00,0x00,0x01,0x40,0x80,0x00,0x40,0x40,0x00,0x40,0x60,0x00,0x40,
0xF8,0x7F,0x40,0xF8,0x7F,0x40,0x06,0x20,0x40,0x23,0x20,0xC0,0x20,0x20,0x78,0x20,
0x20,0x5C,0x20,0x20,0x44,0x20,0x20,0xC0,0x20,0x20,0x40,0xFF,0x3F,0x40,0x21,0x20,
0x40,0x20,0x20,0x40,0x20,0x20,0x40,0x20,0x20,0x40,0x30,0x20,0x20,0x30,0x30,0x20,
0x00,0x30,0x00,0x00,0x20,0x00,0x00,0x00,/*"在",6*/

0x00,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,0xFE,0x7F,0x40,
0x22,0x02,0x40,0x22,0x02,0x44,0x22,0x22,0x58,0x22,0x42,0x38,0x22,0x62,0x40,0xFE,
0x3F,0x40,0x00,0x00,0x40,0x00,0x00,0x20,0x00,0x00,0x58,0xFE,0x0F,0x4C,0x04,0x24,
0x44,0x00,0x20,0x40,0x00,0x60,0x40,0xFF,0x7F,0x40,0xFF,0x1F,0x20,0x00,0x00,0x20,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"前",7*/
};

void font_24_test(void)
{
    display_lattice_row_high(0, 200, p1, sizeof(p1));
    display_lattice_row_low(0, 200+30, p2, sizeof(p2));
    display_lattice_column_high(0, 200+60, p3, sizeof(p3));
    display_lattice_column_low(0, 200+90, p4, sizeof(p4));

    display_ascii_column_24(0, 200+120, "hello world! Goodbye!");
    display_font_column_24(0, 200+150, p4, sizeof(p4)/FULLWIDTH_SIZE);
}

#include <time.h>

void show_time(void)
{
#define X 5
#define Y 250

#define offsetYear      0
#define offsetMonth     5
#define offsetDay       8
#define offsetHour      11
#define offsetMinute    14
#define offsetSecond    17

    char szTime[32] = {0};
    time_t iTime;
    struct tm *TimeInfo = NULL;
    static bool fOnlyOnce = true;
    static char chDateFormat = '-';

    static int iYear = 0;
    static int iMonth = 0;
    static int iDay = 0;
    static int iHour = 0;
    static int iMinute = 0;
    static int iSecond = 0;

    iTime = time(NULL);
    TimeInfo = localtime(&iTime);

    if (fOnlyOnce)
    {
        iYear = TimeInfo->tm_year + 1900;
        iMonth = TimeInfo->tm_mon + 1;
        iDay = TimeInfo->tm_mday;
        iHour = TimeInfo->tm_hour;
        iMinute = TimeInfo->tm_min;
        iSecond = TimeInfo->tm_sec;
        sprintf(szTime, "%04d%c%02d%c%02d %02d:%02d:%02d ", iYear, chDateFormat, iMonth, chDateFormat, iDay, iHour, iMinute, iSecond);
        display_ascii_column_24(X, Y, szTime);
        fOnlyOnce = false;
    }

    if (iYear != TimeInfo->tm_year + 1900)
    {
        iYear = TimeInfo->tm_year + 1900;
        sprintf(szTime, "%04d", iYear);
        display_ascii_column_24(X, Y, szTime);
    }

    if (iMonth != TimeInfo->tm_mon + 1)
    {
        iMonth = TimeInfo->tm_mon + 1;
        sprintf(szTime, "%02d", iMonth);
        display_ascii_column_24(X + offsetMonth * CHARACTER_HALFWIDTH, Y, szTime);
    }

    if (iDay != TimeInfo->tm_mday)
    {
        iDay = TimeInfo->tm_mday;
        sprintf(szTime, "%02d", iDay);
        display_ascii_column_24(X + offsetDay * CHARACTER_HALFWIDTH, Y, szTime);
    }

    if (iHour != TimeInfo->tm_hour)
    {
        iHour = TimeInfo->tm_hour;
        sprintf(szTime, "%02d", iHour);
        display_ascii_column_24(X + offsetHour * CHARACTER_HALFWIDTH, Y, szTime);
    }

    if (iMinute != TimeInfo->tm_min)
    {
        iMinute = TimeInfo->tm_min;
        //printf("minute: %02d\n", iMinute);
        sprintf(szTime, "%02d", iMinute);
        display_ascii_column_24(X + offsetMinute * CHARACTER_HALFWIDTH, Y, szTime);
    }

    if (iSecond != TimeInfo->tm_sec)
    {
        iSecond = TimeInfo->tm_sec;
        //printf("second: %02d\n", iSecond);
        sprintf(szTime, "%02d", iSecond);
        display_ascii_column_24(X + offsetSecond * CHARACTER_HALFWIDTH, Y, szTime);
    }
}
