/**
-------------------------------------------
实现了对数据和文件的读取操作
检查文件是否是正确的: FILE *check_file(char *path);
在屏幕上显示数据 同时可以根据提示进行 增加,排序:void dss_show_items(info_t *info, item_t *item);


-------------------------------------------
*/

#ifndef _IO_NO_EXCEPTION
#define _IO_NO_EXCEPTION
#endif //_IO_NO_EXCEPTION
#define NUMBER_T "num"
#define TEXT_T "text"

#ifndef _UTILITY

#include "utility.h"

#endif

#define HEADER_MAX_SIZE 0x10
#define ITEM_MAX_LEN 0x20
#define FILE_MAGIC_NUM "This is a dss file."


//废弃了的方法
#ifdef UNUSAGE
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
 * 全局information结构体
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

//item的数量
u_int items_length;
item_t *record;
//record的内存长度
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
    puts("设置表头");
    dss_println("输入表格索引, 按回车结束. 最大输入长度为%d,设置完后按Ctrl+S保存", (ITEM_MAX_LEN) / 2 - 1);
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
    puts("输入信息");
    dss_draw_line();
    //输出标头
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
        return fp;
    }
    *path = '\0';
    return NULL;
}

struct info {
    u_int header_nums;
    u_int item_nums;
    char header[HEADER_MAX_SIZE][ITEM_MAX_LEN + 6];
    char type[HEADER_MAX_SIZE][6];
};
typedef struct info info_t;

info_t dss_getinfo(FILE *fp, char *global_password, u_int *user) {
//    fseek(fp,0,SEEK_SET);
//    char tmp[50];
//    fgets(tmp,50,fp);
//    printf("%s",tmp);
    info_t info;
    fread(&info, sizeof(info_t), 1, fp);
    char file_passwd[PASSWD_MAX_LENGTH];
    fread(file_passwd, 1, PASSWD_MAX_LENGTH, fp);
    printf("\n请输入密码:");
    char input_passwd[PASSWD_MAX_LENGTH];
    dss_get_password(input_passwd, PASSWD_MAX_LENGTH);
    if (strcmp(input_passwd, file_passwd) == 0) {
        strcpy(global_password, input_passwd);
        *user = ADMIN;
    } else {
        *user = USER;
    }
    return info;
}


int dss_get_header(FILE *fp, info_t *info);

void _dss_create_header_draw(info_t *info) {
    dss_cls();
    int title_start = dss_get_window_size().X / 2 - 4;
    dss_print_space(title_start);
    puts("设置表头");
    dss_draw_line();
    dss_println("输入表格标题和类型, 按回车结束. 最大输入长度为%d,设置完后按Ctrl+S保存", (ITEM_MAX_LEN) / 2 - 1);
    puts("类型分为文本类型和数字类型, 文本类型以text表示,数字类型以num表示");
    dss_draw_line();
    printf("     序号  标题名                  类型名");
    dss_draw_line();
    for (int k = 0; k < info->header_nums && k < HEADER_MAX_SIZE; ++k) {
        printf("     %02d.   %-24s %s\n", k + 1, info->header[k], info->type[k]);
    }
    COORD location;
    location.X = 0;
    location.Y = dss_get_window_size().Y - 2;
    dss_set_cursor_location(location);
    dss_draw_line();
    fputs("请输入:", stdout);
}


/**
 * 创建表头


            if (in == 'S' - 64) {
                return 'S' - 64;
            }

 * */
void dss_create_header(info_t *info) {
    _dss_create_header_draw(info);
    do {
        dss_get_header(stdin, info);
        _dss_create_header_draw(info);
    } while (getch() != 'S' - 64);

    info->item_nums = 0;
}


int dss_get_header(FILE *fp, info_t *info) {
    char buf[0x40];
    if (fp == stdin) {
        char in;
        int i = 0;
        while (1) {
            in = getch();
            if (in == 13) {
                buf[i] = '\0';
                break;
            } else if (in == '\b') {
                if (i > 0) {
                    fputs("\b \b", stdout);
                    --i;
                }
            } else {
                if (i == ITEM_MAX_LEN + 4)
                    continue;
                if (in >= 0x20) {
                    putchar(in);
                    buf[i] = in;
                    ++i;
                }
            }

        }
        if (info->header_nums <= HEADER_MAX_SIZE) {
            sscanf(buf, "%s%4s", info->header[info->header_nums], info->type[info->header_nums]);
            if (*(info->type[info->header_nums]) == 't')
                strcpy(info->type[info->header_nums], TEXT_T);
            else if (*(info->type[info->header_nums]) == 'n')
                strcpy(info->type[info->header_nums], NUMBER_T);
            else
                strcpy(info->type[info->header_nums], NUMBER_T);
            info->header_nums++;
        }
    } else {
        for (int i = 0; i < info->header_nums; ++i) {
            fgets(buf, 0x40, fp);
            sscanf(buf, "%s%s", info->header[info->header_nums], info->type[info->header_nums]);
        }

    }
    return 0;
}

void dss_save(FILE *fp, info_t *info, item_t *items, char *passwd) {

    //写入头信息
    fwrite(info, sizeof(info_t), 1, fp);
    //密码
    if (*passwd == 0) {
        printf("\n请输入密码：");
        dss_get_password(passwd, PASSWD_MAX_LENGTH);
    }
    fwrite(passwd, 1, PASSWD_MAX_LENGTH, fp);
    //信息
    for (int i = 0; i < info->item_nums; ++i) {
        fwrite(items + i, sizeof(item_t), 1, fp);
    }
    fflush(fp);
    fclose(fp);
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
            fread(items, sizeof(item_t), info->item_nums, fp);
        }
    }

}

void _dss_show_items_draw(info_t *info, item_t *items, u_int start_items, int usr) {

    dss_cls();
    dss_set_title("查看&编辑");
    int title_start = dss_get_window_size().X / 2 - 4;
    dss_print_space(title_start);
    puts("查看&编辑");

    if (usr == USER) {
        dss_colored_put("i:插入新记录  d:删除指定记录", DISABLE_TEXT);
    } else {
        printf("i:插入新记录  d:删除指定记录");
    }
    printf("q:退出  e:指令模式  上方向键:向上翻页  下方向键:向下翻页");
    dss_draw_line();
    dss_print_space(5);
    for (int i = 0; i < info->header_nums; ++i) {
        printf("%-15s", info->header[i]);
    }
    dss_draw_line();
    //TODO 显示行数
    for (int j = start_items; j < info->item_nums && j - start_items < 20; ++j) {
        printf("%-3d. ", j + 1);
        for (int i = 0; i < info->header_nums; ++i) {
            printf("%-15s", items[j].item[i]);
        }
        putchar('\n');
    }
    dss_draw_line();

}

int sort_index;
char sort_type;

int _dss_sort_up(const void *s1, const void *s2) {

    if (sort_type == 't') {
        return strcmp(((item_t *) s1)->item[sort_index], ((item_t *) s2)->item[sort_index]);
    }
    if (sort_type == 'n') {
        double d1 = 0, d2 = 0;
        sscanf(((item_t *) s1)->item[sort_index], "%lf", &d1);
        sscanf(((item_t *) s2)->item[sort_index], "%lf", &d2);
        return d1 * 10 - d2 * 10;
    }
}

int _dss_sort_down(const void *s1, const void *s2) {
    return -_dss_sort_up(s1, s2);
}

int lines_of_5 = 0;

void dss_ex_mode(info_t *info, item_t *item) {
    {
        dss_set_title("ex mode");
        COORD c;
        c.X = 0;
        c.Y = dss_get_window_size().Y - 10;
        dss_set_cursor_location(c);
        dss_draw_line();
        dss_print_space(dss_get_window_size().X * 9 - 2);
        c.Y++;
        dss_set_cursor_location(c);

        puts("语法: sort 标题 [<|>] 默认: < ");
        puts("      show 标题 < | > num  :显示某标题大于或小于num的数据");
        puts("      aver n 标题... :求平均 n 为列数");
        puts("      sum n 标题... :计算总和 n为列数");

        dss_draw_line();
    }
    char command[80];
    gets(command);
    char split[HEADER_MAX_SIZE + 3][0x20];
    sscanf(command, "%s%s%s%s%s%s%s%s%s%s%s%s",
           split[0], split[1], split[2],
           split[3], split[4], split[5],
           split[6], split[7], split[8],
           split[9], split[10], split[11]
    );

    //sort start
    if (strcmp(split[0], "sort") == 0) {
        for (int i = 0; i < info->header_nums; ++i) {
            if (strcmp(info->header[i], split[1]) == 0) {
                sort_index = i;
                sort_type = *(info->type[i]);
                switch (*split[2]) {
                    case '>':
                        qsort(item, info->item_nums, sizeof(item_t), _dss_sort_down);
                        break;
                    case '<':
                    default:
                        qsort(item, info->item_nums, sizeof(item_t), _dss_sort_up);
                }
                break;
            }
        }
        //sort end
    }
        //show start
    else if (strcmp(split[0], "show") == 0) {
        for (int i = 0; i < info->header_nums; ++i) {
            if (strcmp(split[1], info->header[i]) == 0);
            {
                sort_index = i;
                sort_type = *(info->type[i]);
            }
        }
        switch (*split[2]) {
            case '>':
                qsort(item, info->item_nums, sizeof(item_t), _dss_sort_up);
                double vertical_l = 0;
                for (int i = 0; i < info->item_nums; ++i) {
                    double tmp;
                    sscanf(item[i].item[sort_index], "%lf", &tmp);
                    if (vertical_l < tmp) {
                        lines_of_5 = tmp;
                        break;
                    }
                }
                break;

            case '<':
                qsort(item, info->item_nums, sizeof(item_t), _dss_sort_down);
                double vertical_s = 0;
                for (int i = 0; i < info->item_nums; ++i) {
                    double tmp;
                    sscanf(item->item[i], "%lf", &tmp);
                    if (vertical_l > tmp) {
                        lines_of_5 = tmp;
                        break;
                    }
                }
                break;
            default:
                printf("\n语法有误");
                lines_of_5 = 0;
                return;
        }


    }
        //aver start
    else if (strcmp(split[0], "aver") == 0) {
        int n = 0;
        sscanf(split[1], "%d", &n);
        int match[HEADER_MAX_SIZE];
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < info->header_nums; ++j) {
                if (strcmp(split[i + 2], info->header[j]) == 0) {
                    match[i] = j;
                    break;
                }
            }
        }
        int aver_index;
        aver_index = info->header_nums;
        strcpy(info->type[info->header_nums], "num");
        info->header_nums++;

        for (int k = 0; k < info->header_nums; ++k) {
            if (strcmp("aver", info->header[k]) == 0) {
                aver_index = k;
                info->header_nums--;
                strcpy(info->type[k], "num");
            }
        }
        strcpy(info->header[aver_index], "aver");
        for (int l = 0; l < info->item_nums; ++l) {
            double sum = 0;
            for (int i = 0; i < n; ++i) {
                double data = 0;
                sscanf(item[l].item[match[i]], "%lf", &data);
                sum += data;
            }
            sprintf(item[l].item[aver_index], "%.1lf", sum / n);
        }

    }
        //aver start
    else if (strcmp(split[0], "sum") == 0) {
        int n = 0;
        sscanf(split[1], "%d", &n);
        int match[HEADER_MAX_SIZE];
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < info->header_nums; ++j) {
                if (strcmp(split[i + 2], info->header[j]) == 0) {
                    match[i] = j;
                    break;
                }
            }
        }
        int aver_index;
        aver_index = info->header_nums;
        strcpy(info->type[info->header_nums], "num");
        info->header_nums++;
        for (int k = 0; k < info->header_nums; ++k) {
            if (strcmp("sum", info->header[k]) == 0) {
                aver_index = k;
                strcpy(info->type[k], "num");
                info->header_nums--;
            }
        }
        strcpy(info->header[aver_index], "sum");
        for (int l = 0; l < info->item_nums; ++l) {
            double sum = 0;
            for (int i = 0; i < n; ++i) {
                double data = 0;
                sscanf(item[l].item[match[i]], "%lf", &data);
                sum += data;
            }
            sprintf(item[l].item[aver_index], "%.1lf", sum);
        }


    }

}

void _dss_insert_item(info_t *info, item_t *item) {
    COORD c;
    c.X = 0;
    c.Y = dss_get_window_size().Y - 3;
    dss_set_cursor_location(c);
    dss_draw_line();
    char first[HEADER_MAX_SIZE * ITEM_MAX_LEN];
    while (gets(first) == NULL);
    char input_get[HEADER_MAX_SIZE][ITEM_MAX_LEN];
    sscanf(first, "%s%s%s%s%s%s%s%s%s%s%s%s",
           input_get[0], input_get[1], input_get[2], input_get[3],
           input_get[4], input_get[5], input_get[6], input_get[7],
           input_get[8], input_get[9], input_get[10], input_get[11]
    );
    for (int i = 0; i < info->header_nums; ++i) {
        strncpy(item[info->item_nums].item[i], input_get[i], ITEM_MAX_LEN);
    }
    info->item_nums++;
}

void dss_delete(info_t *info, item_t *item, u_int index) {
    if (index == info->item_nums) {
        info->item_nums--;
        return;
    }
    item_t *tmp = (item_t *) malloc(sizeof(item_t) * info->item_nums - index);
    memcpy(tmp, &item[index], sizeof(item_t) * info->item_nums - index);
    memcpy(&item[index - 1], tmp, sizeof(item_t) * info->item_nums - index);
    info->item_nums--;
}

void dss_show_items(info_t *info, item_t *item, int user) {
    short ctrl;
    redraw:
    dss_cls();
    _dss_show_items_draw(info, item, lines_of_5, user);
    operate:
    ctrl = dss_get_control();
    if (ctrl == UP) {
        if (lines_of_5 > 0)
            lines_of_5--;
    } else if (ctrl == DOWN) {
        if (lines_of_5 < info->item_nums - 1)
            lines_of_5++;
    } else if (ctrl == 'q') {
        lines_of_5 = 0;
        return;
    } else if (ctrl == 'd') {
        if (user == ADMIN) {
            COORD c;
            c.X = 0;
            c.Y = dss_get_window_size().Y - 3;
            dss_set_cursor_location(c);
            dss_draw_line();
            dss_print_space(dss_get_window_size().X * 2 - 2);
            c.Y++;
            dss_set_cursor_location(c);
            printf("请输入删除条目的序号:");
            int index;
            scanf("%d", &index);
            if (index <= info->item_nums && index > 0) {
                dss_delete(info, item, index);
            }
        }
    } else if (ctrl == 'e') {
        dss_ex_mode(info, item);
    } else if (ctrl == 'i') {
        if (user == ADMIN)
            _dss_insert_item(info, item);
    } else {
        goto operate;
    }
    goto redraw;
}


void _name_sort(info_t *info, item_t *item) {
    for (int i = 0; i < info->header_nums; ++i) {
        if (strcmp("name", info->header[i]) == 0) {
            sort_index = i;
            qsort(item, info->item_nums, sizeof(item_t), _dss_sort_up);
        }
    }
}
