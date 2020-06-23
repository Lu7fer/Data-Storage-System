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

/*����*/
varchar_t *data;
char *file_position;
#define FILE_POSITION_SIZE 500

char *dss_select_file(char *, u_int);
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
                file_position=(char *)malloc(FILE_POSITION_SIZE);
                dss_colored_put("\rĬ��λ��û�������ļ�,�Ƿ��ֶ�ָ��?(y:yes / n:no)\n", ALERT_TEXT);
                file_select:
                switch (getch()) {
                    case 'n': {
                        if (user == USER)
                            dss_colored_put("������ͨ�û�,��ָ�������ļ�!", ALERT_TEXT);
                        has_data_file = NO_DATA_FILE;
                        break;
                    }
                    case 'y': {
                        has_data_file = HAVE_DATA_FILE;
                        dss_select_file(file_position,FILE_POSITION_SIZE);
                        break;
                    }
                    default: {
                        dss_colored_put("\r������y��n", ALERT_TEXT);
                        goto file_select;
                    }
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

//Print_Data()��Count_score()��Input_Data()��Manager()


/**
 *@param path file path
 *@param length >=500
 */
char *dss_select_file(char *path, u_int length) {
    OPENFILENAME open;// �����Ի���ṹ��
//    char path[MAX_PATH];// ���������ȡ�ļ����ƵĻ�������
    ZeroMemory(&open, sizeof(OPENFILENAME)); // ��ʼ��ѡ���ļ��Ի���
    open.lStructSize = sizeof(OPENFILENAME);//ָ������ṹ�Ĵ�С�����ֽ�Ϊ��λ��
    open.lpstrFile = path;//�򿪵��ļ���ȫ·��
    open.lpstrFile[0] = '\0'; //��һ���ַ����ǹ�������������ʾ�ַ���
    open.nMaxFile = length;  //ָ��lpstrFile����Ĵ�С����TCHARsΪ��λ
    open.lpstrFilter = "�����ļ�(*.dss)\0*.dss\0�����ļ�(*.*)\0*.*\0\0";  //���ļ�����
    open.nFilterIndex = 1;  //ָ�����ļ����Ϳؼ��е�ǰѡ��Ĺ�����������
    open.lpstrFileTitle = NULL; // ָ�����ѡ����ļ����ļ�������չ���Ļ��壨����·����Ϣ���������Ա������NULL��
    open.nMaxFileTitle = 0;  //ָ��lpstrFileTitle����Ĵ�С����TCHARsΪ��λ
    open.lpstrInitialDir = NULL;  //ָ���Կ��ַ��������ַ���������������ַ�����ָ����ʼĿ¼��
    open.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;//λ��ǵ����ã������ʹ������ʼ���Ի���
    //GetOpenFileName (&open) ;//���ļ��Ի���
    //GetSaveFileName(&open);//�����ļ��Ի���
    if (GetOpenFileName(&open))  // ��ʾ��ѡ���ļ��Ի���
    {
        return path;
    }
    return NULL;
}