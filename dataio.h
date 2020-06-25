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

list_t *dss_parse(char *data) {

}