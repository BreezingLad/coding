/*************************************************************************
	> File Name: testCASCycleQueue.c
	> Author: liulianglu
	> Description: 
	> Created Time: Sun 27 Aug 2017 05:15:27 PM EDT
 ************************************************************************/
#include<unistd.h>

#include<iostream>
#include<vector>
using namespace std;

#include"stl_char.h"
#include"casCycleQueue.h"

void testCASCycleQueue();

int main(int argc, char *argv[])
{
    testCASCycleQueue();
    return 0;
}


class Student
{
public:
    Student() {}
    Student(const char* strName, int iAge, char chSex = 1):name(strName), age(iAge), sex(chSex){}
    stl_char32  name;
    int         age;
    char        sex;
};

class testThread
{
public:

    pthread_t  handle;

    int seq;
    int times;
    
    int success;
    char    statues;

};

int gTestTimes;

int gProducerNum;
int gConsumerNum;

vector<testThread*>     producerThread;
vector<testThread*>     consumerThread;

CASCycleQueue*      gQueue;
pthread_cond_t      gCond;

void PrintStudentQuqueue(CASCycleQueue* q)
{
    Student* current = NULL;
    cout<<"print stuendt queue:";
    if(CASCycleQueue_isFull(q))
    {
        for(int i = 0; i < q->itemNum; i++)
        {
            current = (Student*)CASCycleQueue_at(q, i);
            cout<<current->name.elements <<' ' <<current->sex<<' '<<current->age<<"\n";
        }
    }else 
    {
        for(int i = q->rear; i != q->front; i = ++i%q->itemNum)
        {
            current = (Student*)CASCycleQueue_at(q, i);
            cout<<current->name.elements << ' '<<current->sex<<' '<<current->age<<"\n";

        }   
    }
    cout<<endl;
}

void* Produce(void* args)
{
    static int seq = 0;
    int     index = seq++;

    Student* producer = NULL;
    for(register int i = 0; i < gTestTimes; i++)
    {
        producer = (Student*)CASCycleQueue_push(gQueue);
        if(producer)
        {
            producer->name = "liulianglu";
            producer->sex   = 1;
            producer->age   = i;
            producerThread.at(index)->success++;
            cout<<"produce"<<endl;
        }else
        {
            cout<<"full"<<endl;
            PrintStudentQuqueue(gQueue);
        }

    }

    producerThread.at(index)->statues = 0;
    return NULL;

}

void* Consume(void* args)
{
    static int seq = 0;
    int index = seq++;
    for(register int i = 0; i < gTestTimes; i++)
    {
        Student* consumer = new Student();
        if(!CASCycleQueue_pop(gQueue, consumer))
        {
            consumer->name = "lll";
            consumer->sex = 0;
            consumer->age = i;
            consumerThread.at(index)->success++;
            //cout<<"consume"<<endl;

        }
        else 
        {
            cout<<"empty"<<endl;
        }
    }
    consumerThread.at(index)->statues = 0;
    return NULL;
}
void testCASCycleQueue()
{
    gTestTimes = 1000;
    gProducerNum = 10;
    gConsumerNum = 10;

    int nRet =0;

    gQueue = CreateCASCycleQueue(10, sizeof(Student));
    nRet = pthread_cond_init(&gCond, NULL);
    if(nRet)
    {
        printf("pthread_cond_init error: %d", errno);
        exit(0);
    }

    vector<testThread*>::iterator it = producerThread.begin();

    testThread* newThread = NULL;
    register int i = 0;
    
    for(i = 0; i < gProducerNum; i++)
    {
        newThread = new testThread();
        newThread->statues = 1;
        newThread->seq = i;

        pthread_create(&newThread->handle, NULL, Produce, NULL);

        producerThread.push_back(newThread);
        
        //pthread_join(newThread->handle, NULL);

    }

    for( i = 0; i < gConsumerNum; i++)
    {
        newThread = new testThread();
        newThread->statues = 1;
        newThread->seq = i;
        
        pthread_create(&newThread->handle, NULL, Consume, NULL);

        consumerThread.push_back(newThread);
        //pthread_join(newThread->handle, NULL);
    
    }

    bool isAlive = true;

    do
    {
        
        it = producerThread.begin();
        isAlive = true;
        while(it != producerThread.end())
        {
            if((*it)->statues == 1)
            {
                isAlive = false;
                break;
            }
            it++;
        }
        if(isAlive)
        {
            it = consumerThread.begin();
            while(it != consumerThread.end())
            {
                if((*it)->statues == 1)
                {
                    isAlive = false;
                    break;
                }
                it++;
            }
            if(isAlive)
            {
                int totals = 0;
                for( it = producerThread.begin(); it != producerThread.end(); it++ )
                {
                    totals+=(*it)->success;
                }

                cout<<"Produce:"<<totals;
                totals = 0;
                for(it = consumerThread.begin(); it != consumerThread.end(); it++)
                {
                    totals+= (*it)->success;
                }

                cout<<"Consume:"<<totals<<"Queue used:"<<gQueue->used<<endl;
                break;
            }

            sleep(500);
        }
    }while(true);

    //DestroyCASCycleQueue(gQueue);
}
