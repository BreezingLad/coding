
#include<stdlib.h>
#include "hashtable.h"

hash_table_t* hash_table_init(int buckets, hashfunc hfunc, hashcompare cfunc)
{
	hash_table_t* local = (hash_table_t*)malloc(sizeof(hash_table_t));
	if (local)
	{
		local->buckets = (hash_bucket_t *)malloc(buckets * sizeof(hash_bucket_t));
		if (!local->buckets)
		{
			free(local);
			return NULL;
		}

		local->ibuckets = buckets;
		local->func = hfunc;
		local->cmp = cfunc;
		local->current = 0;

		queue_init(local->buckets->elem_list);
	}
	
	return local;
}

deque_queue *hash_table_get(hash_table_t *tbl, void *key)
{	
	hash_bucket_t bucket = tbl->buckets[tbl->func(key, tbl->ibuckets)];

}