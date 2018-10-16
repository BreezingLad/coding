#pragma once

//#include <list/deque.h>
#include"../list/deque.h"

typedef struct hash_bucket {		//从抽象角度来看，这里只是应该保存某个桶的信息。
	deque_queue*		elem_list;
}hash_bucket_t;

typedef unsigned int uint32_t;

//hash函数定义
typedef uint32_t(*hashfunc) (void *data_key, int size);
//查找元素对比函数定义
typedef int(*hashcompare) (deque_queue *data_list, void *key2);
//用于遍历哈希表中元素逐一执行的函数定义
typedef void(*ssdc_hash_exec_t)(deque_queue *data_list, void*mydata, uint32_t index);

typedef struct hash_table {

	uint32_t                 ibuckets;		//hash桶个数
	struct hash_bucket*     buckets;		//桶指针，这里其实是需要一个二级指针。

	uint32_t				current;	//当前hash表元素数量。
	hashfunc				func;		//哈希函数
	hashcompare				cmp;		//查找元素对比函数
} hash_table_t;



hash_table_t* hash_table_init(int buckets, hashfunc hfunc, hashcompare cfunc);

deque_queue *hash_table_get(hash_table_t *tbl, void *key);

int hash_table_insert(hash_table_t *tbl, deque_queue *data, void *key);

deque_queue * hash_table_remove(hash_table_t *tbl, void *key);

void hash_table_destroy(hash_table_t *tbl);

void hash_table_traverse(hash_table_t *tbl, ssdc_hash_exec_t exec_func, void *mydata);


