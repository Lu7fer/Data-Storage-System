/** 
 * @Author : Lu7fer
 * @Date: 2020-06-18 22:06:14
 * @LastEditTime: 2020-06-20 17:39:31
 * @FilePath: /Data-Storage-System/user.c
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

#ifndef _USER
#define _USER
#endif

#ifndef _INC_STDLIB
#include <stdlib.h>
#endif
#ifndef _INC_STRING
#include <string.h>
#endif

//! #define node_t
//! todo typedef ? nodedata

#ifndef _UTILITY
#include "utility.h"
#endif /* utility.h */


int dss_login(char *passwd, char *input)
{
    return strcmp(passwd,input);
}

int dss_modify_passwd(){
    puts("Please input new passwoed:");
    int i;
}