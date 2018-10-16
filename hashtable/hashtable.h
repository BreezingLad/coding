#pragma once

//#include <list/deque.h>
#include"../list/deque.h"

typedef struct hash_bucket {		//�ӳ���Ƕ�����������ֻ��Ӧ�ñ���ĳ��Ͱ����Ϣ��
	deque_queue*		elem_list;
}hash_bucket_t;

typedef unsigned int uint32_t;

//hash��������
typedef uint32_t(*hashfunc) (void *data_key, int size);
//����Ԫ�ضԱȺ�������
typedef int(*hashcompare) (deque_queue *data_list, void *key2);
//���ڱ�����ϣ����Ԫ����һִ�еĺ�������
typedef void(*ssdc_hash_exec_t)(deque_queue *data_list, void*mydata, uint32_t index);

typedef struct hash_table {

	uint32_t                 ibuckets;		//hashͰ����
	struct hash_bucket*     buckets;		//Ͱָ�룬������ʵ����Ҫһ������ָ�롣

	uint32_t				current;	//��ǰhash��Ԫ��������
	hashfunc				func;		//��ϣ����
	hashcompare				cmp;		//����Ԫ�ضԱȺ���
} hash_table_t;



hash_table_t* hash_table_init(int buckets, hashfunc hfunc, hashcompare cfunc);

deque_queue *hash_table_get(hash_table_t *tbl, void *key);

int hash_table_insert(hash_table_t *tbl, deque_queue *data, void *key);

deque_queue * hash_table_remove(hash_table_t *tbl, void *key);

void hash_table_destroy(hash_table_t *tbl);

void hash_table_traverse(hash_table_t *tbl, ssdc_hash_exec_t exec_func, void *mydata);


