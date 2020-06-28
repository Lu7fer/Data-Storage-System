/**
 * @Author : Lu7fer
 * @Date: 2020-06-19 20:21:15
 * @LastEditTime: 2020-06-23 18:58:20
 * @FilePath: /Data-Storage-System/utility.h
 * @Stu_ID: 2019X....X229_Lu7fer
 * @Github: https://github.com/Lu7fer/C-language-study-homework

-------------------------------------------
ʵ���˶Կ���̨�Ĳ�����һЩ����

�ļ�ѡ����Ե���ϵͳ���ļ�ѡ���: char *dss_select_file(char *path, u_int length, char method) ;
�����������ʱ����Բ���ʾ���ն���:char *dss_password_gets(char *passwd, u_int size);
��ȡ���̵ļ�ͷ���� :short dss_get_control();
����ʵ������,�ڻس���ʱ�򲻻���: char *dss_get_input(char *inputs, u_int size);
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
  * @param message Ҫ�������ʾ
  * @param str ��ȡ���ַ���
  * @param len_of_str �ַ�������
 */
char *dss_gets_invisible(char *massage, char *str, u_int len_of_str) {
    puts(massage);
    u_int position = 0;
    char get;
    while ((get = getch()) != '\n') {
        /*��ȡ���˸�*/
        if (get == '\b') {
            if (position > 0) {
                position--;
                continue;
            }
            continue;
        }
        /*����������*/
        if (position >= len_of_str - 1)
            continue;
        str[position] = get;
        position++;
    }
    str[position] = '\0';
    return str;
}


void dss_clean() {
    CloseHandle(handle_out); //�رձ�׼����豸���
    handle_out = NULL;
#ifdef DSS_DEBUG
    system("pause");
#endif
}

/**
 *  �� 224 72  ��  224 80 ��  224 75 �� 224 77
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
 * �ɼ�����
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
                /**��ȥascii���ɴ�ӡ�ַ�*/
                if (inputs[i] > 0x20 && inputs[i] < 0x7F) {
                    if (i < size - 1) {
                        ++i;
                        break;
                    } else {
                        COORD position = dss_get_cursor_location();
                        COORD output = dss_get_window_size();
                        output.X = 0;
                        SetConsoleCursorPosition(handle_out, output);
                        dss_colored_put("������̫����", ALERT_TEXT);
                        SetConsoleCursorPosition(handle_out, position);
                    }
                }
                if (inputs[i] == 0xE0) { getch(); }
            }
        }
    }
}


/**
 * ����Windows���ļ�ѡ�񴰿�
 * @param path : file path
 * @param length >=500
 * @param method : o open file , s save file
 */
char *dss_select_file(char *path, u_int length, char method) {
    OPENFILENAME open;// �����Ի���ṹ��
//    char path[MAX_PATH];// ���������ȡ�ļ����ƵĻ�������
    ZeroMemory(&open, sizeof(OPENFILENAME)); // ��ʼ��ѡ���ļ��Ի���
    open.lStructSize = sizeof(OPENFILENAME);//ָ������ṹ�Ĵ�С�����ֽ�Ϊ��λ��
    open.lpstrFile = path;//�򿪵��ļ���ȫ·��
    open.lpstrFile[0] = '\0'; //��һ���ַ����ǹ�������������ʾ�ַ���
    open.nMaxFile = length;  //ָ��lpstrFile����Ĵ�С����TCHARsΪ��λ
//    if (type == 'd')
    open.lpstrFilter = "�����ļ�(*.dss)\0*.dss\0�����ļ�(*.*)\0*.*\0\0";  //���ļ�����
//    else if (type == 'p')
//        open.lpstrFilter = "�����ļ�(*.pw)\0*.pw\0�����ļ�(*.*)\0*.*\0\0";
    open.nFilterIndex = 1;  //ָ�����ļ����Ϳؼ��е�ǰѡ��Ĺ�����������
    open.lpstrFileTitle = NULL; // ָ�����ѡ����ļ����ļ�������չ���Ļ��壨����·����Ϣ���������Ա������NULL��
    open.nMaxFileTitle = 0;  //ָ��lpstrFileTitle����Ĵ�С����TCHARsΪ��λ
    open.lpstrInitialDir = NULL;  //ָ���Կ��ַ��������ַ���������������ַ�����ָ����ʼĿ¼��
    open.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;//λ��ǵ����ã������ʹ������ʼ���Ի���
    //GetOpenFileName (&open) ;//���ļ��Ի���
    //GetSaveFileName(&open);//�����ļ��Ի���
    if (method == 'o')
        if (GetOpenFileName(&open))  // ��ʾ��ѡ���ļ��Ի���
            return path;

    if (method == 's')
        if (GetSaveFileName(&open))
            return path;

    return NULL;
}


char *dss_get_password(char *passwd, u_int size) {
    char in;
    int i = 0;
    while (1) {
        in = getch();
        if (in == 13) {
            passwd[i] = '\0';
            fputc('+', stdin);
            break;
        } else if (in == '\b') {
            if (i > 0) {
                fputs("\b \b", stdout);
                --i;
            }
        } else {
            if (i == size - 2)
                continue;
            if (in >= 0x20) {
                putchar('*');
                passwd[i] = in;
                ++i;
            }
        }

    }
}

