#include <stdlib.h>
#include <stdio.h>

#include <assert.h>
#include <stdbool.h>
#include "headers/cnf_llist.h"
#include "headers/group.h"


void haltt(){
	while(1){
	char input;
	scanf("%c", &input);
	 if(getchar()!=0){ break;}
	}
}


set_s* MakeSet(){
	set_s* NewSet		= malloc(sizeof(*NewSet));
	NewSet->first	= NULL;
	NewSet->end		= NULL;
	NewSet->list	= NULL;
	return NewSet;
}

list_s* MakeList(void* data){
	list_s* NewMember			= malloc(sizeof(*NewMember));
	NewMember->data			= data;
	NewMember->next			= NULL;
	NewMember->previous 		= NULL;
	
	return NewMember;

}

void ExtendSet(void* data, set_s* set){
	if(set==NULL){
		printf(" ExtendSet empty \n");
		exit(0);
	}
	list_s* NewMember			= malloc(sizeof(*NewMember));
	NewMember->data			= data;
	NewMember->next			= NULL;
	NewMember->previous 		= NULL;

	if( set->list == NULL){
		set->list				= NewMember;
		set->first				= NewMember;
		set->end					= NewMember;
	}else{
		set->end->next			= NewMember;
		NewMember->previous	= set->end;
		set->end					= NewMember;
	}
}
void InsertFirstSet(void* data, set_s* set){
	if(set==NULL){
		printf(" ExtendSet empty \n");
		exit(0);
	}
	list_s* NewMember			= malloc(sizeof(*NewMember));
	NewMember->data			= data;
	NewMember->next			= NULL;
	NewMember->previous 		= NULL;

	if( set->list == NULL){
		set->list				= NewMember;
		set->first				= NewMember;
		set->end					= NewMember;
	}else{
		set->first->next->previous	= NewMember;
		NewMember->next				= set->first;
		set->first						= NewMember;
	}
}
void InsertNumToSet( int num, set_s* set){

	int* this_num = malloc(sizeof(*this_num) );
	*this_num = num;
	ExtendSet( this_num, set);
}

int ReduceSet(list_s* list_t, set_s** set){
	//printf("reduce set \n");
	if( list_t==NULL) return -1;
	free(list_t->data);
	if(list_t->previous!=NULL){
	
		if(list_t->next!=NULL){
			list_t->previous->next	= list_t->next;
			list_t->next->previous	= list_t->previous;
			(*set)->list= list_t->previous;
			free(list_t);
		}else{
			list_t->previous->next	= NULL;
			(*set)->end					= (*set)->end->previous;
			(*set)->list= list_t->previous;
			free(list_t);
		}
	}else{
		if(list_t->next!=NULL){
			list_t->next->previous	= NULL;
			(*set)->first				= list_t->next;
			(*set)->list 				=(*set)->first;
			(*set)->list= list_t->next;
			free(list_t);
			return 1;
		}else{
		
			(*set)->first				= NULL;
			(*set)->end					= NULL;
			(*set)->list= NULL;
			
			free(list_t);
			return 2;
		}
		
	}
	//return 1;
}
int GroupReduceSet(list_s* list_t, set_s** set){
	
	if( list_t==NULL) return -1;
	//free(list_t->data);
	if(list_t->previous!=NULL){
		if(list_t->next!=NULL){
			list_t->previous->next	= list_t->next;
			list_t->next->previous	= list_t->previous;
			(*set)->list= list_t->previous;
			free(list_t);
			
		}else{
			list_t->previous->next	= NULL;
			(*set)->end					= (*set)->end->previous;
			(*set)->list				= list_t->previous;
			free(list_t);
			
		}
	}else{
		if(list_t->next!=NULL){
			list_t->next->previous	= NULL;
			(*set)->first				= list_t->next;
			(*set)->list 				=(*set)->first;
			(*set)->list= list_t->next;
			free(list_t);
			return 1;
		}else{
			(*set)->first				= NULL;
			(*set)->end					= NULL;
			(*set)->list= NULL;
			
			free(list_t);
			return 2;
		}
		
	}
	//return 1;
}

int RemoveFromSet(list_s** list_t1, group_s** set, set_s** removed_set ){
	list_s* list_t = *list_t1;
	if( list_t==NULL) return -1;
	
	GS_mem* save= malloc( sizeof(*save));
	
	//printf(" removed %i \n", *(int*) (*list_t1)->data);
	if(list_t->previous!=NULL){
	
		if(list_t->next!=NULL){
		//printf("1\n");	
			list_t->previous->next	= list_t->next;
			list_t->next->previous	= list_t->previous;
			(*set)->list= list_t->previous;
			
			save->group = *set;
			save->list  = list_t;
			
			ExtendSet( save, *removed_set);
			list_t = (*set)->list;
			
		}else{
		//printf("2\n");
			list_t->previous->next	= NULL;
			(*set)->end					= (*set)->end->previous;
			(*set)->list				= list_t->previous;
			save->group = *set;
			save->list  = list_t;
			
			ExtendSet( save, *removed_set);
			list_t = (*set)->list;
			//printf("end set \n");
			
		}
	}else{
		if(list_t->next!=NULL){
		//printf("3\n");
			list_t->next->previous	= NULL;
			(*set)->first				= list_t->next;
			(*set)->list 				=(*set)->first;
			//(*set)->list= list_t->next;
			
			save->group = *set;
			save->list  = list_t;
			
			ExtendSet( save, *removed_set);
			list_t = (*set)->list;
			return 1;
		}else{
					

			(*set)->first				= NULL;
			(*set)->end					= NULL;
			(*set)->list 				= NULL;
			save->group = *set;
			save->list  = list_t;
			
			ExtendSet( save, *removed_set);
			
			//*list_t1 = NULL;
			
			//printf(" empty set\n ");
			return 2;
		}
		
	}
	return 0;
}

int RemoveFromSetInsertTo(list_s** list_t1, set_s** set, list_s** removed_set, set_s** rem_set ){
	list_s* list_t = (*list_t1);
	if( list_t==NULL) return -1;
	if (*removed_set == NULL){ 
		printf(" removed_set = null \n ");
		return 0;
	}
	
	
	//printf(" numend \n");
	GS_mem* save= malloc( sizeof(*save));
	//printf(" var st %i \n", *(int*)(*set)->data); 
	if((list_t)->previous!=NULL){
		if((list_t)->next!=NULL){
			(list_t)->previous->next	= list_t->next;
			(list_t)->next->previous	= list_t->previous;
			(*set)->list= (list_t)->previous;
			
			save->group = *set;
			save->list  = list_t;
		
			SetPostInsert( save, removed_set, rem_set );	
//			ExtendSet( save, *removed_set);
			//*list_t1 = (*set)->list;
			
		}else{

			(list_t)->previous->next	= NULL;
			(*set)->end					= (*set)->end->previous;
			(*set)->list				= (list_t)->previous;
			save->group = *set;
			save->list  = (list_t);
			SetPostInsert( save, removed_set, rem_set );	
			//	*list_t1 = (*set)->list;
			
		}
	}else{
		if((list_t)->next!=NULL){
			(list_t)->next->previous	= NULL;
			(*set)->first				= (list_t)->next;
			(*set)->list 				=(*set)->first;
			//(*set)->list				= (*list_t1)->next;
			save->group = *set;
			save->list  = (*list_t1);
			SetPostInsert( save, removed_set , rem_set);
			
				//haltt();
			//*list_t1 = (*set)->list;
			//return 1;
		}else{
			//(*set)->first				= NULL;
			//(*set)->end					= NULL;
			//(*set)->list 				= NULL;
			
			save->group = *set;
			save->list  = list_t;
			
			SetPostInsert( save, removed_set, rem_set );

			(*list_t1)->next=NULL;
			(*list_t1)->previous=NULL;

		
			*list_t1 = NULL;
			
			//printf(" empty set\n ");
			return 2;
		}	
		
	}
	return 0;
}

void SetPostInsert( void* insert_this , list_s** removed_set, set_s** rem_set){
// set removed_set->list prior to call
// change atribute->propagated_end[] after this instr
	list_s* set = *removed_set;
	//list_s* list = set;
	list_s* insert = MakeList(insert_this);
	
	GS_mem* data 		= (GS_mem*)insert_this;
	list_s* list		= (list_s*)data->list;
	//group_s* set 		= (group_s*)data->group;
	
	
		if( (set)->next !=NULL){
			//if((set) !=NULL)
			insert->next 								= (*removed_set)->next;
			insert->previous 							= (*removed_set);
			insert->next->previous	   				= insert;

			(*removed_set) 							= insert;
		}else{
			insert->previous 							= (*rem_set)->end;
			insert->next 								= NULL;
			(*rem_set)->end->next					= insert;
			(*rem_set)->end 							= insert;
			//list->next 									= insert;
			return;
			
		}
		
		
}

void MoveSet( list_s* a, list_s* b, set_s* set){
if( a==b){
	printf("equal\n");
	haltt(0);
}
if(b->next == a ||a->previous == b ) 
	return;
//if(a->previous ==b || a->next == b ) 
//	return;
	//printf(" %p \n", b->next);
	//a->previous->next = a->next;
	if(a->next == NULL){
	exit(0);
	
		set->end = a->previous;
		set->end->next = NULL;
		
	}
	//
	if( b->next !=NULL){
		b->next->previous	= a;
		a->previous->next = a->next;
		a->next->previous = a->previous;
		
		a->previous			= b;
		a->next				= b->next;
		
		b->next				= a;
		//haltt();
		//printf("mv %p %p \n",a, a->next);
		//printf(" null \n");
		//haltt();
	}else{
	exit(0);
	}
	

if( a == b  ) {
	printf("1\n");
	exit(0);
}

}

void ReinstateVariable(set_s** removed_set){

	set_s*  set_d		= *removed_set;
	list_s* list_d 	= set_d->end;
	
	GS_mem* data 		= (GS_mem*)list_d->data;
	list_s* list		= (list_s*)data->list;
	group_s* set 		= (group_s*)data->group;
	
	assert(set);

	(*removed_set)->list= (*removed_set)->end;

//printf( " num %i \n", *(int*)(list)->data);

	if( (list)->next == NULL ){
		(set)->end 				= list;
	}else{
		(list)->next->previous = list;
	}
	
	if ( (list)->previous == NULL ){
		(set)->first 				= list;
	}else{
		(list)->previous->next = list;
	}
	
	free( (*removed_set)->end->data);
	GroupReduceSet( (*removed_set)->end, removed_set);
	(*removed_set)->list = (*removed_set)->end;
}

void Del_GS_Mem( set_s** set){
GS_mem* dat;
	list_s* list;	
	while((*set)->end!=NULL){
		
		dat 	= (*set)->end->data;
		list	= (list_s*)dat->list;
		free( list->data );
		free( list );
		free((*set)->end->data);
		
		GroupReduceSet( (*set)->end , (set) );
	}
	free( *set );
}

void RemoveSetElements(set_s** set){
	while( (*set)->end!=NULL ){
		//free( (*set)->end->data );
		GroupReduceSet( (*set)->end , (set) );
	}
	free( *set );	
}

void DelSet(set_s** set){

	while((*set)->end!=NULL){
		free((*set)->end->data);
		GroupReduceSet( (*set)->end , (set) );
	}
	free( *set );
}

void ResetSet(set_s** set){

	while((*set)->end!=NULL){
		//free((*set)->end->data);
		GroupReduceSet( (*set)->end , (set) );
	}
}


// group y
// set 	x
group_s* MakeGroup(){

	set_s*	group 	= MakeSet();
	set_s*	set		= MakeSet();
	ExtendSet(set, group);
	//group->list->data = set;
	return group;
}

void ExtendGroup(group_s* group){

	set_s* NewSet = MakeSet();
	ExtendSet(NewSet, group);
}

set_s* NextSet(group_s* group ){

	list_s* set = group->list;
	
	if( set->next == NULL){
		//printf(" no next \n");
		
		return NULL;
	}

	group->list = group->list->next;
	
	return group->list->data;


}

set_s* PreviousSet(group_s* group){
	
	list_s* set = group->list;

	if( set->previous ==NULL ){	
		//printf(" no next pre-set \n");
		exit(0);
		return NULL;
	}
	group->list = group->list->previous;
	
	return group->list->data;
}


// shrinks group by getting rid of selected set
void ReduceGroup(group_s** group, set_s** set){

	list_s* temp;
	if( (*group)->list->previous!=NULL ){
		temp = (*group)->list->previous;
	}else if( (*group)->list->next!=NULL ){
		temp = (*group)->list->next;
	}else{
		temp = NULL;
	}
	DelSet( (set_s**)&((*group)->list->data) );
	
	GroupReduceSet( (*group)->list, group);
	
	(*group)->list = temp;
	
	if( (*group) ->list!=NULL){
		if ( set != NULL)
			//(*set)= NULL;
		return;
	}
	
	if( set!=NULL ){
		(*set) = (set_s*)(*group) ->list->data;
	}
	
}

set_s** MakeSetArray(size_t array_size){
	set_s** array ;
	array = calloc(array_size+1, sizeof(set_s*));
	for( size_t i =0; i<=array_size; i++){
		array[i] = MakeSet();
	}
	return array;
}

void FreeSetArray ( set_s** array, size_t array_size){
	for( size_t i =0; i<=array_size; i++){
		array[i]->list=array[i]->end;
		
		while( array[i]->end !=NULL){
			free( array[i]->list->data);	
			GroupReduceSet(array[i]->end, &array[i]);
		}	
		
		free( array[i] ) ;
	}
	free( array);

}


void DeleteGroup(group_s** group){
(*group)->list = (*group)->first;
	while((*group)->list!=NULL){
		ReduceGroup( (group), NULL );
		(*group)->list = (*group) ->first;
	}
	free(*group);
}

void CopyGroupSet(set_s* s1, set_s* s2){
	list_s* l1 = s1->first;
	while(l1!=NULL){
		ExtendSet( l1->data, s2);
		l1 = l1->next;
	}
}

int CountGroupSet(set_s* set){
	list_s* l1 = set->first;
	int count=1;
	while(l1!=NULL){
		count++;
		l1=l1->next;
	}
	return count;
}



