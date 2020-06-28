//
// Created by Dr.Yttrium on 2020/6/24.
//

#ifndef _DATAIO
#define _DATAIO
#endif /* _DATAIO */

#ifndef _UTILITY

#include "utility.h"

#endif/* _UTILITY */

struct map {
    char *key;
    char *value;
};
typedef struct map map_t;
struct item {
    u_int length;
    map_t *maps;
    u_int capacity;
};
typedef struct item item_t;

/**
 * 生成一条map
 **/
map_t *dss_set_map(char *key, char *value) {
    map_t *map;
    map = (map_t *) malloc(sizeof(map_t));
    map->key = strcpy((char *) malloc(strlen(key + 1)), key);
    map->value = strcpy((char *) malloc(strlen(value + 1)), value);
    return map;
}

/**
 * 设置一条信息
 * @param map 存入一条键值对,此map将会被复制
 * @param item 要存入的条目,如果为空,则新建
 */
item_t *dss_add_map(map_t *map, item_t *item) {
    if (item == NULL) {
        item = (item_t *) malloc(sizeof(item_t));
        item->maps = (map_t *) malloc(sizeof(map_t *) * 10);
        item->length = 0;
        item->capacity = 10;
    }
    if (item->capacity == item->length) {
        map_t *temp = (map_t *) malloc(sizeof(map_t *) * item->capacity + 10);
        memcpy(temp, item->maps, item->length * sizeof(map_t *));
        free(item->maps);
        item->maps = temp;
    }
    int i;
    for (i = 0; i < item->length; ++i) {
        if (strcmp(item->maps[i].key, map->key) == 0) {
            if (item->maps[i].value == NULL)
                item->maps[i].value = (char *) malloc(strlen(map->value + 1));
            if (map->value == NULL)
                item->maps[i].value = NULL;
        }
    }
    map_t *tmp = dss_set_map(map->key, map->value);
    (item->maps[i]) = *tmp;
    return item;
}

void dss_free_map(map_t *map) {
    if (map->key != NULL)
        free(map->key);
    if (map->value != NULL)
        free(map->value);
}

item_t *dss_remove_map(char *key, item_t *item) {
    if (key == NULL || item == NULL)
        return item;
    u_int i;
    for (i = 0; i < item->length; ++i) {
        if (strcmp(key, item->maps[i].key) == 0) {
            dss_free_map(&item->maps[i]);
            if (i == item->length - 1)
                break;
            map_t *tmp = (map_t *) malloc(sizeof(map_t) * item->length - 1 - i);
            memcpy(tmp, &item->maps[i + 1], sizeof(map_t) * item->length - 1 - i);
            memcpy(&item->maps[i], tmp, sizeof(map_t) * item->length - 1 - i);
            free(tmp);
            break;
        }
    }
    return item;
}

struct linked_list {
    item_t *node;
    struct linked_list *next;
};
typedef struct linked_list list_t;

list_t *dss_add_item(list_t *list, item_t *item) {
    if (list == NULL) {
        list = (list_t *) malloc(sizeof(list_t));
        list->next = NULL;
        list->node = item;
    } else {
        list_t *tmp = list;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = (list_t *) malloc(sizeof(list_t));
        tmp->next = NULL;
        tmp->node = item;
    }
    return list;
}

list_t *dss_remove_item(list_t *list, item_t *item) {
    if (list == NULL)
        return list;
    list_t *tmp = list;
    do {
        if (tmp->next->node == item) {
            free(tmp->next->node->maps);
            free(tmp->next->node);
            tmp->next = tmp->next->next->node;
            free(item);
            tmp = tmp->next;
        }
    } while (tmp->next->next);
    if (tmp->next == item) {
        free(tmp->node->maps);
        free(tmp->node);
        free(tmp->next);
        tmp->next = NULL;
    }
    return list;
}


varchar_t *dss_save_attr(const char *key, const char *value) {
    u_int len;
    varchar_t *xml;
    len = strlen(value) + strlen(key) * 2;
    xml = dss_new_string(len + 16);
    sprintf(xml->string, "<%s>  %s  </%s>\n", key, value, key);
    return xml;
}

/**
#define INT "%d"
#define LONG_LONG "%ld"
#define FLOAT "%f"
#define DOUBLE "%lf"
#define STRING "%s"
#define CHAR "%c"
#define BOOLEAN "bool"
#define U_INT "%u"
#define U_LONG_LONG "%llu"
*/
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
 * 查找标签 example: to find <target> xxx </target> ,just set *target "target"
 * @param data 被查找的数据
 * @param target 目标标签
 * */
varchar_t *dss_parse_target(char *target, FILE *fp) {
    const int MAX_LENGTH = 0x100000;// 1024*1024=1048576
    u_int capacity = 0x100;
    varchar_t *target_tmpl = dss_new_string(strlen(target) + 5);
    sprintf(target_tmpl->string, "</%s>", target);
    varchar_t *tmp = dss_new_string(capacity);
    while (1) {
        if (fgets(tmp->string, tmp->capacity, fp)) {
            if (capacity >= MAX_LENGTH)
                return NULL;
            dss_free_string(tmp);
            if (strcmp(tmp->string + (strlen(tmp->string) - strlen(target_tmpl)), target_tmpl->string))
                break;
            tmp = dss_new_string(capacity << 1);
        } else {
            return NULL;
        }
    }
    varchar_t *rtn = dss_new_string(strlen(tmp->string) + 2);
    strcpy(rtn->string, tmp->string);
    dss_free_string(target_tmpl);
    dss_free_string(tmp);
    return rtn;
}

list_t *dss_parse(char *root_tag, FILE *fp) {
    varchar_t *to_parse;
    if (to_parse = dss_parse_target(root_tag, fp)) {
        
    }
}