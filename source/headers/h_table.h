#ifndef H_TABLE_H
#define H_TABLE_H

typedef struct hasht_bucket{
	
	int64_t clause;
	struct hasht_bucket *previous;
	struct hasht_bucket *next;
	struct hasht_bucket *end;
	struct hasht_bucket *first;

} hasht_bucket;


typedef struct hash_t{
	struct hasht_bucket** 	table;
	int64_t size;

}hash_t;


hash_t* hasht_create(int64_t );

void table_add( int64_t , hash_t* );

bool check_table(int64_t , hash_t* );

void hash_t_destroy( hash_t** );

void hash_t_empty( hash_t**);

#endif
