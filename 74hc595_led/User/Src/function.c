/*******************************************************************************
 * @file        function.c
 * @date        2021.10.21
 * @author      信念D力量
 * @brief       菜单功能源文件
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 ******************************************************************************/
#include "function.h"
#include "74hc595_led.h"

uint8_t current_index = 0; /* 当前索引 */

void (*current_operation_index)();

typedef struct
{
    uint8_t current;    /* 当前索引值 */
    uint8_t up;         /* 向上翻索引 */
    uint8_t down;       /* 向下翻索引 */
    uint8_t enter;      /* 确认索引号 */
    void (*current_operation)();    /* 指针函数 */
} menu_t;

menu_t menu_table[6] =
{
    {0,2,1,3, page_home},
    {1,0,2,6, page_test1},
    {2,1,0,6, page_test2}
};

/*******************************************************************************
 * @brief 主页函数
 * @brief 主页显示一些用户需要展示的数据
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

