/*
 * @Author : Lu7fer
 * @Date: 2020-06-20 18:03:20
 * @LastEditTime: 2020-06-23 19:00:08
 * @FilePath: /Data-Storage-System/display.h
 * @Stu_ID: 2019X....X229_Lu7fer
 * @Github: https://github.com/Lu7fer/Data-Storage-System
 * @Copyright
-------------------------------------------
Copyright (C) 2020 - Lu7fer
Data-storage-System is free software:
you can redistribute it and/or modify it under the terms of 
the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
 
You should have received a copy of the GNU General Public License 
along with Data-Storage-System. 
If not, see <http: //www.gnu.org/licenses/>.
-------------------------------------------
 */

#ifndef _DISPLAY
#define _DISPLAY
#endif

#ifndef _INC_STDLIB

#include <stdlib.h>

#endif
#ifndef _INC_STDIO

#include <stdio.h>

#endif
/**
0 = 黑色       8 = 灰色
1 = 蓝色       9 = 淡蓝色
2 = 绿色       A = 淡绿色
3 = 浅绿色     B = 淡浅绿色
4 = 红色       C = 淡红色
5 = 紫色       D = 淡紫色
6 = 黄色       E = 淡黄色
7 = 白色       F = 亮白色
*/

/*显示相关*/
#define ALERT "COLOR CF"
#define NORMAL "COLOR 2E"
#define CLEAN_SCR "cls"
#define ALERT_TEXT 0x24
void dss_set_bgcolor(char *bg)
{
    system(bg);
}

void dss_cls()
{
    system(CLEAN_SCR);
}

#ifndef _INC_WINDOWS

#include <Windows.h>

#endif

void dss_blink()
{

    system(ALERT);
    Sleep(50);
    system(NORMAL);
    Sleep(50);
    system(ALERT);
    Sleep(50);
    system(NORMAL);
}
/*

oooooo   oooooo     oooo           oooo                                                  
 `888.    `888.     .8'            `888                                                  
  `888.   .8888.   .8'    .ooooo.   888   .ooooo.   .ooooo.  ooo. .oo.  .oo.    .ooooo.  
   `888  .8'`888. .8'    d88' `88b  888  d88' `"Y8 d88' `88b `888P"Y88bP"Y88b  d88' `88b 
    `888.8'  `888.8'     888ooo888  888  888       888   888  888   888   888  888ooo888 
     `888'    `888'      888    .o  888  888   .o8 888   888  888   888   888  888    .o 
      `8'      `8'       `Y8bod8P' o888o `Y8bod8P' `Y8bod8P' o888o o888o o888o `Y8bod8P' 
*/
#define WELCOME_LINES 7
#define WINDOW_COLS 120
#define WINDOW_LINES 30

HANDLE handle_out;               //定义一个句柄
CONSOLE_SCREEN_BUFFER_INFO scbi; //定义一个窗口缓冲区信息结构体

/*
 * update scbi
*/
void init()
{
    static int flag = 1;
    if (flag)
    {
        handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
        flag = 0;
    }
    GetConsoleScreenBufferInfo(handle_out, &scbi);

    //!   CloseHandle(handle_out); //关闭标准输出设备句柄
}

void dss_draw_line()
{
    init();
    fputc('\n', stdout);
    for (size_t i = 0; i < scbi.dwSize.X - 1; i++)
    {
        fputc('-', stdout);
    }
    fputc('\n', stdout);
    fflush(stdout);
}

/**
 *  ascii code draw
*/
char *_dss_welcome[] = {
    "   oooooo   oooooo     oooo           oooo                                                  ",
    "    `888.    `888.     .8'            `888                                                  ",
    "     `888.   .8888.   .8'    .ooooo.   888   .ooooo.   .ooooo.  ooo. .oo.  .oo.    .ooooo.  ",
    "      `888  .8'`888. .8'    d88' `88b  888  d88' `'Y8 d88 '`88b `888P'Y88bP'Y88b  d88' `88b ",
    "       `888.8'  `888.8'     888ooo888  888  888       888   888  888   888   888  888ooo888 ",
    "        `888'    `888'      888    .o  888  888   .o8 888   888  888   888   888  888    .o ",
    "         `8'      `8'       `Y8bod8P' o888o `Y8bod8P' `Y8bod8P' o888o o888o o888o `Y8bod8P' "};

void dss_welcome()
{
    dss_draw_line();
    for (size_t i = 0; i < WELCOME_LINES; i++)
    {
        puts(_dss_welcome[i]);
    }
    dss_draw_line();
}

void dss_reset_window()
{
    init();
    if (scbi.dwSize.X < WINDOW_LINES && scbi.dwSize.Y < WINDOW_COLS)
    {
        // SMALL_RECT rc = {1, 1, 120 - 1, 40 - 1}; // 重置窗口位置和大小
        // SetConsoleWindowInfo(handle_out, 1, &rc);
        char s[40];
        sprintf(s, "mode con cols=%d lines=%d", WINDOW_COLS, WINDOW_LINES);
        system(s);
    }
}


void dss_set_title(char *title)
{
    //设置窗口标题为“控制台窗口操作”
    SetConsoleTitle((LPCSTR)title);
}

void dss_get_title(char *title, u_int n)
{ //获得当前窗口标题
    GetConsoleTitle((LPSTR)title, n);
}

/**
 **从 stdio.h 抄一点源码,
 **程序可能会快一点
*/
void dss_println(const char *__format, ...)
{
    //丢掉__mingw_vfprintf返回值,本来也没打算用
    __builtin_va_list __local_argv;
    __builtin_va_start(__local_argv, __format);
    __mingw_vfprintf(stdout, __format, __local_argv);
    __builtin_va_end(__local_argv);
    putchar('\n');
}


void dss_colored_putln(char *out, const int color)
{
    init();
    SetConsoleTextAttribute(handle_out, color);
    puts(out);
    SetConsoleTextAttribute(handle_out, 0xE);
}

/**
0 = 黑色       8 = 灰色
1 = 蓝色       9 = 淡蓝色
2 = 绿色       A = 淡绿色
3 = 浅绿色     B = 淡浅绿色
4 = 红色       C = 淡红色
5 = 紫色       D = 淡紫色
6 = 黄色       E = 淡黄色
7 = 白色       F = 亮白色
*/
void dss_colored_put(char *out, const int color)
{
    init();
    SetConsoleTextAttribute(handle_out, color);
    fputs(out,stdout);
    SetConsoleTextAttribute(handle_out, 0xE);
}
void dss_colored_putchar(char out, const int color)
{
    init();
    SetConsoleTextAttribute(handle_out, color);
    putchar(out);
    SetConsoleTextAttribute(handle_out, 0xE);
}