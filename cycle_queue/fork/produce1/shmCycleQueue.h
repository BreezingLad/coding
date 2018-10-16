/*************************************************************************
	> File Name: shmCycleQueue.h
	> Author: liulianglu
	> Description: 
	> Created Time: Fri 01 Sep 2017 02:20:48 AM EDT
 ************************************************************************/

#ifndef _SHMCYCLEQUEUE_H
#define _SHMCYCLEQUEUE_H

#include<pthread.h>
#include<semaphore.h>

#include<string>
using std::string;

typedef struct CycleQueue
{
    int     front;
    int     used;
    int     rear;
    pthread_mutex_t mutex;
}CycleQueue,*pCycleQueue;

class shmCycleQueue
{
public:
    shmCycleQueue();
    ~shmCycleQueue();

    char Create(const char* name, int itemSize, int itemNum);
    char Open(const char* name, int itemSize, int itemNum);

    void Close();

    void*   CycleQueue_push(int miliTimeout);
    char    CycleQueue_pop(void* des, int miliTimeout);

private:
    CycleQueue*         m_queue;
    pthread_mutex_t*    m_hmutex;
    sem_t*              m_hFull;
    sem_t*              m_hEmpty;

    string              m_shmName;
    int                 m_ItemNum;
    int                 m_ItemSize;

    void Init();
    void ReleaseHandle();


    void*  CycleQueue_at(int it)
    {
        return (unsigned char*)m_queue + 
            sizeof(CycleQueue) + it * m_ItemSize;
    }

};

#endif
