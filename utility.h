/*
 * @Author : Lu7fer
 * @Date: 2020-06-19 20:21:15
 * @LastEditTime: 2020-06-23 18:58:20
 * @FilePath: /Data-Storage-System/utility.h
 * @Stu_ID: 2019X....X229_Lu7fer
 * @Github: https://github.com/Lu7fer/C-language-study-homework
 * @Copyright
-------------------------------------------
Copyright (C) 2020 - Lu7fer
C-language-study-homework is free software:
you can redistribute it and/or modify it under the terms of 
the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
 
You should have received a copy of the GNU General Public License 
along with C-language-study-homework. 
If not, see <http: //www.gnu.org/licenses/>.
-------------------------------------------
 */

#ifndef _UTILITY
#define _UTILITY
#endif

typedef unsigned int u_int;

#ifndef _INC_STDLIB
#include <stdlib.h>
#endif /* stdlib.h */

#ifndef _INC_CONIN

#include <conio.h>

#endif /* conio.h */

#ifndef _INC_STDIO
#include <stdio.h>
#endif /* stdio.h */

#ifndef _SHLOBJ_H_

#include <shlobj.h>

#endif

/**
  * @param message 要输出的提示
  * @param str 获取的字符串
  * @param len_of_str 字符串长度
 */
char *dss_gets_invisible(char *massage, char *str, u_int len_of_str) {
    puts(massage);
    u_int position = 0;
    char get;
    while ((get = getch()) != '\n') {
        /*获取到退格*/
        if (get == '\b') {
            if (position > 0) {
                position--;
                continue;
            }
            continue;
        }
        /*超过缓冲区*/
        if (position >= len_of_str - 1)
            continue;
        str[position] = get;
        position++;
    }
    str[position] = '\0';
    return str;
}

#ifndef _VARCHER

#include "varchar.h"

#endif

void dss_clean() {
    CloseHandle(handle_out); //关闭标准输出设备句柄
    handle_out = NULL;
#ifdef DSS_DEBUG
    system("pause");
#endif
}

/**
 *  上 224 72  下  224 80 左  224 75 右 224 77
 *  Ctrl a-z 1-26
 *  F1-F10 0 59 - 0 68 F11-F12  224 133 224 134
*/

#define UP 0xE048
#define DOWN 0xE050
#define RIGHT 0xE04D
#define LEFT 0xE04B

short dss_get_control() {
    short control = 0;
    switch (control = getch()) {
        case 0xE0:
            control <<= 8;
            break;
        default:
            return control;
    }
    return control | getch();
}


/**
 * 采集输入
*/
char *dss_get_input(char *inputs, u_int size) {
    u_int i = 0;
    for (;;) {
        switch (inputs[i] = getche()) {
            case '\b':
                if (i == 0)
                    break;
                --i;
                break;
            case '\n':
                inputs[i] = '\0';
                return inputs;
            default: {
                /**除去ascii不可打印字符*/
                if (inputs[i] > 0x20 && inputs[i] < 0x7F) {
                    if (i < size - 1) {
                        ++i;
                        break;
                    } else {
                        COORD position = dss_get_cursor_location();
                        COORD output = dss_get_window_size();
                        output.X = 0;
                        SetConsoleCursorPosition(handle_out, output);
                        dss_colored_put("你输入太多啦", ALERT_TEXT);
                        SetConsoleCursorPosition(handle_out, position);
                    }
                }
                if (inputs[i] == 0xE0) { getch(); }
            }
        }
    }
}


/**
 * 调出Windows的文件选择窗口
 * @param path : file path
 * @param length >=500
 * @param method : o open file , s save file
 */
char *dss_select_file(char *path, u_int length, char method) {
    OPENFILENAME open;// 公共对话框结构。
//    char path[MAX_PATH];// 用来保存获取文件名称的缓冲区。
    ZeroMemory(&open, sizeof(OPENFILENAME)); // 初始化选择文件对话框
    open.lStructSize = sizeof(OPENFILENAME);//指定这个结构的大小，以字节为单位。
    open.lpstrFile = path;//打开的文件的全路径
    open.lpstrFile[0] = '\0'; //第一个字符串是过滤器描述的显示字符串
    open.nMaxFile = length;  //指定lpstrFile缓冲的大小，以TCHARs为单位
//    if (type == 'd')
    open.lpstrFilter = "数据文件(*.dss)\0*.dss\0所有文件(*.*)\0*.*\0\0";  //打开文件类型
//    else if (type == 'p')
//        open.lpstrFilter = "密码文件(*.pw)\0*.pw\0所有文件(*.*)\0*.*\0\0";
    open.nFilterIndex = 1;  //指定在文件类型控件中当前选择的过滤器的索引
    open.lpstrFileTitle = NULL; // 指向接收选择的文件的文件名和扩展名的缓冲（不带路径信息）。这个成员可以是NULL。
    open.nMaxFileTitle = 0;  //指定lpstrFileTitle缓冲的大小，以TCHARs为单位
    open.lpstrInitialDir = NULL;  //指向以空字符结束的字符串，可以在这个字符串中指定初始目录。
    open.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;//位标记的设置，你可以使用来初始化对话框
    //GetOpenFileName (&open) ;//打开文件对话框
    //GetSaveFileName(&open);//保存文件对话框
    if (method == 'o')
        if (GetOpenFileName(&open))  // 显示打开选择文件对话框。
            return path;

    if (method == 's')
        if (GetSaveFileName(&open))
            return path;

    return NULL;
}


char *dss_password_gets(char *passwd, u_int size) {
    u_int i = 0;
    for (;;) {
        switch (passwd[i] = getch()) {
            case '\b':
                if (i == 0)
                    break;
                --i;
                break;
            case '\n':
                passwd[i] = '\0';
                return passwd;
            default: {
                /**除去ascii不可打印字符*/
                if (passwd[i] > 0x20 && passwd[i] < 0x7F) {
                    if (i < size - 1) {
                        ++i;
                        break;
                    } else {
                        COORD position = dss_get_cursor_location();
                        COORD output = dss_get_window_size();
                        output.X = 0;
                        SetConsoleCursorPosition(handle_out, output);
                        dss_colored_put("你输入太多啦", ALERT_TEXT);
                        SetConsoleCursorPosition(handle_out, position);
                    }
                }
            }
        }
    }
}


