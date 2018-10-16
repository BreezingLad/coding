#pragma once

/*

http://blog.csdn.net/wm_1991/article/details/51454098

*/

#include<stdlib.h>
#include<string.h>

/*
 * cas(v,a,b)
 * 如果v的值是a, 则将其更新为b,返回程序;
 * 否则返回失败*/
#define CAS __sync_bool_compare_and_swap

#define		CASCycleQueue_Empty	0
#define		CASCycleQueue_Full	1

typedef struct
{
	void*	data;
	char	state;
}Node;

typedef struct
{
	Node* element;
	int front;
    int used;
	int rear;

	int itemNum;
	int	itemSize;
}CASCycleQueue;

inline bool CASCycleQueue_isFull(CASCycleQueue* q)
{
	return (q->rear + 1) % q->itemNum == q->front;
}

inline bool CASCycleQueue_isEmpty(CASCycleQueue* q)
{
    return q->rear == q->front;
}
inline void* CASCycleQueue_at(CASCycleQueue* q, int it)
{
    return q->element[it].data;
}

CASCycleQueue* CreateCASCycleQueue(int itemNum, int itemSize);

//放入消息时，队列已满直接告知用户，不必等到可以放入时放入。
void* CASCycleQueue_push(CASCycleQueue* q);
bool CASCycleQueue_pop(CASCycleQueue* q, void* des);

