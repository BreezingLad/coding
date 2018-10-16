/*************************************************************************
	> File Name: shmCycleQueue.cpp
	> Author: liulianglu
	> Description: 
	> Created Time: Fri 01 Sep 2017 02:38:51 AM EDT
 ************************************************************************/
#include<sys/shm.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/mman.h>

#include<iostream>
using namespace std;

#include"shmCycleQueue.h"

void shmCycleQueue::Init()
{
    this->m_hmutex = NULL;
    this->m_hFull = NULL;
    this->m_hEmpty = NULL;

    this->m_queue = NULL;
    this->m_ItemSize = 0;
    this->m_ItemNum = 0;
}
void shmCycleQueue::ReleaseHandle()
{
    if(this->m_hmutex)
    {
        pthread_mutex_destroy(this->m_hmutex);
    }
    if(this->m_hFull)
    {
        
    }
    if(this->m_hEmpty)
    {
        
    }
    if(this->m_queue)
    {
        
    }
    this->Init();
}

int MapString2Int(const char* str)
{
    int result = 0;
    while(*str != '\0') result += *str++;
    return result;
}
char shmCycleQueue::Create(const char* name, int size, int num)
{
    this->m_ItemNum = num;
    this->m_ItemSize = size;
    this->m_shmName = name;

    shm_unlink(m_shmName.data());
    int iRet = shmget((key_t)MapString2Int(this->m_shmName.data()), 
            m_ItemNum * m_ItemSize + sizeof(CycleQueue),
            0644|IPC_CREAT|IPC_EXCL);
    if(iRet == -1)
    {
        //not exsit;
        return 0;
    }

    m_queue = (CycleQueue*)shmat(iRet, 0, 0);
    if(m_queue == (void*)-1)
    {
        m_queue = NULL;
        return 0;
    }

    string strEmpty = m_shmName + ".empty",
           strFull = m_shmName + ".full";
           //strMutex = m_shmName + ".mutex";
    pthread_mutexattr_t attr;
    iRet = pthread_mutexattr_init(&attr);
    if(iRet)
    {
        return 0;
    }

    iRet = pthread_mutexattr_settype(&attr, PTHREAD_PROCESS_SHARED);



    pthread_mutex_init(&m_queue->mutex, &attr);


    



    return 1;
}
