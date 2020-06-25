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
#define DSS_DEBUG

#ifndef _DISPLAY


#include "display.h"

#endif
#ifndef _UTILITY

#include "utility.h"

#endif



#define ADMIN '1'
#define USER '2'
char user;

#define  HAVE_DATA_FILE 1
#define  NO_DATA_FILE 0
char has_data_file;

/*数据*/
varchar_t *data;
char *file_path;
#define FILE_POSITION_SIZE 500


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
                file_path = (char *) malloc(FILE_POSITION_SIZE);
                dss_colored_put("\r默认位置没有数据文件,是否手动指定?(y:yes / n:no)\n", ALERT_TEXT);
                file_select:
                switch (getch()) {// TODO ->
                    case 'n': {
                        if (user == USER) {
                            dss_colored_put("您是普通用户,请指定数据文件!", ALERT_TEXT);
                            dss_select_file(file_path, FILE_POSITION_SIZE);
                            //TODO 文件读取
                            printf(file_path);
                            has_data_file = HAVE_DATA_FILE;
                        }
                        has_data_file = NO_DATA_FILE;
                        break;
                    }
                    case 'y': {

                        dss_select_file(file_path, FILE_POSITION_SIZE);
                        //TODO 文件读取
                        printf(file_path);
                        has_data_file = HAVE_DATA_FILE;
                        break;
                    }
                    default: {
                        dss_colored_put("\r请输入y或n", ALERT_TEXT);
                        goto file_select;
                    }//TODO <-
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

#ifdef DSS_DEBUG

int main(int argc, const char **argv) {
    dss_set_bgcolor(NORMAL);
    for (;;) {
        putchar(getch());
    }
}

#endif


