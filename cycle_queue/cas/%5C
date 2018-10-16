/*************************************************************************
	> File Name: testSTL_char.cpp
	> Author: liulianglu
	> Description: 
	> Created Time: Thu 07 Sep 2017 05:32:24 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;

#include"stl_char.h"

int main(int argc, char *argv[])
{
    //1.not waste a bit memory
    std::cout<<"length of stl_char32: "<<sizeof(stl_char32)<<endl;

    //conveniency
    const char* strName = "xiaoliu";
    stl_char32 stlName = strName;

    const char* strName1 = "XiaoLiu";
    const char* strName2 = "xiaoming";

    if(stlName == stlName)
    {
        cout<<"compared ignored case!"<<endl;
    }

    if(stlName != stlName)
    {
        cout<<"Could judged different string"<<endl;
    }
    
    return 0;
}
