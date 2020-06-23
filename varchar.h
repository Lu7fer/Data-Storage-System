/*
 * @Author : Lu7fer
 * @Date: 2020-06-20 17:12:23
 * @LastEditTime: 2020-06-22 00:12:15
 * @FilePath: /Data-Storage-System/varchar.h
 * @Stu_ID: 2019X....X229_Lu7fer
 * @Github: https://github.com/Lu7fer/C-language-study-homework
 * @Copyright
-------------------------------------------
Copyright (C) 2020 - Lu7fer
C-language-study-homework is free software:
you can redistribute it and/or modify it under the terms of 
the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
 
You should have received a copy of the GNU General Public License 
along with C-language-study-homework. 
If not, see <http: //www.gnu.org/licenses/>.
-------------------------------------------
 */

#ifndef _VARCHAR
#define _VARCHAR
#endif

#ifndef _INC_STRING
#include <string.h>
#endif

#ifndef _UTILITY
#include "utility.h"
#endif

struct varchar
{
    u_int capacity;
    char *string;
};
typedef struct varchar varchar_t;

varchar_t *manager;
u_int manager_used = 0;
u_int manager_capacity = 0;

varchar_t *dss_new_string(u_int length)
{
    if (length < 2)
        return NULL;
    u_int i;
    //获得字符串
get:
    if (manager_used < manager_capacity)
    {
        for (i = 0; i < manager_capacity; i++)
        {
            if (manager[i].capacity < 2)
            {
                manager[i].string = (char *)malloc(length + 1);
                manager[i].capacity = length + 1;
            }
            return &(manager[i]);
        }
    }
    else
    {
        u_int exbanded_len;
        if (manager_capacity == 0)
        {
            exbanded_len = 16;
        }
        else
        {
            exbanded_len = manager_capacity << 1;
        }
        varchar_t *tmp = (varchar_t *)calloc(exbanded_len, sizeof(varchar_t));
        memcpy(tmp, manager, manager_capacity * sizeof(varchar_t));
        manager_capacity = exbanded_len;
        goto get;
    }
}

void dss_free_string(varchar_t *str)
{
    free(str->string);
    str->capacity = 0;
}

void dss_free_all_str(){
    u_int i;
    for ( i = 0; i < manager_capacity; i++)
    {
        if(manager[i].capacity!=0){
            free(manager[i].string);
        }
    }
    
}