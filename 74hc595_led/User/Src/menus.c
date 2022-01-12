/*******************************************************************************
 * @file        menus.c
 * @date        2022.1.12
 * @author      信念D力量
 * @brief       数码管菜单程序
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 ******************************************************************************/
#include "menus.h"
#include "74hc595_led.h"

menu_t menu_list[] = {
    {0, main_page},
    {1, fu_01},
    {2, fu_02},
    {3, fu_03},
    {4, fu_04},
};


/* 主页 */
void main_page(void)
{
    hc595_show_number(0);
}

/* 一级菜单 */
void fu_01(void)
{
    hc595_show_strings("FU01");
}

void fu_02(void)
{
    hc595_show_strings("FU02");
}

void fu_03(void)
{
    hc595_show_strings("FU03");
}

void fu_04(void)
{
    hc595_show_strings("FU04");
}

void fu_05(void)
{
    hc595_show_strings("FU05");
}

void menu_loop(void)
{
    
}