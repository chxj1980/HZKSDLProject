//#ifndef _FONT_SIZE_H
//#define _FONT_SIZE_H

/**
 * �������ļ�ǰ���붨��CHARACTER_WIDTH
 * ������˵�ġ�����ָȫ���ַ����ԣ���18*18����ȫ���ַ���Ϊ18����Ϊ18��
 * ����ַ���Ϊ9����Ϊ18(����ռ�ÿռ�Ϊ3�ֽڣ���24λ)����ʼ��8�ֽڶ��롣
 */

#define ALIGN(x, a) (((x)+(a)-1)&~((a)-1))          // �ֽڶ���(�˴�����ֻʹ��8�ֽڶ���)
#define CHARACTER_HALFWIDTH (CHARACTER_WIDTH/2)     // ��ǿ��
#define CHARACTER_HEIGHT ALIGN(CHARACTER_WIDTH, 8)  // ����߶�
#define FULLWIDTH_SIZE (ALIGN(CHARACTER_WIDTH, 8)*CHARACTER_WIDTH/8)    // ȫ��ռ���ֽ���
#define HALFWIDTH_SIZE (ALIGN(CHARACTER_WIDTH, 8)*CHARACTER_WIDTH/2/8)  // ���ռ���ֽ���

// ���󻺳����������ܴ�С/��Ǵ�С * ��ǿ�
//#endif /* _FONT_SIZE_H */