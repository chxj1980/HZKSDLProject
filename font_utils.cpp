#include <stdlib.h> 
#include "font_utils.h"
#include "sdl_graphic.h"
#include "font_12x24.h"
#include "ascii_16x32.h"
#include "ascii_36x72.h"

// ����ת����
// ԭ(����ʽ����λ��ǰ��8����ռ1�ֽ�) -> Ŀ��(1����ռ1�ֽڣ��µ�������ң����ϵ������С���ռ�ô�СΪwidth * height�ֽ�)
// width��height���µ���Ŀ���(����Ԥ�������)
static int change_lattice(unsigned char* old_lattice, unsigned char* new_lattice, int width, int height)
{
    unsigned char tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
    int offset_byte = 0;    // ������ռ�ֽ�ƫ��
    unsigned char* p = NULL;
    unsigned char* q = NULL;

    if (old_lattice == NULL || new_lattice == NULL)
        return -1;
    p = old_lattice;
    q = new_lattice;

    for(int i = 0; i < width; i++)
    {
        for (int j = 0; j < height/8; j++)
        {
            offset_byte = j*8;
            // һ���ֽ�
            /// ��λ��ǰ
            tmp1 = (*p & 0x01) >> 0;
            tmp2 = (*p & 0x02) >> 1;
            tmp3 = (*p & 0x04) >> 2;
            tmp4 = (*p & 0x08) >> 3;
            tmp5 = (*p & 0x10) >> 4;
            tmp6 = (*p & 0x20) >> 5;
            tmp7 = (*p & 0x40) >> 6;
            tmp8 = (*p & 0x80) >> 7;
            p++;
            // height/8���ֽ�
            q[width * (0+offset_byte) + i] = tmp1;
            q[width * (1+offset_byte) + i] = tmp2;
            q[width * (2+offset_byte) + i] = tmp3;
            q[width * (3+offset_byte) + i] = tmp4;
            q[width * (4+offset_byte) + i] = tmp5;
            q[width * (5+offset_byte) + i] = tmp6;
            q[width * (6+offset_byte) + i] = tmp7;
            q[width * (7+offset_byte) + i] = tmp8;
        }
    }
    return 0;
}

/*
my_ascii_to_lattice - ASCII�ַ�ת���ɵ���
fontdata�������ֿ�ָ��
character_width��������(����32*32�ĵ��󣬿����32)
strings���ַ���
lattice��ת����ĵ�������
w/h��ת����ĵ������ݵĿ���
ע��fontdataȡģ��' '��ʼ(ǰ32���ַ��ǲ��ɴ�ӡ�ַ�)��ʹ��PCtoLCD2002.exe�����ȡģ��ʽ������ʽ����λ��ǰ
*/
static void my_ascii_to_lattice(unsigned char* fontdata, int char_width, char* strings, unsigned char* lattice, int* w, int* h)
{
#define MY_ALIGN(x, a) (((x)+(a)-1)&~((a)-1))          // �ֽڶ���(�˴�����ֻʹ��8�ֽڶ���)

	unsigned char tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
	int offset_byte = 0;    // ������ռ�ֽ�ƫ��
	char* q = NULL;
	int i,j, k;
	int len = 0;

	int width = 0;
	int height = 0;

	int half_width = char_width / 2; // ���
	int char_height = MY_ALIGN(char_width, 8); // ��(������8��������)
	int char_halfwidth_size = MY_ALIGN(char_width, 8)*char_width/2/8; // ��ǵ����ܴ�С

	if (fontdata == NULL || strings == NULL || lattice == NULL || w == NULL || h == NULL) return;

	len = (int)strlen((const char *)strings);
	width = len * half_width;
	height = char_height;
	q = (char*)lattice;

	*w = width;
	*h = height;

	// ���ٸ��ַ�
	for ( i = 0; i < len; ++i )
	{
		// ǰ��32(0x20)���ǲ��ɴ�ӡ�ַ�����Ҫ��ȥ0x20
		unsigned char* p = fontdata + ((strings[i] - 0x20) * char_halfwidth_size);

		// ÿ���ַ����ǰ��
		for ( j = 0; j < half_width; ++j )
		{
			int iDstLine = 0;

			// ÿ���ַ����ж��ٸ��ֽ�
			for (k = 0; k < char_height/8; k++)
			{
				offset_byte = k * 8;
				// �ֽ����һ���ֽ����е�λ
				// ע�� ��λ��ǰ
				tmp1 = (*p & 0x01) >> 0;
				tmp2 = (*p & 0x02) >> 1;
				tmp3 = (*p & 0x04) >> 2;
				tmp4 = (*p & 0x08) >> 3;
				tmp5 = (*p & 0x10) >> 4;
				tmp6 = (*p & 0x20) >> 5;
				tmp7 = (*p & 0x40) >> 6;
				tmp8 = (*p & 0x80) >> 7;

				// height/8���ֽ�
				q[width * (0+offset_byte) + half_width * i + j] = tmp1;
				q[width * (1+offset_byte) + half_width * i + j] = tmp2;
				q[width * (2+offset_byte) + half_width * i + j] = tmp3;
				q[width * (3+offset_byte) + half_width * i + j] = tmp4;
				q[width * (4+offset_byte) + half_width * i + j] = tmp5;
				q[width * (5+offset_byte) + half_width * i + j] = tmp6;
				q[width * (6+offset_byte) + half_width * i + j] = tmp7;
				q[width * (7+offset_byte) + half_width * i + j] = tmp8;

				p++;
			}
		}
	}
}

// ����
static void display(int x, int y, unsigned char* lattice, int width, int height)
{
    char tmp = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //tmp = lattice[i * width + j];
            tmp = *lattice++;
            if (tmp & 0x01)
                pixel(x + j, y + i, 0);
            else
                pixel(x + j, y + i, 0xffffff);
            //Sleep(10);
            //myrefresh();
        }
        //Sleep(1000);
        //myrefresh();
    }
    myrefresh();
}

// ����
static void display1(int x, int y, unsigned char* lattice, int width, int height)
{
    char tmp = 0;
    for (int i = 0; i < width; i++)
    {
        // ��ʾһ��
        for (int j = 0; j < height; j++)
        {
            tmp = lattice[j * width + i];
            if (tmp & 0x01)
                pixel(x + i, y + j, 0xffffff);
            //Sleep(10);
            //myrefresh();
        }
        Sleep(30);
        myrefresh();
    }
    myrefresh();
}

// ����ת�������
void foo_test(void)
{
#include "font_data.h"

#define CHARACTER_WIDTH 72
#include "font_size.h"

	int width, height;
    int len;
    unsigned char* p = NULL;

    len = sizeof(p72);
    width = len / HALFWIDTH_SIZE * CHARACTER_HALFWIDTH;
    height = CHARACTER_HEIGHT;

    p = (unsigned char*)malloc(width*height);
    if (p == NULL)
        return;
    memset(p, '\0', width*height);

	printf("len: %d\n", width*height);
    //change_lattice((unsigned char*)myfont, p, width, height);
    //FILE* fp;
    //fp = fopen("fon1.bin", "wb");
    //fwrite(p, 1, width*height, fp);
    //fclose(fp);

    change_lattice((unsigned char*)p72, p, width, height);
    display1(350, 405, p, width, height);


    free(p);
}

/////////////////// asciiת���������
void bar_test(void)
{
	char* p = "2013-07-14 18:00:888";
    unsigned char* q = NULL;
    int width, height;
    int len = (int)strlen(p);
    q = (unsigned char *)malloc(1024*1024);
    memset(q, '\0', 1024*1024);
    //ascii_to_lattice32((unsigned char *)p, q, &width, &height);

	my_ascii_to_lattice(ascii_fontdata72, 72, p, q, &width, &height);

	printf("width: %d height: %d\n", width, height);

    display(0, 0, q, width, height);
}