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

//#define DSS_DEBUG
//#define DSS_DEBUG

#ifndef _DISPLAY


#include "display.h"

#endif
#ifndef _IO_NO_EXCEPTION

#include "io_no_exception.h"

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

/*ȫ�� �ļ�·��*/
varchar_t *data;
char *file_path;
#define FILE_PATH_SIZE 500

/*ȫ�� �ļ�·��*/
#define PASSWD_MAX_LENGTH 64
char passwd[PASSWD_MAX_LENGTH];


//
//��������
//
void dss_print_panel();

void dss_print_space(u_int count);
//
//������������
//
//#define DSS_DEBUG

#ifndef DSS_DEBUG

int main(int argc, char const *argv[]) {
    //init

    dss_cls();
    dss_reset_window();
    *passwd = 0;
    file_path = (char *) calloc(FILE_PATH_SIZE, 1);
    info_t info;
    info.header_nums = 0;
    info.item_nums = 0;
    FILE *fp;
    item_t *items;
    char another_save[FILE_PATH_SIZE];
    FILE *another_fp;
    static int lines_of_5=0;
    //init
    dss_cls();
    dss_reset_window();
    dss_set_global_color(NORMAL);
    dss_set_title("��ӭʹ��ѧ������ϵͳ!");
    dss_welcome();
    puts("��ϵͳ���Խ���ѧ���ɼ��Ĺ������");
    puts("��ѡ���������:\n1.����Ա(�����޸ĺͲ鿴Ȩ��)\t2.��ͨ�û�(���в鿴Ȩ��)");
    user_select: //user_select
    switch (getch()) {
        case '1': {
            user = ADMIN;
            break;
        }
        case '2': {
            user = USER;
            break;
        }
        default: {
            dss_set_title("��������,���������� ");
            dss_colored_put("\r��������,����������", ALERT_TEXT);
            goto user_select; //goto
        }
    }

//    dss_select_file(file_path,FILE_PATH_SIZE,'o');
    menu:
    dss_set_title("��ѡ����Ҫ���еĲ���");
    dss_print_panel();
    switch (getch()) {
        case '1':
            dss_select_file(file_path, FILE_PATH_SIZE, 'o');
            fp = check_file(file_path);
            info = dss_getinfo(fp);

            goto menu;
        case '2':
            if (user == ADMIN) {
                dss_set_title("�½�");
                info.header_nums = 0;
                info.item_nums = 0;
                dss_create_header(&info);
            }
            goto menu;
        case '3':
            if ((user == ADMIN) && (*file_path != 0) && (info.header_nums != 0)) {
                dss_save(fp, &info, items, passwd);
            }
            goto menu;
        case '4':
            dss_select_file(another_save, FILE_PATH_SIZE, 's');
            if (another_fp = fopen(another_save, "w")) {
                dss_save(another_fp, &info, items, passwd);
            }
            goto menu;
        case '5':
            goto menu;
        case '6':
            break;
        default:
            goto menu;
    }
    dss_thanks();
    puts("��л����ʹ��, ���򽫻���5����Զ��˳�...");
    Sleep(5000);
    dss_clean();
    return 0;
}

#endif

#ifdef DSS_DEBUG

int main(int argc, const char **argv) {
    printf("%d",getch());
    return 0;
}

#endif


void dss_print_panel(info_t *info) {
    dss_cls();
    dss_reset_window();
    putchar('\n');
    int title_start = dss_get_window_size().X / 2 - 4;
    dss_print_space(title_start);
    puts("ѡ��˵�");
    if (*file_path != 0) {
        printf("�ļ�λ��:");
        if (strlen(file_path) > dss_get_window_size().X - 11) {
            fwrite(file_path, 1, dss_get_window_size().X - 15, stdout);
            puts("...");
        } else
            puts(file_path);
    } else
        putchar('\n');

    dss_draw_line();
    putchar('\n');
    dss_print_space(8);
    printf("1.ѡ��  \t\t\t");
    if (user == ADMIN) {
        printf("2.�½�  \t\t\t");
        if(info->header_nums!=0){
            printf("3.����  \n\n");
            dss_print_space(8);
            printf("4.���Ϊ\t\t\t");
        }
    } else {
        dss_colored_put("2.�½�  \t\t\t", DISABLE_TEXT);
    }

    if (*file_path == 0) {
        if (info->header_nums == 0)
            printf("5.�鿴&�༭\t\t\t");
        dss_set_color(NORMAL_TEXT);
        if (info->header_nums != 0) {
            printf("5.�鿴&�༭\t\t\t");
        }
        dss_set_color(NORMAL_TEXT);
        printf("6.�˳�\n");
    } else {
        printf("3.����  \n\n");
        dss_print_space(8);
        printf("4.���Ϊ\t\t\t");
        printf("5.�鿴&�༭  \t\t\t");
        printf("6.�˳�\n");
    }
    printf("\n  ��������Ӧ���ּ���...");

}
