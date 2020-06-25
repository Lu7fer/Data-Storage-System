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

varchar_t *dss_set_attr(const char *key, const char *value) {
    u_int len;
    varchar_t *xml;
    len = strlen(value) + strlen(key) * 2;
    xml = dss_new_string(len + 16);
    sprintf(xml->string, "<%s>  %s  </%s>\n", key, value, key);
    return xml;
}

#define INT "%d"
#define LONG_LONG "%ld"
#define FLOAT "%f"
#define DOUBLE "%lf"
#define STRING "%s"
#define CHAR "%c"
#define BOOLEAN "bool"
#define U_INT "%u"
#define U_LONG_LONG "%llu"

/**
 * @param str 需要读取的字符串
 * @param key 属性key
 * @param value 读到的值
*/
int dss_get_attr(const char *str, const char *key, varchar_t *value) {
    //<key>%s</key>
    varchar_t *format = dss_new_string(strlen(key) * 2 + 10);
    sprintf(format->string, "<%s>%%s</%s>", key, key);
    if (value->capacity < (strlen(str) - 7)) {
        dss_free_string(value);
        value = dss_new_string(strlen(str));
    }
    int rtn = sscanf(str, format->string, value);
    dss_free_string(format);
    return rtn;
}

/**
 *  上 224 72  下  224 80 左  224 75 右 224 77
 *  Ctrla-z 1-26
 *  F1-F10 0 59 - 0 68 F11-F12  224 133 224 134
*/

#define UP 0xE048
#define DOWN 0xE050
#define RIGHT 0xE04D
#define LEFT 0xE04B

/**
 * 采集输入
*/
short dss_get_input() {
    short in = getch();
    char next;
    switch (in) {
        case 0:
            in <<= 8;
            break;
        case 0xE0:
            in <<= 8;
            switch (next = getch()) {
                case 0x48:
                case 0x4B:
                case 0x4D:
                case 0x50:
                    in += next;
                    break;
                default:
                    break;
            }
    }
    return in;
}


/**
 * 调出普通的文件选择窗口
 *@param path : file path
 *@param length >=500
 */
char *dss_select_file(char *path, u_int length) {
    OPENFILENAME open;// 公共对话框结构。
//    char path[MAX_PATH];// 用来保存获取文件名称的缓冲区。
    ZeroMemory(&open, sizeof(OPENFILENAME)); // 初始化选择文件对话框
    open.lStructSize = sizeof(OPENFILENAME);//指定这个结构的大小，以字节为单位。
    open.lpstrFile = path;//打开的文件的全路径
    open.lpstrFile[0] = '\0'; //第一个字符串是过滤器描述的显示字符串
    open.nMaxFile = length;  //指定lpstrFile缓冲的大小，以TCHARs为单位
    open.lpstrFilter = "数据文件(*.dss)\0*.dss\0所有文件(*.*)\0*.*\0\0";  //打开文件类型
    open.nFilterIndex = 1;  //指定在文件类型控件中当前选择的过滤器的索引
    open.lpstrFileTitle = NULL; // 指向接收选择的文件的文件名和扩展名的缓冲（不带路径信息）。这个成员可以是NULL。
    open.nMaxFileTitle = 0;  //指定lpstrFileTitle缓冲的大小，以TCHARs为单位
    open.lpstrInitialDir = NULL;  //指向以空字符结束的字符串，可以在这个字符串中指定初始目录。
    open.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;//位标记的设置，你可以使用来初始化对话框
    //GetOpenFileName (&open) ;//打开文件对话框
    //GetSaveFileName(&open);//保存文件对话框
    if (GetOpenFileName(&open))  // 显示打开选择文件对话框。
    {
        return path;
    }
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
                        dss_colored_put("你输入太多啦", ALERT_TEXT);
                    }
                }
            }
        }
    }
}
