/*************************************************************************
	> File Name: stl_char32.h
	> Author: liulianglu
	> Description: 
	> Created Time: Sun 27 Aug 2017 05:23:04 PM EDT
 ************************************************************************/

#ifndef _STL_CHAR32_H
#define _STL_CHAR32_H

#include<string.h>
#include<stdlib.h>

typedef char char32[32];

class stl_char32
{
public:
    char32  data;
    stl_char32()    { memset(data, 0, sizeof(data)); }
    stl_char32(const char* str) { strncpy(data, str, sizeof(data) - 1); data[31] = '\0'; }

    stl_char32& operator=(const char* str){ strncpy(data, str, sizeof(data) - 1); return *this; }

    bool operator==(const char* str)    
    {
        register int i = 0;
        while(*str == data[i] && data[i] != '\0') i++, str++;
        return *str == '\0' && data[i] == '\0';

    }


};


#endif
