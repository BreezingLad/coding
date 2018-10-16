/*************************************************************************
	> File Name: Student.h
	> Author: liulianglu
	> Description: 
	> Created Time: Tue 29 Aug 2017 04:18:09 AM EDT
 ************************************************************************/

#ifndef _STUDENT_H
#define _STUDENT_H

#include"stl_char32.h"

class Student
{
public:
    Student():name("liualinglu"), age(25), sex(1)   {}
    Student(const char* strName, int iAge, char chSex):name(strName), age(iAge), sex(chSex){}
    stl_char32  name;
    int         age;
    char        sex;
};
#endif
