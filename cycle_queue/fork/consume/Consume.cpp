/*************************************************************************
	> File Name: Produce.cpp
	> Author: liulianglu
	> Description: 
	> Created Time: Tue 29 Aug 2017 04:07:42 AM EDT
 ************************************************************************/

#include<unistd.h>
#include<iostream>
using namespace std;

#include"Student.h"
#include"CycleQueue.h"

int main(int argc, char *argv[])
{
    int iProcNum = 1;
    int iTimes = 100;
    int iQueueSize = 10;

    pid_t pProc = 0;
    for(int i = 0; i < iProcNum - 1; i++)   //父进程也参与计算，所以要-1
    {
        pProc = fork();
        switch(pProc)
        {
            case 0:     //child process

                break;
            case -1:
                cout<<"fork failed"<<endl;
            defalut:
                cout<<"parent for next!"<<endl;
                continue;
        }
    }

    //test
    CycleQueue* q = CreateCycleQueue("gQueue", iQueueSize, sizeof(Student));

    Student* consumer = new Student();
    for(int i = 0; i < iTimes; i++)
    {
        if(!CycleQueue_pop(q, consumer))
        {
            consumer->name = "consumer";
            cout<<"consume"<<endl;
        }else 
        {
            cout<<"empty queue"<<endl;
        }
    }


}
