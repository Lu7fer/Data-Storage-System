/* 
 * @Author : Lu7fer
 * @Date: 2020-06-21 19:12:37
 * @LastEditTime: 2020-06-23 22:16:40
 * @FilePath: /Data-Storage-System/main.c
 * @Stu_ID: 2019X....X229_Lu7fer
 * @Github: https://github.com/Lu7fer/Data-Storage-System
-------------------------------------------
实现了主界面
并绘制界面

-------------------------------------------
 */

//#define DSS_DEBUG
//#define DSS_DEBUG


#define ADMIN '1'
#define USER '2'
#define PASSWD_MAX_LENGTH 64
#ifndef _DISPLAY

#include "display.h"

#endif
#ifndef _IO_NO_EXCEPTION

#include "io_no_exception.h"

#endif
#ifndef _UTILITY
#include "utility.h"
#endif

int user;

#define  HAVE_DATA_FILE 1
#define  NO_DATA_FILE 0
char has_data_file;

/*全局 文件路径*/
#define FILE_PATH_SIZE 500
char file_path[FILE_PATH_SIZE];
/*全局 文件路径*/


#define PASSWD_MAX_LENGTH 64
char passwd[PASSWD_MAX_LENGTH];

//
//方法声明
//
void dss_print_panel(info_t *info, char *tips);

void dss_print_space(u_int count);
//
//方法声明结束
//
//#define DSS_DEBUG

#ifndef DSS_DEBUG

int main(int argc, char const *argv[]) {
    //init
    dss_cls();
    dss_reset_window();
    *passwd = 0;
    *file_path = 0;
    info_t info;
    info.header_nums = 0;
    info.item_nums = 0;
    FILE *fp;
    item_t items[0x100];
    char another_save[FILE_PATH_SIZE];
    FILE *another_fp;
    static int lines_of_5 = 0;
    //init
    dss_cls();
    dss_reset_window();
    dss_set_global_color(NORMAL);
    dss_set_title("欢迎使用学生管理系统!");
    dss_welcome();
    puts("本系统可以进行学生成绩的管理操作");
    puts("请选择您的身份:\n1.管理员(具有修改和查看权限)\t2.普通用户(具有查看权限)");
    user = ADMIN;

//    dss_select_file(file_path,FILE_PATH_SIZE,'o');
    menu:
    dss_set_title("请选择需要进行的操作");
    dss_print_panel(&info, NULL);
    switch (getch()) {
        case 3:
            exit(3);
        case '1':
            dss_select_file(file_path, FILE_PATH_SIZE, 'o');
            fp = check_file(file_path);
            info = dss_getinfo(fp, passwd, &user);
            _dss_load_data(fp, &info, items);
            goto menu;

        case '2':
            if (user == ADMIN) {
                dss_set_title("新建");
                info.header_nums = 0;
                info.item_nums = 0;
                dss_create_header(&info);
                dss_show_items(&info, items,user);

            }
            goto menu;


        case '3':
            if ((user == ADMIN) && (*file_path != 0) && (info.header_nums != 0)) {
                freopen(file_path, "w", fp);
                dss_save(fp, &info, items, passwd);
            }
            goto menu;
        case '4':
            if (info.header_nums != 0) {
                dss_select_file(another_save, FILE_PATH_SIZE, 's');
                if (another_fp = fopen(another_save, "w")) {
                    dss_save(another_fp, &info, items, passwd);
                }
            }
            goto menu;
        case '5':
            dss_show_items(&info, items,user);

            goto menu;
        case '6':
            break;
        default:
            goto menu;
    }
    dss_thanks();
    puts("感谢您的使用, 程序将会在3秒后自动退出...");
    dss_set_title("感谢您的使用, 程序将会在3秒后自动退出...");
    Sleep(3000);
    dss_clean();
    return 0;
}

#endif

#ifdef DSS_DEBUG

int main(int argc, const char **argv) {
    char password[0x40];
    dss_get_password(password,0x40);
    puts(password);
    return 0;
}

#endif


void dss_print_panel(info_t *info, char *tips) {
    dss_cls();
    dss_reset_window();
    putchar('\n');
    int title_start = dss_get_window_size().X / 2 - 4;
    dss_print_space(title_start);
    puts("选择菜单");
    if (*file_path != 0) {
        printf("文件位置:");
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
    printf("1.选择  \t\t\t");
    if (user == ADMIN) {
        printf("2.新建  \t\t\t");
        if (info->header_nums != 0) {
            if (*file_path == '\0')
                dss_colored_put("3.保存  \n\n", DISABLE_TEXT);
            else
                printf("3.保存  \n\n");
            dss_print_space(8);
            printf("4.另存为\t\t\t");
        } else {
            dss_set_color(DISABLE_TEXT);
            printf("3.保存  \n\n");
            dss_print_space(8);
            printf("4.另存为\t\t\t");
            dss_set_color(NORMAL_TEXT);
        }
    } else {
        dss_colored_put("2.新建  \t\t\t", DISABLE_TEXT);
        dss_set_color(DISABLE_TEXT);
        printf("3.保存  \n\n");
        dss_print_space(8);
        printf("4.另存为\t\t\t");
        dss_set_color(NORMAL_TEXT);
    }

    if (*file_path == 0) {
        if (info->header_nums == 0)
            dss_colored_put("5.查看&编辑\t\t\t", DISABLE_TEXT);
        if (info->header_nums != 0) {
            printf("5.查看&编辑\t\t\t");
        }
    } else {
        printf("5.查看&编辑  \t\t\t");
    }
    printf("6.退出\n");
    printf("\n  请输入相应数字继续...");
    COORD coord;
    coord.X = 0;
    coord.Y = dss_get_window_size().Y + 1;
    dss_set_cursor_location(coord);
    if (tips != NULL)
        fputs(tips, stdout);
}
