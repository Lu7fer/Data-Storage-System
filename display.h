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
0 = ��ɫ       8 = ��ɫ
1 = ��ɫ       9 = ����ɫ
2 = ��ɫ       A = ����ɫ
3 = ǳ��ɫ     B = ��ǳ��ɫ
4 = ��ɫ       C = ����ɫ
5 = ��ɫ       D = ����ɫ
6 = ��ɫ       E = ����ɫ
7 = ��ɫ       F = ����ɫ
*/

/*��ʾ���*/
#define ALERT "COLOR CF"
#define NORMAL "COLOR 2E"
#define CLEAN_SCR "cls"

#define NORMAL_TEXT 0x2E
#define ALERT_TEXT 0x24
#define DISABLE_TEXT 0x28

void dss_set_global_color(char *bg) {
    system(bg);
}

void dss_cls() {
    system(CLEAN_SCR);
}

#ifndef _INC_WINDOWS

#include <Windows.h>

#endif

COORD dss_get_cursor_location();

void dss_blink() {

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
#define THINKS_LINES 6
#define WINDOW_COLS 120
#define WINDOW_LINES 30

HANDLE handle_out;               //����һ�����
CONSOLE_SCREEN_BUFFER_INFO scbi; //����һ�����ڻ�������Ϣ�ṹ��

/*
 * update scbi
*/
void init() {
    static int flag = 1;
    if (flag) {
        handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
        flag = 0;
    }
    GetConsoleScreenBufferInfo(handle_out, &scbi);

    //!   CloseHandle(handle_out); //�رձ�׼����豸���
}

void dss_draw_line() {
    init();
    if (dss_get_cursor_location().X!=0)
        fputc('\n', stdout);
    for (size_t i = 0; i < scbi.dwSize.X - 1; i++) {
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

void dss_welcome() {
    dss_cls();
    dss_draw_line();
    for (size_t i = 0; i < WELCOME_LINES; i++) {
        puts(_dss_welcome[i]);
    }
    dss_draw_line();
}

/**

.___________. __    __       ___      .__   __.  __  ___      _______.
|           ||  |  |  |     /   \     |  \ |  | |  |/  /     /       |
`---|  |----`|  |__|  |    /  ^  \    |   \|  | |  '  /     |   (----`
    |  |     |   __   |   /  /_\  \   |  . `  | |    <       \   \
    |  |     |  |  |  |  /  _____  \  |  |\   | |  .  \  .----)   |
    |__|     |__|  |__| /__/     \__\ |__| \__| |__|\__\ |_______/

 */
char *_dss_thanks[] = {
        "   .___________. __    __       ___      .__   __.  __  ___      _______. ",
        "   |           ||  |  |  |     /   \\     |  \\ |  | |  |/  /     /       | ",
        "   `---|  |----`|  |__|  |    /  ^  \\    |   \\|  | |  '  /     |   (----` ",
        "       |  |     |   __   |   /  /_\\  \\   |  . `  | |    <       \\   \\ ",
        "       |  |     |  |  |  |  /  _____  \\  |  |\\   | |  .  \\  .----)   | ",
        "       |__|     |__|  |__| /__/     \\__\\ |__| \\__| |__|\\__\\ |_______/ "

};

void dss_thanks() {
    dss_cls();
    dss_draw_line();
    for (size_t i = 0; i < THINKS_LINES; i++) {
        puts(_dss_thanks[i]);
    }
    dss_draw_line();
}

void dss_print_space(u_int count) {
    for (int j = 0; j < count / 10; ++j) {
        printf("          ");
    }
    for (int i = 0; i < count % 10; ++i) {
        putchar(' ');
    }
}


void dss_reset_window() {
    init();
//    if (scbi.dwSize.X < WINDOW_LINES && scbi.dwSize.Y < WINDOW_COLS) {
    // SMALL_RECT rc = {1, 1, 120 - 1, 40 - 1}; // ���ô���λ�úʹ�С
    // SetConsoleWindowInfo(handle_out, 1, &rc);
    char s[40];
    sprintf(s, "mode con cols=%d lines=%d", WINDOW_COLS, WINDOW_LINES);
    system(s);
    dss_set_global_color(NORMAL);
//    }
}

COORD dss_get_window_size() {
    init();
    return scbi.dwSize;
}

COORD dss_get_cursor_location() {
    init();
    return scbi.dwCursorPosition;
}

void dss_set_title(char *title) {
    //���ô��ڱ���Ϊ������̨���ڲ�����
    SetConsoleTitle((LPCSTR) title);
}

void dss_get_title(char *title, u_int n) { //��õ�ǰ���ڱ���
    GetConsoleTitle((LPSTR) title, n);
}

/**
 **�� stdio.h ��һ��Դ��,
 **������ܻ��һ��
*/
void dss_println(const char *__format, ...) {
    //����__mingw_vfprintf����ֵ,����Ҳû������
    __builtin_va_list __local_argv;
    __builtin_va_start(__local_argv, __format);
    __mingw_vfprintf(stdout, __format, __local_argv);
    __builtin_va_end(__local_argv);
    putchar('\n');
}


void dss_colored_putln(char *out, const int color) {
    init();
    SetConsoleTextAttribute(handle_out, color);
    puts(out);
    SetConsoleTextAttribute(handle_out, 0x2E);
}
void dss_set_cursor_location(COORD coord){
    SetConsoleCursorPosition(handle_out,coord);
}

/**
0 = ��ɫ       8 = ��ɫ
1 = ��ɫ       9 = ����ɫ
2 = ��ɫ       A = ����ɫ
3 = ǳ��ɫ     B = ��ǳ��ɫ
4 = ��ɫ       C = ����ɫ
5 = ��ɫ       D = ����ɫ
6 = ��ɫ       E = ����ɫ
7 = ��ɫ       F = ����ɫ
*/
void dss_colored_put(char *out, const int color) {
    init();
    SetConsoleTextAttribute(handle_out, color);
    fputs(out, stdout);
    SetConsoleTextAttribute(handle_out, 0x2E);
}

void dss_colored_putc(char out, const int color) {
    init();
    SetConsoleTextAttribute(handle_out, color);
    putchar(out);
    SetConsoleTextAttribute(handle_out, 0x2E);
}

void dss_set_color(const int color) {
    init();
    SetConsoleTextAttribute(handle_out, color);
}


