//
// Created by Dr.Yttrium on 2020/6/26.
//

#ifndef _IO_NO_EXCEPTION
#define _IO_NO_EXCEPTION

#endif //_NO_EXCEPTION

#ifndef _UTILITY

#include "utility.h"

#endif

#define HEADER_MAX_SIZE 0x10
#define ITEM_MAX_LEN 0x20
#define FILE_MAGIC_NUM "This is a dss file."

#ifdef XXX
u_int dss_passwd_check(FILE *fp, char *i_pw) {
    char f_pw[64];
    fgets(f_pw, 64, fp);
    return strcmp(i_pw, f_pw);
}

char *_dss_gen(u_int num, char *str) {
    char base[] = "%s ";
    strcpy(str, base);
    for (int i = 1; i < num && i < HEADER_MAX_SIZE; ++i) {
        sprintf(str, "%s %%s", base);
    }
    return str;
}

struct file_information {
    u_int header_nums;
    u_int record_items;
};
typedef struct file_information info_t;

/**
 * ȫ��information�ṹ��
 * */
info_t info;

struct headers {
    char header[HEADER_MAX_SIZE][ITEM_MAX_LEN];
};
typedef struct headers header_t;
header_t header;

int dss_parse_header(FILE *fp, header_t *headers, info_t *info) {

    for (int i = 0; i < info->header_nums; ++i) {
        if (EOF == fscanf(fp, "%s", headers->header[i]))
            return 0;
    }
    return info->header_nums;
}


struct details {

    char *infos[HEADER_MAX_SIZE];
};
typedef struct details item_t;

//item������
u_int items_length;
item_t *record;
//record���ڴ泤��
u_int record_size;

int dss_parse_record(FILE *fp, info_t *info) {
    u_int count = 0;
    record = (item_t *) malloc(sizeof(item_t) * info->record_items * 2);
    items_length = info->record_items * 2;
    record_size = info->record_items;
    char matcher[0x30];
    _dss_gen(info->header_nums, matcher);
    char tmp[HEADER_MAX_SIZE][ITEM_MAX_LEN];
    for (int i = 0; i < info->record_items; ++i, ++count) {
        fscanf(fp, matcher,
               tmp[0], tmp[1], tmp[2], tmp[3],
               tmp[4], tmp[5], tmp[6], tmp[7],
               tmp[8], tmp[9], tmp[10], tmp[11],
               tmp[12], tmp[13], tmp[14], tmp[15]
        );
        for (int j = 0; j < info->header_nums; ++j) {
            record[i].infos[j] = (char *) malloc(strlen(tmp[j]) + 2);
            strcpy(record[i].infos[j], tmp[j]);
        }
    }
    return count;
}


void dss_save(FILE *fp, char *passwd, header_t *header, info_t *info, item_t *records) {
    fputs(FILE_MAGIC_NUM, fp);
    fputc('\n', fp);
    fputs(passwd, fp);
    fputc('\n', fp);
    fwrite(info, sizeof(info_t), 1, fp);
    fputc('\n', fp);
    for (int i = 0; i < info->header_nums; ++i) {
        fprintf(fp, "%s ", header->header[i]);
    }
    fputc('\n', fp);
    for (int j = 0; j < info->record_items; ++j) {
        for (int i = 0; i < info->header_nums; ++i) {
            fprintf(fp, "%s ", records[j].infos[i]);
        }
        putchar('\n');
    }
}

FILE *check_file(char *path) {
    FILE *fp;
    if (fp = fopen(path, "r")) {
        char buf[64];
        fgets(buf, 64, fp);
        if (strcmp(buf, FILE_MAGIC_NUM) == 0)
            return fp;
    }
    return NULL;
}

void dss_edit_items() {
    char inputs[0x30];
    dss_get_input(inputs, 0x30);
    //TODO
}

void dss_create_items() {
    info.record_items = 0;
    record = (item_t *) malloc(sizeof(item_t) * 0x100);
    record_size = 0x100;
    info.header_nums = 0;
    int title_start = dss_get_window_size().X / 2 - 4;
    dss_print_space(title_start);
    puts("���ñ�ͷ");
    dss_println("����������, ���س�����. ������볤��Ϊ%d,�������Ctrl+S����", (ITEM_MAX_LEN) / 2 - 1);
    dss_draw_line();
    int i = 1;
    while (i) {
        printf("     %-2d. ", i);
        dss_get_input(header.header[i], ITEM_MAX_LEN);
        short judge;
        next:
        judge = dss_get_control();
        if (judge == DOWN) {
            continue;
        } else if (judge == ('S' - 64))
            break;
        else
            goto next;
    }
    info.header_nums = i;
    dss_cls();
    title_start = dss_get_window_size().X / 2 - 4;
    dss_print_space(title_start);
    puts("������Ϣ");
    dss_draw_line();
    //�����ͷ
    for (int j = 0; j < info.header_nums; ++j) {
        printf(" %s ", header.header[j]);
    }
    dss_draw_line();

}

#endif

struct table {
    char item[HEADER_MAX_SIZE][ITEM_MAX_LEN];
};
typedef struct table item_t;

FILE *check_file(char *path) {
    FILE *fp;
    if (fp = fopen(path, "r")) {
        char buf[64];
        fgets(buf, 64, fp);
        if (strcmp(buf, FILE_MAGIC_NUM) == 0)
            return fp;
        fclose(fp);
    }
    return NULL;
}

struct info {
    u_int header_nums;
    u_int passwd_length;
    u_int item_nums;
    char header[HEADER_MAX_SIZE][ITEM_MAX_LEN + 6];
    char type[HEADER_MAX_SIZE][5];
};
typedef struct info info_t;

info_t dss_getinfo(FILE *fp) {
    info_t info;
    fread(&info, sizeof(info_t), 1, fp);
    fgetchar();
    return info;
}


#define NUMBER_T "num"
#define TEXT_T "text"

int dss_get_header(FILE *fp, info_t *info);

void _dss_create_header_draw(info_t *info) {
    dss_cls();
    int title_start = dss_get_window_size().X / 2 - 4;
    dss_print_space(title_start);
    puts("���ñ�ͷ");
    dss_draw_line();
    dss_println("��������������, ���س�����. ������볤��Ϊ%d,�������Ctrl+S����", (ITEM_MAX_LEN) / 2 - 1);
    puts("���ͷ�Ϊ�ı����ͺ���������, �ı�������text��ʾ,����������num��ʾ");
    dss_draw_line();
    printf("     ���  ������                  ������");
    dss_draw_line();
    for (int k = 0; k < info->header_nums && k < HEADER_MAX_SIZE; ++k) {
        printf("     %02d.   %-24s %s\n", k + 1, info->header[k], info->type[k]);
    }
    COORD location;
    location.X = 0;
    location.Y = dss_get_window_size().Y - 2;
    dss_set_cursor_location(location);
    dss_draw_line();
    fputs("������:", stdout);
}

/**
 * ������ͷ
 * */
void dss_create_header(info_t *info) {
    _dss_create_header_draw(info);
    while (dss_get_header(stdin, info) != ('S' - 64))
        _dss_create_header_draw(info);
    info->item_nums = 0;
}


int dss_get_header(FILE *fp, info_t *info) {
    char buf[0x40];
    if (fp == stdin) {
        char in;
        int i = 0;
        while (1) {
            in = getch();
            if (in == 'S' - 64) {
                return 'S' - 64;
            }
            if (in == 13) {
                buf[i] = '\0';
                break;
            } else if (in == '\b') {
                fputs("\b \b", stdout);
                --i;
            } else {
                if (i == ITEM_MAX_LEN + 4)
                    continue;
                if (in >= 0x20 && in <= 0x79) {
                    putchar(in);
                    buf[i] = in;
                    ++i;
                }
            }

        }
        if (info->header_nums <= HEADER_MAX_SIZE) {
            sscanf(buf, "%s%s", info->header[info->header_nums], info->type[info->header_nums]);
            info->header_nums++;
        }
    } else {
        fgets(buf, 0x40, fp);
        sscanf(buf, "%s%s", info->header[info->header_nums], info->type[info->header_nums]);
        info->header_nums++;
    }
    return 0;
}

void dss_save(FILE *fp, info_t *info, item_t *items, char *passwd) {
    //ħ��
    fputs(FILE_MAGIC_NUM, fp);
    //�س�
    fputc('\n', fp);
    info->passwd_length = strlen(passwd);
    //д��ͷ��Ϣ
    fwrite(info, sizeof(info_t), 1, fp);
    //����
    fprintf(fp, "%s\n", passwd);
    //��Ϣ
    for (int i = 0; i < info->item_nums; ++i) {
        for (int j = 0; j < info->header_nums; ++j) {
            if (strlen(items[i].item[j]) == 0)
                fprintf(fp, "NULL");
            else
                fprintf(fp, "%s ", items[i].item[j]);
        }
    }
}

char *_dss_gen(u_int num, char *str) {
    char base[] = "%s ";
    strcpy(str, base);
    for (int i = 1; i < num && i < HEADER_MAX_SIZE; ++i) {
        sprintf(str, "%s %%s", base);
    }
    return str;
}


int _dss_load_data(FILE *fp, info_t *info, item_t *items) {
    char buf[ITEM_MAX_LEN * HEADER_MAX_SIZE];
    if (fp == stdin) {
        char in;
        int i = 0;
        while (1) {
            in = getch();
            if (in == 'S' - 64) {
                return 'S' - 64;
            }
            if (in == 13) {
                buf[i] = '\0';
                break;
            } else if (in == '\b') {
                fputs("\b \b", stdout);
                --i;
            } else {
                if (i == (ITEM_MAX_LEN - 1 * info->header_nums))
                    continue;
                if (in >= 0x20 && in <= 0x79) {
                    putchar(in);
                    buf[i] = in;
                    ++i;
                }
            }
        }
        char matcher[ITEM_MAX_LEN];
        _dss_gen(info->header_nums, matcher);
        sscanf(buf, "%s%s", info->header[info->header_nums], info->type[info->header_nums]);
        info->item_nums++;

    } else {
        for (int i = 0; i < info->item_nums; ++i) {
            for (int j = 0; j < info->header_nums; ++j) {
                fscanf(fp, "%s ", items[i].item[j]);
            }
        }
    }

}

void _dss_show_items_draw(info_t *info, item_t *items, u_int lines) {

    dss_cls();
    int title_start = dss_get_window_size().X / 2 - 4;
    dss_print_space(title_start);
    puts("�鿴&�༭");
    printf("i:�����¼�¼   q:�˳�  e:ָ��ģʽ  �Ϸ����:���Ϸ�ҳ  �·����:���·�ҳ");
    dss_draw_line();
    dss_print_space(4);
    for (int i = 0; i < info->header_nums; ++i) {
        printf("%-15s", info->header[i]);
    }
    dss_draw_line();
    for (int j = lines; j < info->item_nums && j < lines + 20; ++j) {
        for (int i = 0; i < info->header_nums; ++i) {
            printf("%-15s", items[j].item[i]);
        }
        putchar('\n');
    }
    dss_draw_line();

}


void dss_show_items(info_t *info, item_t *item) {
    int lines_of_5 = 0;
    _dss_show_items_draw(info, item, lines_of_5);
    short ctrl;
    ctrl = dss_get_control();
    if (ctrl == UP) {
        if (lines_of_5 > 0)
            lines_of_5--;
    } else if (ctrl == DOWN) {
        if (lines_of_5 < info->item_nums - 20)
            lines_of_5++;
    } else if(ctrl == 'q'){

    } else if(ctrl=='e'){
        
    }
}