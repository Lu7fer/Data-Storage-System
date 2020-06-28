//
// Created by Dr.Yttrium on 2020/6/26.
//

#ifndef _IO_STRUCTURE
#define _IO_STRUCTURE
#endif /* _IO_STRUCTURE */

#ifndef _UTILITY

#include "utility.h"

#endif

#define HEADER_START "<header>"
#define HEADER_INLINE_SEPARATOR ' '
#define HEADER_END "</header>"
#define BODY_START "<body>"
#define BODY_END "</body>"
#define BODY_INLINE_SEPARATOR ' '
#define BODY_SEPARATOR '\n'
#define HEADER_MAX_SIZE 0x10

struct header {
    u_int _size;
//    char *unparsed_header;
    char *headers[HEADER_MAX_SIZE];
};
typedef struct header header_t;

struct item {
    char *data[HEADER_MAX_SIZE];
};
typedef struct item item_t;
struct body {
    u_int _size;
    //¹ÒÔØitem_tÊý×é
    item_t *items;
    u_int item_size;
};
typedef struct body body_t;

header_t *dss_parse_header(FILE *fp) {
    header_t *rtn = (header_t *) malloc(sizeof(header_t));
    rtn->_size = 0;
    char passwd[0x1000];
    rewind(fp);
    fgets(passwd, 0x1000, fp);
    char data[0x100];
    for (int i = 0; i < 5; ++i) {
        if (i > 3)
            return NULL;
        fscanf(fp, "%s", data);
        if (strcmp(data, HEADER_START) == 0) {
            break;
        }
    }
    for (int j = 0; j < HEADER_MAX_SIZE; ++j) {
        fscanf(fp, "%s", data);
        if (strcmp(HEADER_END, data) == 0) {
            break;
        }
        rtn->headers[rtn->_size] = (char *) malloc(sizeof(strlen(data) + 2));
        strcpy(rtn->headers[rtn->_size], data);
        rtn->_size++;
    }
    return rtn;
}

char *_dss_gen(u_int num, char *str) {
    char base = "%s";
    strcpy(str, base);
    for (int i = 1; i < num && i < HEADER_MAX_SIZE; ++i) {
        sprintf(str, "%s%%s", base);
    }
    return str;
}

body_t *dss_parse_body(FILE *fp, header_t *header) {
    if (header == NULL) {
        return NULL;
    }
    char *data = (char *) malloc(header->_size * 0x100);
    for (int i = 0; i < 5; ++i) {
        if (i > 3)
            return NULL;
        fscanf(fp, "%s", data);
        if (strcmp(BODY_START, data) == 0)
            break;
    }
    body_t *rtn = (body_t *) malloc(sizeof(body_t));
    rtn->_size = 0;
    char matcher[80];
    _dss_gen(header->_size, matcher);
    rtn->items = (item_t *) malloc(sizeof(item_t) * 0x80);
    rtn->item_size = 0x80;
    char tmp[HEADER_MAX_SIZE][0x100];
    for (int i = 0; fgets(data, header->_size * 0x100, fp) != EOF; ++i) {
        if (rtn->_size == rtn->item_size) {
            item_t *tmp_item = (item_t *) malloc(sizeof(item_t) * rtn->item_size << 1);
            memcpy(tmp_item, rtn->items, sizeof(item_t) * rtn->item_size);
            free(rtn->items);
            rtn->items = tmp_item;
            rtn->item_size <<= 1;
        }
        sscanf(data, matcher, tmp[0], tmp[1], tmp[2], tmp[3],
               tmp[4], tmp[5], tmp[6], tmp[7],
               tmp[8], tmp[9], tmp[10], tmp[11],
               tmp[12], tmp[13], tmp[14], tmp[15]);
        for (int j = 0; j < header->_size; ++j) {
            rtn->items[i].data[j] = (char *) malloc(strlen(tmp[j]) + 2);
            strcpy(rtn->items[i].data[j], tmp[j]);
        }
        rtn->_size++;
    }
    return rtn;
}

