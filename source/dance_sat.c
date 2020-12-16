#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include "headers/cnf_llist.h"
#include "headers/group.h"
#include "headers/h_table.h"
#include "headers/graph.h"
#include "headers/dsat.h"

int learn_count =0;



void halt(){
	while(1){
	char input;
	scanf("%c", &input);
	 if(getchar()!=0){ break;}
	}
}

// this finds initial implication before choice
void Find_Partial_Bijection(group_s* end, formula_atribute* problem, hash_t* h_table){

	list_s* literal;
	GS_mem* mem;
	list_s* node ;
	int count	=0;
	bool* tried	=NULL;
	tried 		= calloc(nr_variables+1, sizeof(bool));

	bool match;
	problem->removed_set->list = problem->removed_set->end;		
	mem 								= (GS_mem*)problem->removed_set->end->data;
	literal 							= (list_s*)mem->list;
	group_s* group 				= (group_s*)mem->group;	
	
	//////////printf(" biject %p %p \n", variable_container, group);
	//printf(" var %i \n", *(int*)literal->data);
	while(1 ){

		if( check_table( (int64_t)group, h_table) ){
		
			//printf(" %i %i  \n ",abs(*(int*) literal->data) , problem->propagated[ abs(*(int*) literal->data) ] );
			if ( problem->assignment[ abs(*(int*) literal->data) ] == 0 && problem->propagated[ abs(*(int*) literal->data) ]  == 0 && problem->known [ abs(*(int*) literal->data)  ] == 0 ){
			
				//this is the clause that needs reguessing
				//table_add( (int64_t)problem->prop_cause[abs( *(int*)literal->data )], h_table);
				free( tried );
				return;
			}
				
			if ( problem->propagated[ abs( *(int*) literal->data) ] == 1  && tried[abs(*(int*) literal->data)]==0 ){
				// if this is propagated, continue on till the last  non- propagated variable
				if( check_table( (int64_t)problem->prop_cause [ abs( *(int*)literal->data)  ] , problem->h_table3)==0 )
					table_add( (int64_t)problem->prop_cause[abs( *(int*)literal->data )], problem->h_table3);
				
				table_add( (int64_t)problem->prop_cause[abs( *(int*)literal->data )], h_table);
				tried[abs(*(int*) literal->data)]=1;
				count++;
			}
				
		}
	

		if( problem->removed_set->list->previous == NULL){
			printf(" no previous bijection \n");
			exit(0);
			return;
		}
		
		problem->removed_set->list = problem->removed_set->list->previous;
		mem 								= (GS_mem*)problem->removed_set->list->data;
		group 							= (group_s*)mem->group;	
		literal 							= (list_s*)mem->list;
		if ( end == group ){
			free( tried );
			return ;
		}
	}
	free( tried );
}

// this finds where a previous literal was remvoed
void LearnClause(formula_atribute* problem, hash_t* h_table, bool restart, int root){

	bool empty =0;
	list_s* l1;
	list_s* literal;
	GS_mem* mem;
	list_s* node ;
	
	list_s* literal2;
	group_s* group2;
	GS_mem* mem2;
	
	list_s* literal3;
	
	int count=0;

	bool* tried=NULL;
	tried = calloc(nr_variables+1, sizeof(bool));

	bool match;
	
	list_s* saved = problem->removed_set->end;

	problem->removed_set->list = problem->removed_set->end;	
	if (problem->removed_set->list ==NULL)
		return;	
	mem 								= (GS_mem*)problem->removed_set->end->data;
	literal3 							= (list_s*)mem->list;
	
	mem 								= (GS_mem*)problem->removed_set->list->data;
	literal 							= (list_s*)mem->list;
	group_s* group 				= (group_s*)mem->group;	
	
	int size_new_clause	= 0;
	bool save_ret			= 0 ;
		
	set_s* new_clause = NULL;
	int* new_var;
	GS_mem* group_set;
	bool prop1 = 0;
	printf("end rem lit %i \n",*(int*) literal->data);
	if (restart==1) {
		tried[abs(*(int*) literal3->data)] = 1;
		//tried[abs( root)] = 1;
	}else{
		
	}
	tried[abs(root)] = 1;
	int clause_sz_cnt =0 ;
		
	printf("\n");
	while(1 ){
		count++;
		if(  check_table( (int64_t)group, h_table)==1 ){
		
			if ( tried[abs(*(int*) literal->data)]==0    ){
			
				// if new clause is to be created
				if(new_clause == NULL){
					new_clause = (set_s*)problem->formula->end->data;
					nr_clauses++;
					
				}
				if ( new_clause != NULL){
				
					tried[abs(*(int*) literal->data)] = 1;
					//flip the set variable. if currently set to 1, it cannot be set to 1 so negate	
					if(problem->assignment[ abs(*(int*) literal->data) ] == 1){
						new_var =  malloc( sizeof(*new_var)) ;
						*new_var = -abs(*(int*) literal->data) ;
						ExtendSet( new_var , new_clause);
					}

					if(problem->assignment[ abs(*(int*) literal->data) ]==0){
						new_var =  malloc( sizeof(int)) ;
						*new_var = abs(*(int*) literal->data);
						ExtendSet( new_var , new_clause);
					}
					
					printf(" %i ", *new_var);
					//connects this variable to its group
					new_clause->end->group = new_clause;
					
					// this create a point to mark the placement of the variable
					group_set				 = malloc(sizeof(*group_set));
					group_set->list		 = new_clause->end;
					group_set->group  	 = new_clause;
					group_set->clause_num = nr_clauses;
					InsertFirstSet( group_set, problem->variable_position[ abs(*(int*) literal->data) ] ) ; 
					clause_sz_cnt++;
				}
					
				//table_add( (int64_t)problem->prop_cause[abs( *(int*)literal->data )], h_table);
			}
		}
		
		if( problem->removed_set->list->previous == NULL){
			break;
		}
		problem->removed_set->list = problem->removed_set->list->previous;
		mem 								= (GS_mem*)problem->removed_set->list->data;
		group 							= (group_s*)mem->group;	
		literal 							= (list_s*)mem->list;
	}
	printf("\n");
	
	 new_clause=new_clause->end->group ;
	new_clause->var_list_size = clause_sz_cnt;
	new_clause->var_list      = malloc ( clause_sz_cnt * sizeof(int));
	
	new_clause->list = new_clause->first;
	for( int x = 0 ; x < clause_sz_cnt; x++){
			if (new_clause->list == NULL)
			exit(0);
		new_clause->var_list[x] = *(int*)new_clause->list->data;
		new_clause->list = new_clause->list->next;

	
	}
	new_clause->list = new_clause->first;
	
	
	// remove the variables from this clause, move the removed variable to the end of the variables removed.
	
	if (new_clause != NULL){
		// create a new clause at the end
		ExtendGroup( problem->formula );
		//empty the variable
		//hash_t_empty( &h_table );
		
		free(tried);
		
		new_clause->list = new_clause->end;
				
		while(new_clause->list!=NULL){
			RemoveFromSetInsertTo(&new_clause->list, &new_clause, (list_s**)&problem->propagated_end[ abs(*(int*)new_clause->list->data) ], &problem->removed_set );
			table_add( (int64_t)new_clause, h_table);
		}
		
		printf(" added new clause \n");
		return;
	}
	
	if(tried!=NULL)
		free( tried );	
	return;
}

// this finds where a previous literal was remvoed
void Find_Bijection(formula_atribute* problem){

	list_s* literal;
	GS_mem* mem;
	list_s* node ;
	
	int count=0;

	bool* tried=NULL;
	tried = calloc(nr_variables+1, sizeof(bool));

	bool match;
	problem->removed_set->list = problem->removed_set->end;	
	mem 								= (GS_mem*)problem->removed_set->list->data;
	literal 							= (list_s*)mem->list;
	group_s* group 				= (group_s*)mem->group;	
	hash_t* h_table 				= problem->h_table2;
	
	int size_new_clause =0;
	
	bool save_ret =0 ;
	
	list_s* saved;
	
	while(1 ){
		count++;
		if(  check_table( (int64_t)group, h_table) ){
		
	//	printf(" here \n");
		
			if(problem->propagated[ abs(*(int*) literal->data) ]  == 0 && tried[abs(*(int*) literal->data)]==0 && problem->known[ abs(*(int*) literal->data) ]  == 0 ){
				////printf(" %i %i %i \n", abs(*(int*) literal->data),problem->assigned[ abs(*(int*) literal->data) ],problem->assignment[ abs(*(int*) literal->data) ] );
				//size_new_clause++;
				//tried[abs(*(int*) literal->data)]=1;
			}
				
			if ( problem->assignment[ abs(*(int*) literal->data) ] == 1 && problem->propagated[ abs(*(int*) literal->data) ]  == 0  && tried[abs(*(int*) literal->data)]==0 ){
				//this is the clause that needs reguessing
				// append the inital neg
				tried[abs(*(int*) literal->data)]=1;
				table_add( (int64_t)problem->init_neg[abs( *(int*)literal->data )], h_table);
				
			}

			if ( problem->assignment[ abs(*(int*) literal->data) ] == 0 && problem->propagated[ abs(*(int*) literal->data) ]  == 0 && problem->known [ abs(*(int*) literal->data)  ] == 0 && save_ret == false ){
				//this is the clause that needs reguessing
				
				table_add( (int64_t)problem->init_neg[abs( *(int*)literal->data )], h_table);
				free( tried );	
				return;
				saved = problem->removed_set->list;
				save_ret = true;
				tried[abs(*(int*) literal->data)]=1;
			}
				
			if ( tried[abs(*(int*) literal->data)]==0){
			
					// if this is propagated, continue on till the last  non- propagated variable
				///////printf(" this is propagated from %p \n", problem->prop_cause[abs( *(int*)literal->data )] );
				table_add( (int64_t)problem->prop_cause[abs( *(int*)literal->data )], h_table);
				tried[abs(*(int*) literal->data)]=1;
				
			}
				
		}
	
		if( problem->removed_set->list->previous == NULL){
			printf(" no previous %i \n", size_new_clause);
			exit(0);
			problem->removed_set->list = saved;
			free( tried );	
			return;
			
		}
		
		problem->removed_set->list = problem->removed_set->list->previous;
		mem 								= (GS_mem*)problem->removed_set->list->data;
		group 							= (group_s*)mem->group;	
		literal 							= (list_s*)mem->list;
		
	}
}

// this finds where a previous literal was remvoed
void back_jmp(formula_atribute* problem){

	list_s* literal;
	GS_mem* mem;
	list_s* node ;
	
	int count=0;

	bool* tried=NULL;
	tried = calloc(nr_variables+1, sizeof(bool));

	bool match;
	problem->removed_set->list = problem->removed_set->end;	
	mem 								= (GS_mem*)problem->removed_set->list->data;
	literal 							= (list_s*)mem->list;
	group_s* group 				= (group_s*)mem->group;	
	hash_t* h_table 				= problem->h_table2;
	
	int size_new_clause =0;
	
	bool save_ret =0 ;
	
	list_s* saved;
	
	while(1 ){
		count++;
		
		if(  check_table( (int64_t)group, h_table) ){
			if(problem->propagated[ abs(*(int*) literal->data) ]  == 0 && tried[abs(*(int*) literal->data)]==0 && problem->known[ abs(*(int*) literal->data) ]  == 0 ){
				printf(" %i %i %i \n", abs(*(int*) literal->data),problem->assigned[ abs(*(int*) literal->data) ],problem->assignment[ abs(*(int*) literal->data) ] );
				//size_new_clause++;
				//tried[abs(*(int*) literal->data)]=1;
			}
				
			if ( problem->assignment[ abs(*(int*) literal->data) ] == 1 && problem->propagated[ abs(*(int*) literal->data) ]  == 0  && tried[abs(*(int*) literal->data)]==0 ){
				//this is the clause that needs reguessing
				// append the inital neg
				tried[abs(*(int*) literal->data)]=1;
				table_add( (int64_t)problem->init_neg[abs( *(int*)literal->data )], h_table);
				
			}

			if ( problem->assignment[ abs(*(int*) literal->data) ] == 0 && problem->propagated[ abs(*(int*) literal->data) ]  == 0 && problem->known [ abs(*(int*) literal->data)  ] == 0 ){
				//this is the clause that needs reguessing
				
				//table_add( (int64_t)problem->init_neg[abs( *(int*)literal->data )], h_table);
				//free( tried );	
				return;
				if (save_ret == true){
					saved = problem->removed_set->list;
					return;
					}
				save_ret = true;
				saved = problem->removed_set->list;
				tried[abs(*(int*) literal->data)]=1;
			}
				
			if ( tried[abs(*(int*) literal->data)]==0){
			
					// if this is propagated, continue on till the last  non- propagated variable
				///////printf(" this is propagated from %p \n", problem->prop_cause[abs( *(int*)literal->data )] );
				table_add( (int64_t)problem->prop_cause[abs( *(int*)literal->data )], h_table);
				tried[abs(*(int*) literal->data)]=1;
				
			}
				
		}
	
		if( problem->removed_set->list->previous == NULL){
			printf(" bck_jmp no previous %i \n", size_new_clause);
			if ( saved != NULL){
				problem->removed_set->list = saved;
				return;	
			}
			exit(0);
			problem->removed_set->list = saved;
			free( tried );	
			return;
			
		}
		
		problem->removed_set->list = problem->removed_set->list->previous;
		mem 								= (GS_mem*)problem->removed_set->list->data;
		group 							= (group_s*)mem->group;	
		literal 							= (list_s*)mem->list;
		
	}
}

int propagate( int variable, formula_atribute* atribute){
	//////printf(" \n propagated %i assignemnt - %i \n ", variable, atribute->assignment[variable] );
	set_s** position						= atribute-> variable_position;
	position[ abs(variable) ]->list  = position[ abs(variable) ]->first;
	list_s* pos_list						= position[ abs(variable) ]->first;

	list_s* literal;
	group_s* group;
	GS_mem* mem;
	int* num;
	int ret;
	
	list_s* literal2;
	group_s* group2;
	GS_mem* mem2;

	while( pos_list !=NULL){		
	
		mem 			= (GS_mem*)pos_list->data;
		literal 		= (list_s*)mem->list;
		group 		= (group_s*)mem->group;	
		
		if ( group->end == NULL){
			printf(" null clause \n");
			exit(0);
		}
		
		if (literal== NULL){
			printf (" null literal \n");
		}
		
		// if there's only a single variable in this clause, and has been accounted for but is polar oposite, it's a fail
		if( (group)->first == (group)->end && group->first !=NULL ){
			
			//if it will become an empty clause
			if( atribute->assigned[ abs( *(int*)literal->data) ] == 1 
			&& (*(int*)literal->data > 0 && atribute->assignment[ abs( *(int*)literal->data) ] == 0
			||	 *(int*)literal->data < 0 && atribute->assignment[ abs( *(int*)literal->data) ] == 1) ){
				 
				 ////printf(" this will become an empty clause %i %i %p \n", *(int*)literal->data ,variable, group );
				if( atribute->first_removed[abs( *(int*)literal->data)] ==NULL){
					atribute->first_removed[abs( *(int*)literal->data)] = group;
				}
				
				//atribute->propagated_end[ abs(*(int*)literal->data) ] =  NULL;
				RemoveFromSet(&literal, &group, &atribute->removed_set );
				return -1;
			}
			
		}
		// remove the variable
		if(( *(int*)literal->data > 0 && atribute->assignment[ abs( *(int*)literal->data) ] == 0
		||	 *(int*)literal->data < 0 && atribute->assignment[ abs( *(int*)literal->data) ] == 1 ) && atribute->assigned[abs( *(int*)literal->data)] == 1){
			
			if( atribute->first_removed[abs( *(int*)literal->data)] == NULL)
				atribute->first_removed[abs( *(int*)literal->data)] = group;
			if ( RemoveFromSet(&literal, &group, &atribute->removed_set ) == 2 ){
				//atribute->propagated_end[ abs(*(int*)literal->data) ] =  NULL;
				printf(" neg \n");
				return 0;
			}
			//atribute->propagated_end[ abs(*(int*)literal->data) ] = group;
			atribute->propagated_end[ abs(*(int*)literal->data) ] =  atribute->removed_set->end;
			

			
			//printf("proped_end %i %i \n", *(int*)literal->data, *(int*)literal2->data);

		}
		
		
		// if the removalse causes a single literal clause, propagate
		if( (group)->first == (group)->end && group->first !=NULL  ){
		
			literal2 = group->first;

			//if it's not been assigned, assign it
			if(atribute->assigned[ abs( *(int*)literal2->data ) ] == 0){
			
				atribute->propagated[abs( *(int*)literal2->data )] = 1;	
				atribute->assigned[ abs( *(int*)literal2->data ) ] = 1 ;
				atribute->prop_cause[abs( *(int*)literal2->data )] = group;
				atribute->propagated_end[ abs( *(int*)literal2->data ) ] =  group->list;
				if( *(int*)literal2->data > 0 ){
					// assign true
					atribute->assignment[ abs( *(int*)literal2->data ) ] = 1;
				}else{
					//assign false
					atribute->assignment[ abs( *(int*)literal2->data ) ] = 0;
				}
				//num = malloc(sizeof(*num));
				//*num = abs(*(int*)literal2->data );
				//ExtendSet( num, atribute->to_prop);
				
				// propagate -- return 0 if mis match
				if( (ret=propagate( abs( *(int*)literal2->data ) , atribute)) != 1){
					// incorrect
					return(ret);
				}
				
				
			}
		}
		pos_list = pos_list->next;
	}

	return 1;
}

bool proc_propagate( int variable, formula_atribute* atribute){
int* num ;
num = malloc(sizeof(*num));
*num = abs(variable);
int ret;
ExtendSet( num, atribute->to_prop);
//atribute->to_prop->list = atribute->to_prop->first;
	while( atribute->to_prop->list!=NULL ){
		if ( ret= propagate( abs(*(int*)atribute->to_prop->first->data) , atribute)!=1 ){
		

			atribute->to_prop->list = atribute->to_prop->first;
			while(atribute->to_prop->first){
				
				atribute->assigned[abs( *(int*)atribute->to_prop->first->data )] = 0;	
				ReduceSet( atribute->to_prop->first, &atribute->to_prop);
				
			}
			
			return(ret);
		}
		
		ReduceSet( atribute->to_prop->first, &atribute->to_prop);
		
	}
	return(1);

}

unsigned int reset_variable( GS_mem* mem, int variable, formula_atribute* problem){
					
	list_s* l1		 = (list_s*)  mem->list;		
	group_s* group	 = (group_s*) mem->group;
						
	if(group == problem->first_removed[ abs(variable) ] && abs(*(int*)l1->data) == abs(variable) ){
		problem->assignment[ abs( *(int*)l1->data)  ]	= 0;
		problem->assigned[ abs( *(int*)l1->data) ]		= 0;
		problem->propagated[abs( *(int*)l1->data)] 		= 0;
		//problem->prop_cause [ abs( *(int*)l1->data) ] 	= NULL;
		problem->init_neg[abs(*(int*)l1->data)] 			= NULL;
		problem->first_removed [ abs(*(int*)l1->data)]  = NULL;
		ReinstateVariable( &problem->removed_set );
 		return 1 ;
 	}
	//printf("err: %i \n", *(int*)l1->data);
	if( problem->first_removed[abs( *(int*)l1->data)] == group ){
		problem->assignment[ abs( *(int*)l1->data)  ]	= 0;
		problem->assigned[ abs( *(int*)l1->data) ]		= 0;
		problem->propagated[abs( *(int*)l1->data)] 		= 0;
		//problem->prop_cause [ abs( *(int*)l1->data) ] 	= NULL;
		problem->init_neg[abs(*(int*)l1->data)] 			= NULL;
		problem->first_removed [ abs(*(int*)l1->data)]  = NULL;
	}
	ReinstateVariable( &problem->removed_set );
	
	return 0;					
}

int main(int argc, char* argv[]){


	int count =0;

	int* guessed;

	formula_atribute* problem	= malloc ( sizeof(*problem));
	problem->formula				= MakeGroup();
	problem->pre_set				= MakeSet();

	// imports cnf into local memory
	read_cnf_list(argv[1], problem);
	
	// exports the formula in a new numerical order has shown.
	char name2[20] = "DIMACS/num2.cnf";
	export_cnf(name2, problem->formula);
	//problem->formula->list 	= problem->formula->first;	

	// memory pre set
	bool changed_clause 			= 0;
	bool* assigned 				= problem->assigned;
	bool* assignment				= problem->assignment;
	bool* propagated				= problem->propagated;
	set_s* guessed_literals 	= MakeSet();;
	problem->removed_set			= MakeSet();
	problem->removed_list		= MakeSet();
	problem->to_prop				= MakeSet();
	
	problem->guessed_literals	= guessed_literals;
	problem->h_table				= hasht_create( 99839);
	problem->h_table2				= hasht_create( 99839);
	problem->h_table3				= hasht_create( 99839);

//	problem->h_table				= hasht_create( 307);
//	problem->h_table2				= hasht_create( 307);
//	problem->h_table3				= hasht_create( 307);

	// generates empy void ptrs to memory;
	problem->propagated_end = calloc( total_lit+1, sizeof(void*) );
	problem->known 			= calloc( nr_variables, sizeof(bool));
	problem->prop_cause		= calloc( total_lit+1, sizeof(void*) );
	problem->init_neg			= calloc( total_lit+1, sizeof(void*) ); 
	problem->first_removed	= calloc( total_lit+1, sizeof(void*) ); 
 
	for ( unsigned int i = 0; i <=total_lit; i++){
		problem->propagated_end[i]	= malloc( sizeof(void*));
		problem->propagated_end[i] = NULL;
	
		problem->prop_cause[i]	 	= malloc( sizeof(void*));
		problem->prop_cause[i]	 	= NULL;
		problem->init_neg[i]		 	= malloc( sizeof(void*));
		problem->init_neg[i]		 	= NULL;
		problem->first_removed[i]	= malloc( sizeof(void*));
		problem->first_removed[i]	= NULL;
	}

	//list of clause
	group_s* clause_container	= problem->formula;
	clause_container->list		= clause_container->first;
	list_s* clause					= clause_container->list;
	GS_mem* m1 ;
	list_s* l1 ;
	group_s* group2;
	group_s* variable_container ;

	list_s* address_rem;
	list_s* literal;
	group_s* group;
	
	GS_mem* mem;
	lut* conflict = malloc( sizeof(*conflict));
	
	set_s* implication = MakeSet();
	
	group_s* previous_temp;
	int temp_literal;
	bool temp_bool;
	int reset_too = 0;
	group_s* temp;
	
	// mem init end
	
	// propagate the pre-set variables
	problem->pre_set->list = problem->pre_set->first;
	while (problem->pre_set->list !=NULL){
		problem->known[ abs( *(int*)problem->pre_set->list->data)  ] 	= 1;
		
		if(propagate( abs( *(int*)problem->pre_set->list->data ), problem) != 1 ){
			printf(" not doable \n");
			exit(0);
		}
		
		problem->pre_set->list= problem->pre_set->list->next;
	}
	
	unsigned int* graphed = calloc( total_lit+1, sizeof(*graphed) );
	
	for( unsigned int var =1 ; var<= total_lit; var++){
		graphed[var]=0;
	}
	
	unsigned int layer = 0;
	unsigned int limit = 8000;

	set_s* var_list	 = MakeSet();
	set_s* sorted		 = MakeSet();
	set_s* out			 = MakeSet();
	set_s* pre 				= MakeSet();
	list_s* loop;
	int* n1; 
	unsigned int var;
	
	int temp_var;
	
	
	problem->pre_set->list = problem->pre_set->first;
	loop = problem->pre_set->first;
	
	//pre = problem->pre_set;
	
	//loop= pre->first;
	
	// organizes the variables that are related to each other n levels deep
	//for( unsigned int var =1 ; var<= total_lit; var++){
	/*
	while(loop){
		graph( abs(*(int*)loop->data), pre, graphed, layer, limit, problem);
		loop = loop->next;
	}
	*/
	//SortByConnection( &pre, problem);
	/*
	
	
	while(pre->list){
			graph( abs(*(int*)pre->list->data), out, graphed, layer, limit, problem);
		//InsertNumToSet( abs( *(int*)pre->list->data) , out );
		//loop = loop->next;
		pre->list= pre->list->next;
		//	printf(" var %i \n", var );
	}
	pre= out;
	pre->list = pre->first;
	*/
	
	for( int var =1 ; var<= total_lit; var++){
	//while(loop){
		//graph( abs(var), pre, graphed, layer, limit, problem);
		//InsertNumToSet( var, pre );
		//loop = loop->next;
	//	printf(" var %i \n", var );
	}
	
		for( int var =total_lit ; var>= 1; var--){
	//while(loop){
		graph( abs(var), pre, graphed, layer, limit, problem);
		//InsertNumToSet( var, pre );
		//loop = loop->next;
	//	printf(" var %i \n", var );
	}
	
	var_list = pre;

	// this memorizes the position of the variable for backjumping	
	var_list->list 	= var_list->first;
	list_s* add_temp 	= var_list->list;
	
	int saved_lit;
	
	int res;
	add_temp = var_list->first;
	list_s** address_recall = calloc ( total_lit+1 ,sizeof(*address_recall));
	while( add_temp ){
		address_recall[ abs( *(int*) add_temp->data ) ] = add_temp ;
		add_temp = add_temp->next;
	}

	// main algo go go
	
	list_s* var_loop = var_list->first;
	int variable;
	
	int var_sat;
	
	while( var_loop ){
	
			variable = abs(*(int*) var_loop->data);
			printf(" variable %i %i \n", variable,problem->assigned[ abs( variable ) ]);
			// been assigned?
			if( problem->assigned[ abs( variable ) ] == 0 ){
				problem->assigned[ abs( variable ) ] 		= 1;
				problem->assignment[ abs( variable ) ] 	= 0; 

				//propagate
				while( ( res = propagate( abs( variable ), problem) ) !=1 ){
					printf(" neg prop variable %i \n", variable);
					m1 		 = (GS_mem*) problem->removed_set->end->data;
					l1 		 = (list_s*)m1->list;		
					group 	 = (group_s*) m1->group;	
					saved_lit = abs( *(int*)l1->data );
					temp_bool = problem->assignment[ abs( variable ) ] ;
					
					// this saves the address fo the clause of the conflict
					if( temp_bool == 0 ){
					
						//empty the hash table for new collision
						hash_t_empty( &problem->h_table );
						table_add( (int64_t)group, problem->h_table);
						
						hash_t_empty( &problem->h_table2 );
						table_add( (int64_t)group, problem->h_table2);
						if( check_table( (int64_t)group, problem->h_table3)==0 )
							table_add( (int64_t)group, problem->h_table3);

						if (res == -1){
//							
//							temp_bool=0;
							m1		 = (GS_mem*) problem->removed_set->end->data;
							l1		 = (list_s*)m1->list;
							group	 = (group_s*) m1->group;
							conflict->first = group;
							
							table_add( (int64_t)problem->prop_cause[ abs( *(int*)l1->data ) ] , problem->h_table2);
							
							LearnClause(problem,problem->h_table2,1, abs(*(int*)l1->data) );
							back_jmp( problem);
							
							m1		 = (GS_mem*) problem->removed_set->list->data;
							l1		 = (list_s*)m1->list;	
							group	 = (group_s*) m1->group;
							
							printf (" pre learn %i \n ", variable);
							
						//	variable = abs(*(int*)l1->data);
							
						//	temp_bool=0;
							
							printf(" variable post learn %i \n", variable);
						}else{
							table_add( (int64_t)problem->prop_cause[ abs( *(int*)l1->data ) ] , problem->h_table2);
							Find_Partial_Bijection( problem->first_removed[ abs( variable ) ] , problem,  problem->h_table2);
						}
						
						
						
					}else{
						m1		 = (GS_mem*) problem->removed_set->end->data;
						group	 = (group_s*) m1->group;
						conflict->second = group;
						
						//empty the hash table for new collision
						hash_t_empty( &problem->h_table );
						table_add( (int64_t)group, problem->h_table);
						if ( problem->prop_cause [ abs( *(int*)l1->data ) ]!=NULL)
							table_add( (int64_t)problem->prop_cause [ abs( variable) ] , problem->h_table);
						
						table_add( (int64_t)group, problem->h_table2);
						if( check_table( (int64_t)group, problem->h_table3)==0 )
							table_add( (int64_t)group, problem->h_table3);

						
						if ( problem->prop_cause [ abs( *(int*)l1->data ) ]!=NULL){
							table_add( (int64_t)problem->prop_cause [ abs( variable)  ] , problem->h_table2);
							if( check_table( (int64_t)problem->prop_cause [ abs( variable)  ] , problem->h_table3)==0 )
								table_add( (int64_t)problem->prop_cause[ abs(  variable) ] , problem->h_table3);
						}

							
							Find_Partial_Bijection( problem->first_removed[ abs( variable ) ] , problem,  problem->h_table2);
						
					}
					
					m1		 = (GS_mem*) problem->removed_set->end->data;
					l1		 = (list_s*)m1->list;		
					group	 = (group_s*) m1->group;

					if (l1==NULL){
						printf("empty variable \n");
						exit(0);
					}

					// reset to the point this was guessed
					while(1 ){	
						
						if( reset_variable( m1, variable, problem) == 1 )
							break;
						
						if(problem->removed_set->end == NULL){
							printf("empty removed set \n ");
							exit(0);
					}
						m1		 = (GS_mem*) problem->removed_set->end->data;
						l1		 = (list_s*)m1->list;		
						group	 = (group_s*) m1->group;
					}
						
										
					if(problem->removed_set->end == NULL){
						printf("empty removed set \n ");
						exit(0);
					}
					
					problem->first_removed[abs( variable )] = NULL;
					// if the variable under test was guessed 0, flip the guess
					if ( temp_bool == 0 ){
					printf(" cont \n");
					
						problem->init_neg[abs(variable )] 			= conflict->first;
						problem->assignment[ abs( variable ) ] 	= 1;
						problem->assigned[ abs( variable ) ]		= 1;
						//variable= temp_var;
						
						
						continue;
					}else{
						//halt();	
						if(problem->removed_set->end == NULL){
							printf("sec empty removed set \n ");
							exit(0);
						}
						//m1		 = (GS_mem*) problem->removed_set->end->data;
						//l1		 = (list_s*)m1->list;
						
						//	Find_Bijection(problem);
						//m1		 = (GS_mem*) problem->removed_set->list->data;
						//l1		 = (list_s*)m1->list;	
						//
						//problem->removed_set->list = problem->removed_set->end;
						
						//LearnClause(problem,problem->h_table3,0,abs(*(int*)l1->data));
						//back_jmp( problem);
						//problem->removed_set->list = problem->removed_set->end;
						
						
						// this prints the matchin clauses;
						/*
						group = conflict->first;
						printf("\n var %i\n \n", variable);
						for( int i = 0 ; i< group->var_list_size; i++){
						
							printf(" %i \n", group->var_list[i]);
						}
						printf("\n");
						
						group = conflict->second;
						
						for( int i = 0 ; i< group ->var_list_size; i++){
						
							printf(" %i \n", group->var_list[i]);
						}
						printf("\n");
						*/
						//halt();
						
						m1		 = (GS_mem*) problem->removed_set->end->data;
						l1		 = (list_s*)m1->list;	
						group	 = (group_s*) m1->group;
	
						//this just moves to the previous guessed variable assigned 0;
						while( 1 ){
							//printf (" back %i \n", *(int*) l1->data);
						 	if ( problem->assignment[ abs(*(int*) l1->data) ] == 0 && problem->propagated[ abs(*(int*) l1->data) ]  == 0 && problem->known [ abs(*(int*) l1->data)  ] == 0 ){
								break;
							}		
							if( problem->removed_set->list->previous == NULL){
								printf(" removed set null \n");
								exit(0);
							}
							problem->removed_set->list = problem->removed_set->list->previous;
							mem 		= (GS_mem*) problem->removed_set->list->data;
							l1 		= (list_s*)mem->list;	
							group 	= (group_s*)mem->group;			
						
						}
						mem 		= (GS_mem*) problem->removed_set->list->data;
						l1 		= (list_s*)mem->list;	
						group 	= (group_s*)mem->group;	
						
						reset_too = abs(*(int*)l1->data);
						
						// reset as it's about to run down another branch
						mem 			= (GS_mem*)problem->removed_set->list->data;
						literal 		= (list_s*)mem->list;
						group 		= (group_s*)mem->group;	
						//		reset_too = abs(*(int*)l1->data);
						while( 1 ){

						 	if( reset_variable( mem, reset_too, problem) == 1 )
								break;
							
							if( problem->removed_set->end == NULL){
								printf(" removed set null \n");
								exit(0);
							}
								
							mem 		= (GS_mem*) problem->removed_set->end->data;
							literal  = (list_s*)mem->list;	
							group 	= (group_s*)mem->group;			
						
						}
						
						//	found a negate correlation,
						// this variable needs to be changed because of this clause
						problem->init_neg[abs(reset_too)] = temp;
						assignment[ abs( reset_too) ]		= 1;
						assigned[ abs( reset_too)  ]		= 1;	
						list_s* temp = var_loop;
						//printf(" init addr recall %p \n",address_recall[ abs(variable) ]->next);
						
						//printf(" addr recall %p \n",address_recall[ abs(variable) ]->next );
						printf(" %i reset_too %i \n",variable,reset_too);
						variable = abs(reset_too);
//						MoveSet( var_loop, address_recall[ abs(reset_too) ], var_list);
						var_loop = address_recall[ abs(reset_too) ];
						//halt(0);
						//printf(" reset too %i \n", reset_too);
						continue;
					}
				}
			}
			
			if (var_loop == var_loop->next){
				printf("error \n");
				exit(0);				
			}
				
			printf(" passed \n ");
			var_loop = var_loop->next;
		}
	
	//DelSet( &guessed_literals);
	
	// exports the completed set of variables
	problem->formula->list = problem->formula->first;	
	char name[10] = "fini.cnf";
	export_cnf(name, problem->formula);
		
	free( problem->assignment);
	free( problem->assigned);
	free(problem);
	printf(" finished %i \n",learn_count);
	return 0;


}
