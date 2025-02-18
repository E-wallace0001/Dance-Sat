#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "../source/headers/group.h"

int main(){


	printf(" creating array of set_s \n");
	size_t size = 10;

	set_s** array;
	array = MakeSetArray( size);

	for ( size_t i = 0; i< size; i++){
	
		assert(array[i]!=NULL);
	
	}
	printf(" array check complete \n");
	
	printf(" appending set of array \n");
	
	set_s* set = array[0];
	
	int count = 1;
	int* this = malloc(sizeof(int*)) ;
	*this = count;
	for( size_t i =0; i < 10; i++){
		
		ExtendSet( this, set);
		
		printf(" %i \n", *(int*) this);
		this= malloc( sizeof(int*) );
		count++;
		(*this) = count ;
		
		//this=&count;
		//count2++;
	}
	free(this);
	printf(" appending complete \n");
	printf("summing list \n");
	int sum =0;	
	list_s* list= set->first;
	while(1){
	
		 if (list ==NULL){
			printf("FAIL empty list->data\n");
			exit(0);
		}
	
		sum+= *(int*) list->data;
		printf(" Data:%i sum : %i\n", *(int*) list->data, sum);
		if( list->next==NULL)
			break;
		list= list->next;
	}
	
	assert( sum ==55);
	printf(" summing forward complete\n");
	
	while(1){
	
		if (list ==NULL){
			printf("FAIL empty list->data\n");
			exit(0);
		}
		
		sum-= *(int*) list->data;
		printf(" Data:%i sum : %i\n", *(int*) list->data, sum);
		if( list->previous==NULL)
			break;
		list= list->previous;
	}	
	assert(sum == 0);
	printf("reverse list complete \n");

	set= array[0];
	printf("removing from set\n");
	set_s* removed_set=MakeSet();
	
	RemoveFromSet(&list, &set, &removed_set );
	
	RemoveFromSet(&list, &set, &removed_set );
	RemoveFromSet(&list, &set, &removed_set );
	RemoveFromSet(&list, &set, &removed_set );
	RemoveFromSet(&list, &set, &removed_set );
	list = set->end;
	RemoveFromSet(&list, &set, &removed_set );
	RemoveFromSet(&list, &set, &removed_set );

	printf("summing list \n");
	sum =0;	
	list= set->first;
	while(1){
	
		 if (list ==NULL){
			printf("FAIL empty list->data\n");
			exit(0);
		}
	
		sum+= *(int*) list->data;
		printf(" Data:%i sum : %i\n", *(int*) list->data, sum);
		if( list->next==NULL)
			break;
		list= list->next;
	}
	//assert( sum ==30);
	
	//assert( sum == 21);
	printf(" summing forward complete\n");
	
	while(1){
	
		if (list ==NULL){
			printf("FAIL empty list->data\n");
			exit(0);
		}
		
		sum-= *(int*) list->data;
		printf(" Data:%i sum : %i\n", *(int*) list->data, sum);
		if( list->previous==NULL)
			break;
		list= list->previous;
	}	
	assert(sum == 0);
	
	printf("reverse list complete \n");

	printf("reinstating removed in reverse order \n");
	ReinstateVariable(&removed_set );		
	ReinstateVariable(&removed_set );		
	ReinstateVariable(&removed_set );		
	ReinstateVariable(&removed_set );		
	ReinstateVariable(&removed_set );		
	ReinstateVariable(&removed_set );		
	ReinstateVariable(&removed_set );		
	

	printf("summing list \n");
	sum =0;	
	list= set->first;
	while(1){
	
		 if (list ==NULL){
			printf("FAIL empty list->data\n");
			exit(0);
		}
	
		sum+= *(int*) list->data;
		printf(" Data:%i sum : %i\n", *(int*) list->data, sum);
		if( list->next==NULL)
			break;
		list= list->next;
	}
	
	assert( sum == 55);
	printf(" summing forward complete\n");
	
	while(1){
	
		if (list ==NULL){
			printf("FAIL empty list->data\n");
			exit(0);
		}
		
		sum-= *(int*) list->data;
		printf(" Data:%i sum : %i\n", *(int*) list->data, sum);
		if( list->previous==NULL)
			break;
		list= list->previous;
	}	
	
	assert(sum == 0);
	printf("reverse list complete \n");
	
	
	printf("create group \n");
	
	group_s* formula = MakeGroup();
	assert(formula!=NULL);
	
	printf(" group creation success \n");

	// access to clause
	set_s* s1 = (set_s*)formula->end->data;
	
	int* num;
	
	
	// adding literals
	printf(" extending literals \n");
	num = malloc(sizeof(*num));
	*num = 1;
	ExtendSet( num , s1);
	
	num = malloc(sizeof(*num));
	*num = 3;

	ExtendSet( num , s1);	num = malloc(sizeof(*num));
	*num = 4;
	sum =0;
	ExtendSet( num , s1);
	s1->list = s1->first;
	while(s1->list!=NULL){
		printf(" num %i \n", *(int*)s1->list->data);
		sum+=*(int*)s1->list->data;
		s1->list=s1->list->next;
	}
	assert(sum == 8);

	printf(" first clause made \n");
	printf(" temporarily removeing first \n");
	s1->list = s1->first;
	s1->list = s1->list->next;	
	RemoveFromSet(&s1->list, &s1, &removed_set );
	
	sum =0;
	s1->list = s1->first;
	while(s1->list!=NULL){
		printf(" num %i \n", *(int*)s1->list->data);
		sum+=*(int*)s1->list->data;
		s1->list=s1->list->next;
	}
	
		
	assert( sum == 5);
	
	sum = 0;
	removed_set->list=removed_set->first;
	printf( " count set %i \n",CountGroupSet(removed_set) );

printf(" set post \n");
		sum =0;	
		num = malloc(sizeof(*num));
		*num = 3;
		s1->list = s1->first;
		s1->list = s1->list->next;
		SetPostInsert( num,&s1->list, &s1);
		s1->list = s1->first;
		
		while(s1->list!=NULL){
			printf(" num %i \n", *(int*)s1->list->data);
			sum+=*(int*)s1->list->data;
			s1->list=s1->list->next;
		}
		printf(" end num %i \n", *(int*)s1->end->data);
		s1->list = s1->first;
		num = malloc(sizeof(*num));
		*num = 222;
		s1->list = s1->list->next->next;
		SetPostInsert( num,&s1->list, &s1);
		s1->list = s1->first;
		
		list_s* saved = NULL;
		
		while(s1->list!=NULL){
			printf(" num %i \n", *(int*)s1->list->data);
			sum+=*(int*)s1->list->data;
			
			saved = s1->list;
			
			s1->list=s1->list->next;
		}
		printf("\n");
		
		
		s1->list = s1->end;
		while(s1->list!=NULL){
			printf(" num %i \n", *(int*)s1->list->data);
			sum+=*(int*)s1->list->data;
		
			s1->list=s1->list->previous;
		}
		
		//printf("s1->first data %i %i \n", *(int*)saved->data,*(int*)s1->first->next->data );
		printf("s1->sec data %i \n", *(int*)s1->first->next->next->next->data );
		printf("s1->sec data %i \n", *(int*)s1->first->next->data );
		MoveSet( s1->end, s1->first, s1);
		printf("s1->sec data %i \n", *(int*)s1->first->next->next->next->data );
		printf("s1->sec data %i \n", *(int*)s1->first->next->data );
		s1->list = s1->first;
		
		
		while(s1->list!=NULL){
			printf(" few num %i \n", *(int*)s1->list->data);
			sum+=*(int*)s1->list->data;
			
			s1->list=s1->list->next;
		}
		printf("\n");
		
		
		s1->list = s1->end;
		while(s1->list!=NULL){
			printf(" bck num %i \n", *(int*)s1->list->data);
			sum+=*(int*)s1->list->data;
		
			s1->list=s1->list->previous;
		}
		MoveSet( s1->end, s1->first->next, s1);
		
		printf("fwd\n");
		s1->list = s1->first;
			while(s1->list!=NULL){
			printf(" fwd num %i \n", *(int*)s1->list->data);
			sum+=*(int*)s1->list->data;
			
			s1->list=s1->list->next;
		}
		printf("diff\n");
		
		
				s1->list = s1->end;
		while(s1->list!=NULL){
			printf(" bck num %i \n", *(int*)s1->list->data);
			sum+=*(int*)s1->list->data;
		
			s1->list=s1->list->previous;
		}
	exit(0);
	/*
	ReinstateVariable(&removed_set );
	
	s1->list = s1->first;
	while(s1->list!=NULL){
		printf(" num %i \n", *(int*)s1->list->data);
		sum+=*(int*)s1->list->data;
		s1->list=s1->list->next;
	}
	assert( sum ==8 );
	*/
	printf(" resseting mem\n");
	// scratch code
	Del_GS_Mem( &removed_set );
	FreeSetArray(array,size);
	printf(" complete \n");
	return 0;


}
