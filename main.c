/* 
 * @Author : Lu7fer
 * @Date: 2020-06-21 19:12:37
 * @LastEditTime: 2020-06-23 22:16:40
 * @FilePath: /Data-Storage-System/main.c
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

// #define DSS_DEBUG
//#define DSS_DEBUG

#ifndef _DISPLAY
#ifndef _SHLOBJ_H_

#include <shlobj.h>

#endif

#include <Commdlg.h>

#include "display.h"

#endif
#ifndef _UTILITY

#include "utility.h"

#endif

#define ALERT_TEXT 0x24

#define ADMIN '1'
#define USER '2'
char user;

#define  HAVE_DATA_FILE 1
#define  NO_DATA_FILE 0
char has_data_file;

/*数据*/
varchar_t *data;
char *file_position;
#define FILE_POSITION_SIZE 500

char *dss_select_file(char *, u_int);
#ifndef DSS_DEBUG

int main(int argc, char const *argv[]) {
    dss_set_bgcolor(NORMAL);
    dss_cls();
    dss_set_title("欢迎使用学生管理系统!");
    dss_welcome();
    puts("本系统可以进行学生成绩的管理操作");
    puts("请选择您的身份:\n1.管理员(具有修改和查看权限)\t2.普通用户(具有查看权限)");
    user_select: //user_select

    switch (getch()) {
        case '1': {
            user = ADMIN;
        }
        case '2': {
            user = USER;
            FILE *fp;
            int fsize;
            dss_set_title("正在从默认路径读取数据文件...");
            if (fp = fopen("data.dss", "r")) {
                fsetpos(fp, (fpos_t *) SEEK_END);
                fsize = ftell(fp);
                fsetpos(fp, (fpos_t *) SEEK_SET);
                data = dss_new_string(fsize + 512);
                //32KB缓冲
                char buf[1024 * 32], *index = data->string;
                int rd_size;
                while ((rd_size = fread(buf, 1, 1024 * 32, fp)) != EOF) {
                    memcpy(index, buf, rd_size);
                    index += rd_size;
                }
            } else {
                dss_set_title("文件读取失败");
                file_position=(char *)malloc(FILE_POSITION_SIZE);
                dss_colored_put("\r默认位置没有数据文件,是否手动指定?(y:yes / n:no)\n", ALERT_TEXT);
                file_select:
                switch (getch()) {
                    case 'n': {
                        if (user == USER)
                            dss_colored_put("您是普通用户,请指定数据文件!", ALERT_TEXT);
                        has_data_file = NO_DATA_FILE;
                        break;
                    }
                    case 'y': {
                        has_data_file = HAVE_DATA_FILE;
                        dss_select_file(file_position,FILE_POSITION_SIZE);
                        break;
                    }
                    default: {
                        dss_colored_put("\r请输入y或n", ALERT_TEXT);
                        goto file_select;
                    }
                }
            }
            break;
        }
        default: {
            dss_set_title("输入有误,请重新输入 ");
            dss_colored_put("\r输入有误,请重新输入", ALERT_TEXT);
            // fputs("\r输入有误,请重新输入", stdout);
            goto user_select; //goto
        }
    }

    dss_clean();
    return 0;
}

#endif

//Print_Data()、Count_score()、Input_Data()、Manager()


/**
 *@param path file path
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