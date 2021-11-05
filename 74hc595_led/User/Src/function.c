/*******************************************************************************
 * @file        function.c
 * @date        2021.10.21
 * @author      ����D����
 * @brief       �˵�����Դ�ļ�
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 ******************************************************************************/
#include "function.h"
#include "74hc595_led.h"

uint8_t current_index = 0; /* ��ǰ���� */

void (*current_operation_index)();

typedef struct
{
    uint8_t current;    /* ��ǰ����ֵ */
    uint8_t up;         /* ���Ϸ����� */
    uint8_t down;       /* ���·����� */
    uint8_t enter;      /* ȷ�������� */
    void (*current_operation)();    /* ָ�뺯�� */
} menu_t;

menu_t menu_table[6] =
{
    {0,2,1,3, page_home},
    {1,0,2,6, page_test1},
    {2,1,0,6, page_test2}
};

/*******************************************************************************
 * @brief ��ҳ����
 * @brief ��ҳ��ʾһЩ�û���Ҫչʾ������
 ******************************************************************************/
void page_home(void)
{
    hc595_show_strings("FU-1");
}

void page_test1(void)
{
    hc595_show_strings("FU-2");
}
void page_test2(void)
{
    hc595_show_strings("FU-3");
}

