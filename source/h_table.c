
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "headers/h_table.h"

hasht_bucket* hasht_indx_create(){
	hasht_bucket* indx = malloc(sizeof(*indx));
	if(indx==NULL){
		printf("error creating a make clause \n");
		exit(0);
	}
		indx->clause=0;
		indx->end=indx;
		indx->first=indx;
		indx->previous=NULL;
		indx->next=NULL;
	return indx;
}

hash_t* hasht_create(int64_t size){


	hash_t* h_table = malloc(sizeof(*h_table));


	hasht_bucket** table = calloc(size, sizeof(hasht_bucket));

	h_table->table	= table;
	h_table->size	= size;

	for( int64_t i = 0; i<=size; i++){
		table[i]=hasht_indx_create();
	}
return h_table;
}

void bucket_end_del( hasht_bucket** bucket_list){
	if (*bucket_list==NULL) exit(0);
	
	hasht_bucket* list = (*bucket_list)->first->end;
	if(list->previous!=NULL){
		list->first->end= list->previous;
		list->previous->next=NULL;
		free(list);
	}else{
		list->first = list;
		list->end = list;
		list->clause=0;
		list->next= NULL;
		list->previous = NULL;
		//list=NULL;
	}

}

hasht_bucket* hasht_add(int64_t val, hasht_bucket* list){


	hasht_bucket* indx	= hasht_indx_create();
	list->end->next 		= indx;
	indx->previous			= list->end;
	indx->first				= list->first;
	list->first->end		= indx;
	indx->clause 			= val;

}


int64_t hashval( int64_t val, hash_t* ht){
	return val % ht->size;
}

bool check_table(int64_t val, hash_t* ht){

	hasht_bucket** table= ht->table;
	if( table==NULL) {
		printf(" null table \n");
		exit(0);
	}

	int64_t hash = hashval(val, ht);

	hasht_bucket* indx = table[hash];

	if( indx->clause == 0){
		
		return 0;
	}
	else{
		while(indx != NULL){
			
			if( indx->clause == val) 
				return 1;
				
			if(indx->next == NULL) 
				return 0;
				
			indx = indx->next;
			
		}
	}
}

void table_add( int64_t val, hash_t* ht){

	hasht_bucket** table = ht->table;

	if( table==NULL) {
		printf(" null table \n");
		exit(0);
	}
	
	hasht_bucket* indx = table[ hashval(val, ht) ];
	
	if( indx->clause==0){
		indx->clause = val;
	}else{
		hasht_add( val, indx);
	}
}

void hash_t_destroy( hash_t** ht){


	int64_t size					= (*ht)->size;
	hasht_bucket** table		= (*ht)->table;
	
	for(int64_t i = 0; i<=size; i++){
	
		while(table[i]->first->end->previous != NULL){
			bucket_end_del( &table[i] );
		}
		free(table[i]);
	}
	free(table);
	free(*ht);
	ht = NULL;
}

void hash_t_empty( hash_t** ht){


	int64_t size					= (*ht)->size;
	hasht_bucket** table		= (*ht)->table;
	
	for(int64_t i = 0; i<=size; i++){
		while(table[i]->first->clause != 0){
			bucket_end_del( &table[i] );
		}
		
		//free(table[i]);
	}
}

