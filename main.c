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

/*����*/
varchar_t *data;
char *file_path;
#define FILE_POSITION_SIZE 500


#ifndef DSS_DEBUG

int main(int argc, char const *argv[]) {
    dss_set_bgcolor(NORMAL);
    dss_cls();
    dss_set_title("��ӭʹ��ѧ������ϵͳ!");
    dss_welcome();
    puts("��ϵͳ���Խ���ѧ���ɼ��Ĺ������");
    puts("��ѡ���������:\n1.����Ա(�����޸ĺͲ鿴Ȩ��)\t2.��ͨ�û�(���в鿴Ȩ��)");
    user_select: //user_select

    switch (getch()) {
        case '1': {
            user = ADMIN;
        }
        case '2': {
            user = USER;
            FILE *fp;
            int fsize;
            dss_set_title("���ڴ�Ĭ��·����ȡ�����ļ�...");
            if (fp = fopen("data.dss", "r")) {
                fsetpos(fp, (fpos_t *) SEEK_END);
                fsize = ftell(fp);
                fsetpos(fp, (fpos_t *) SEEK_SET);
                data = dss_new_string(fsize + 512);
                //32KB����
                char buf[1024 * 32], *index = data->string;
                int rd_size;
                while ((rd_size = fread(buf, 1, 1024 * 32, fp)) != EOF) {
                    memcpy(index, buf, rd_size);
                    index += rd_size;
                }
            } else {
                dss_set_title("�ļ���ȡʧ��");
                file_path = (char *) malloc(FILE_POSITION_SIZE);
                dss_colored_put("\rĬ��λ��û�������ļ�,�Ƿ��ֶ�ָ��?(y:yes / n:no)\n", ALERT_TEXT);
                file_select:
                switch (getch()) {// TODO ->
                    case 'n': {
                        if (user == USER) {
                            dss_colored_put("������ͨ�û�,��ָ�������ļ�!", ALERT_TEXT);
                            dss_select_file(file_path, FILE_POSITION_SIZE);
                            //TODO �ļ���ȡ
                            printf(file_path);
                            has_data_file = HAVE_DATA_FILE;
                        }
                        has_data_file = NO_DATA_FILE;
                        break;
                    }
                    case 'y': {

                        dss_select_file(file_path, FILE_POSITION_SIZE);
                        //TODO �ļ���ȡ
                        printf(file_path);
                        has_data_file = HAVE_DATA_FILE;
                        break;
                    }
                    default: {
                        dss_colored_put("\r������y��n", ALERT_TEXT);
                        goto file_select;
                    }//TODO <-
                }
            }
            break;
        }
        default: {
            dss_set_title("��������,���������� ");
            dss_colored_put("\r��������,����������", ALERT_TEXT);
            // fputs("\r��������,����������", stdout);
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


