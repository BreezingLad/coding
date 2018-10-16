/*************************************************************************
	> File Name: CycleQueue.c
	> Author: liulianglu
	> Description: 
	> Created Time: Sat 26 Aug 2017 09:33:39 PM EDT
 ************************************************************************/

#include<iostream>
#include<string>    //信号量名称
using namespace std;



#include<sys/shm.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include"CycleQueue.h"

void debugCycleQueue(CycleQueue* q)
{
    cout<<"itemSize: "<<q->itemLen<<" front: "<<q->front
        <<" rear: "<<q->rear<<" used: " <<q->used<<endl;

}

int mapstring2int(const char* str)
{
    int result = 0;
    while(*str != '\0')
    {
        result += *str++;
    }
    return result;
}

/*
 *status: 0, open shm; 1, create shm; -1, failed.
 */
void* shm_alloc(const char* key, int size, char* status)
{
    *status = 1;
    int iRet = shmget((key_t) mapstring2int(key), size, 0644|IPC_CREAT|IPC_EXCL);
    if(iRet == -1)
    {
        if(errno == EEXIST)
        {
            iRet = shmget((key_t)mapstring2int(key), size, 0644|IPC_CREAT);
            if(iRet == -1)
            {
                *status = -1;
                return NULL;
            }
            *status = 0;
        }else 
        {
            *status = -1;
            return NULL;
        }
    }

    void* mem = shmat(iRet, 0, 0);
    if(mem == (void*)-1)
    {
        *status = -1;
        return NULL;
    }

    return mem;
}

CycleQueue* CreateCycleQueue(const char* key, int size, int itemSize)
{
    char isCreated = 1;
    void* mem = shm_alloc(key, sizeof(CycleQueue) + size * itemSize, &isCreated);

    if(!mem)
    {
        return NULL;
    }

    CycleQueue* q = (CycleQueue*)mem;
    //q->item = mem; 这个地址是进程私有的，照样不能写到共享内存中去的！

     string strEmpty = string(key) + ".empty",
           strFull  = string(key) + "full";
    
   

    if(isCreated)
    {
        q->size     =   size;
        q->itemLen  =   itemSize;
        q->front    =   0;
        q->rear     =   0;
        q->used     =   0;

        pthread_mutex_init(&q->m_mutex, NULL);
        //sem_init(&q->hFull, 0, 0);
        //sem_init(&q->hEmpty, 0, q->size);
         pthread_mutex_lock(&q->m_mutex);


         sem_unlink(strFull.data());
         q->hFull = sem_open(strFull.data(), O_CREAT|O_EXCL, 0755, 0);
         if(q->hFull == SEM_FAILED)
         {
             DestroyCycleQueue(q);

             pthread_mutex_unlock(&q->m_mutex);
             cout<<"create,"<<strFull<<"failed"<<endl;
             return NULL;        
         }
         sem_unlink(strEmpty.data());
         q->hEmpty = sem_open(strEmpty.data(), O_CREAT|O_EXCL, 0755, q->size);
         if(q->hEmpty == SEM_FAILED)
         {
             DestroyCycleQueue(q);
             pthread_mutex_unlock(&q->m_mutex);
             cout<<"create,"<<strEmpty<<" failed"<<endl;
             return NULL;
         }
         pthread_mutex_unlock(&q->m_mutex);
    }else
    {
         pthread_mutex_lock(&q->m_mutex);


         sem_unlink(strFull.data());
         q->hFull = sem_open(strFull.data(), O_CREAT, 0755, 0);
         if(q->hFull == SEM_FAILED)
         {
             DestroyCycleQueue(q);

             pthread_mutex_unlock(&q->m_mutex);
             cout<<"open,"<<strFull<<"failed"<<endl;
             return NULL;        
         }
         sem_unlink(strEmpty.data());
         q->hEmpty = sem_open(strEmpty.data(), O_CREAT, 0755, q->size);
         if(q->hEmpty == SEM_FAILED)
         {
             DestroyCycleQueue(q);
             pthread_mutex_unlock(&q->m_mutex);
             cout<<"open,"<<strEmpty<<" failed"<<endl;
             return NULL;
         }
         pthread_mutex_unlock(&q->m_mutex);

    }

       
    
    debugCycleQueue(q);

    return q;
}

void DestroyCycleQueue(CycleQueue* q)
{
    if(q)
    {

        pthread_mutex_destroy(&q->m_mutex);

        if(q->hFull != SEM_FAILED)
        {
            sem_close(q->hFull);
            q->hFull = SEM_FAILED;
        }

        if(q->hEmpty != SEM_FAILED)
        {
            sem_close(q->hEmpty);
            q->hEmpty = SEM_FAILED;
        }
        //sem_close(&q->hFull);
        //sem_close(&q->hEmpty);
        
        shmdt(q);

        q = NULL;
    }
}

void* CycleQueue_push(CycleQueue* q)
{
    void* current = NULL;

    int rv = 0;
    if((rv = sem_wait(q->hEmpty)) != 0 && errno == EINTR)
    {
        cout<<"wait_error"<<endl;
    }

    pthread_mutex_lock(&q->m_mutex);

    current = (unsigned char*) q + sizeof(CycleQueue) + q->itemLen * q->front;
    q->front = ++q->front % q->size;
    q->used++;
    pthread_mutex_unlock(&q->m_mutex);

    sem_post(q->hFull);

    return current;
}

int CycleQueue_pop(CycleQueue* q, void* des)
{
    void* current = NULL;

    int rv = 0;
    if((rv = sem_wait(q->hFull))!= 0 && errno == EINTR)
    {
        cout<<"wait error"<<endl;
    }
    pthread_mutex_lock(&q->m_mutex);
    //current = (unsigned char*)q->item + q->rear * q->itemLen;
    current = CycleQueue_at(q, q->rear);
    if(des)
    {
        memcpy(des, current, q->itemLen);
    }
    q->rear = ++q->rear % q->size;
    q->used--;
    pthread_mutex_unlock(&q->m_mutex);

    sem_post(q->hEmpty);
    return  0;
}
