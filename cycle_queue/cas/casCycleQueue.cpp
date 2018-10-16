#include "casCycleQueue.h"

CASCycleQueue* CreateCASCycleQueue(int itemNum, int itemSize)
{
	CASCycleQueue* q = (CASCycleQueue*)malloc(sizeof(CASCycleQueue) + sizeof(Node) * itemNum * itemSize);
	if (q)
	{
		q->element = (Node*)(q + sizeof(CASCycleQueue));
		for (int i = 0; i < itemNum; i++)
		{
			q->element[i].data = (unsigned char*)q + sizeof(CASCycleQueue) + sizeof(Node) * i;
			q->element[i].state = CASCycleQueue_Empty;
		}
		q->itemNum = itemNum;
		q->itemSize = itemSize;
		q->front = 0;
		q->rear = 0;
	}
	return q;	
}



//放入消息时，队列已满直接告知用户，不必等到可以放入时放入。
void* CASCycleQueue_push(CASCycleQueue* q)
{

	do  //多个线程到达时，仅有一个线程会竞争成功。
        //而竞争成功后的线程在移动了q->rear之后
        //——标志位可能改变，其它线程才能得到资源——或者是队列已满！
        //这样，两句CAS就实现了对q->rear的互斥访问
	{
        if(CASCycleQueue_isFull(q))
        {
            return NULL;
        }
	} while (!CAS(&q->element[q->rear].state, CASCycleQueue_Empty, CASCycleQueue_Full));
	
    void* current = q->element[q->rear].data;
    //看的博客这里是没有循环的，好像是一定会成功：自己的位置存储的是自己的元素。
    while(!CAS(&q->rear, q->rear, (q->rear + 1) % q->itemNum));
    return current;
}
bool CASCycleQueue_pop(CASCycleQueue* q, void* des)
{
    do
    {
        if(CASCycleQueue_isEmpty(q))
        {
            return false;
        }
    }while(!CAS(&q->element[q->front].state,
                CASCycleQueue_Full, 
                CASCycleQueue_Empty));
    if(des)
    {
        memcpy(des, q->element[q->front].data, q->itemSize);
    }

    CAS(&q->front,  q->front, (q->front + 1) % q->itemNum);
    return true; 

}
