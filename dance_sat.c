#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "cnf_llist.h"
#include <stdio.h>
#include "group.h"
#include "rnd.h"

typedef struct lut{

	void* first;
	void* second;

} lut;


void halt(){
	while(1){
	char input;
	scanf("%c", &input);
	 if(getchar()!=0){ break;}
	}
}

// this finds initial implication before choice
group_s* Find_Partial_Bijection(group_s* end, group_s* variable_container,formula_atribute* problem, group_s* group1, lut* conflict){

	list_s* literal;
	GS_mem* mem;
	list_s* node ;
	set_s* graph;
	graph = MakeSet();
	
	int count=0;

	bool* tried=NULL;
	tried = calloc(nr_variables+1, sizeof(bool));

	ExtendSet( conflict->first, graph);
	
	bool match;
							
	mem 								= (GS_mem*)problem->removed_set->end->data;
	literal 							= (list_s*)mem->list;
	group_s* group 				= (group_s*)mem->group;	
	
	//printf(" biject %p %p \n", variable_container, group);
	while(1 ){
	
		count++;

		//printf ("%i %p %i assign %i prop %i assig %i cause %p init_neg %p first rem %p known %i \n",count, group, *(int*) literal->data, problem->assignment[ abs(*(int*) literal->data) ] , problem->propagated[ abs(*(int*) literal->data) ], problem->assigned[abs(*(int*) literal->data)], problem->prop_cause[ abs( *(int*) literal->data ) ], problem->init_neg[ abs( *(int*) literal->data ) ] , problem->first_removed[abs( *(int*)literal->data)],problem->known[abs( *(int*)literal->data)] );
		
		// if the clauses come across the start point, return
		if ( end == group ){
			free( tried );
			return (graph);
		}
		
		node		= graph->first;

		while( node ){
			////printf( "node %p \n", node->data);
			match = node->data == group;
			if( match ){
				
				if ( problem->assignment[ abs(*(int*) literal->data) ] == 1 && problem->propagated[ abs(*(int*) literal->data) ]  == 0 && tried[abs(*(int*) literal->data)]==0  ){
					
					// append the inital neg
					
					//printf(" reguess another \n");
					ExtendSet( problem->init_neg[abs( *(int*)literal->data )], graph);
					tried[abs(*(int*) literal->data)]=1;
					//halt();
				}
				
				if ( problem->propagated[ abs( *(int*) literal->data) ] == 1  && tried[abs(*(int*) literal->data)]==0 ){
					// if this is propagated, continue on till the last  non- propagated variable
					//printf(" this is propagated from %p \n", problem->prop_cause[abs( *(int*)literal->data )] );
					ExtendSet( problem->prop_cause[abs( *(int*)literal->data )], graph);
					tried[abs(*(int*) literal->data)]=1;
				}
				
			}
			node = node->next;	
		}

		if( problem->removed_set->list->previous == NULL){
			printf(" no previous \n");
			exit(0);
		}
		
		problem->removed_set->list = problem->removed_set->list->previous;
		mem 								= (GS_mem*)problem->removed_set->list->data;
		group 							= (group_s*)mem->group;	
		literal 							= (list_s*)mem->list;
		
	}
}

// this finds where a previous literal was remvoed
void Find_Bijection(group_s* variable_container,formula_atribute* problem, group_s* group1, lut* conflict){

	list_s* literal;
	GS_mem* mem;
	list_s* node ;
	set_s* graph;
	graph = MakeSet();
	
	int count=0;

	bool* tried=NULL;
	tried = calloc(nr_variables+1, sizeof(bool));

	ExtendSet( conflict->first, graph);
	ExtendSet( conflict->second, graph);
	
	bool match;
							
	mem 								= (GS_mem*)problem->removed_set->end->data;
	literal 							= (list_s*)mem->list;
	group_s* group 				= (group_s*)mem->group;	
	

	
	//printf(" biject %p %p \n", variable_container, group);
	while(1 ){
	
		count++;

		//printf ("%i %p %i assign %i prop %i assig %i cause %p init_neg %p first rem %p known %i \n",count, group, *(int*) literal->data, problem->assignment[ abs(*(int*) literal->data) ] , problem->propagated[ abs(*(int*) literal->data) ], problem->assigned[abs(*(int*) literal->data)], problem->prop_cause[ abs( *(int*) literal->data ) ], problem->init_neg[ abs( *(int*) literal->data ) ] , problem->first_removed[abs( *(int*)literal->data)],problem->known[abs( *(int*)literal->data)] );
		
		node		= graph->first;

		while( node ){
			////printf( "node %p \n", node->data);
			match = node->data == group;
			if( match ){
				
				if ( problem->assignment[ abs(*(int*) literal->data) ] == 1 && problem->propagated[ abs(*(int*) literal->data) ]  == 0  ){
					//this is the clause that needs reguessing
					
					// append the inital neg
					//printf(" reguess another \n");
					ExtendSet( problem->init_neg[abs( *(int*)literal->data )], graph);
					//halt();
				}

				if ( problem->assignment[ abs(*(int*) literal->data) ] == 0 && problem->propagated[ abs(*(int*) literal->data) ]  == 0 && problem->known [ abs(*(int*) literal->data)  ] == 0 ){
					//this is the clause that needs reguessing
					RemoveSetElements( &graph );
					free( tried );
					return;
				}
				
				if ( problem->propagated[ abs( *(int*) literal->data) ] == 1  && tried[abs(*(int*) literal->data)]==0){
					// if this is propagated, continue on till the last  non- propagated variable
					//printf(" this is propagated from %p \n", problem->prop_cause[abs( *(int*)literal->data )] );
					ExtendSet( problem->prop_cause[abs( *(int*)literal->data )], graph);
					tried[abs(*(int*) literal->data)]=1;
				}
				
			}
			node = node->next;	
		}

		if( problem->removed_set->list->previous == NULL){
			printf(" no previous \n");
			exit(0);
		}
		
		problem->removed_set->list = problem->removed_set->list->previous;
		mem 								= (GS_mem*)problem->removed_set->list->data;
		group 							= (group_s*)mem->group;	
		literal 							= (list_s*)mem->list;
		
	}
}

void Reset_Guesses_Till(formula_atribute* problem){
	list_s* literal;
	group_s* group;
	GS_mem* mem;

	mem 			= (GS_mem*)problem->removed_set->list->data;
	literal 		= (list_s*)mem->list;
	group 		= (group_s*)mem->group;	
						
	while( 1 ){
		
		problem->assignment[ abs( *(int*)problem->guessed_literals->end->data ) ]		= 0;
		problem->assigned[ abs( *(int*)problem->guessed_literals->end->data ) ]			= 0;
		problem->propagated[abs( *(int*)problem->guessed_literals->end->data )] 		= 0;
		
		problem->prop_cause [ abs( *(int*)problem->guessed_literals->end->data ) ] 	= NULL;
		problem->init_neg[abs( *(int*)problem->guessed_literals->end->data) ] 			= NULL;
		problem->first_removed [ abs( *(int*)problem->guessed_literals->end->data ) ] = NULL;
		
		GroupReduceSet( problem->guessed_literals->end, &problem->guessed_literals);

		if ( problem->guessed_literals->end==NULL){
			break;
		}
		
		//printf("reset guessed %i \n", *(int*)problem->guessed_literals->end->data );
		
		if (  abs( *(int*)problem->guessed_literals->end->data )  == abs( *(int*)literal->data) ){
			break;	
		}
					
	}
	
	
	
}


bool propagate( size_t variable, formula_atribute* atribute){
	//printf(" \n propagated %lu assignemnt - %i \n ", variable, atribute->assignment[variable] );

	set_s** position				= atribute-> variable_position;
	position[ variable ]->list = position[ variable ]->first;
	list_s* pos_list				= position[ variable ]->list;

	list_s* literal;
	group_s* group;
	GS_mem* mem;
	

	
	//printf("1\n");
	while( pos_list !=NULL){		
	
		mem 			= (GS_mem*)pos_list->data;
		literal 		= (list_s*)mem->list;
		group 		= (group_s*)mem->group;	
		
		// if there's only a signele variable in this clause
		if( (group)->first == (group)->end ){
		
			//if it will become an empty clause
			if( *(int*)literal->data > 0 && atribute->assignment[ abs( *(int*)literal->data) ] == 0
			||	 *(int*)literal->data < 0 && atribute->assignment[ abs( *(int*)literal->data) ] == 1){
				 //printf(" this will become an empty clause %i %p \n", *(int*)literal->data , group );
				 
				if( atribute->first_removed[abs( *(int*)literal->data)] ==NULL)
					atribute->first_removed[abs( *(int*)literal->data)] = group;
					
				 RemoveFromSet(&literal, &group, &atribute->removed_set );
				 return 0;
			}
			
			if ( pos_list->next==NULL)
				break;
			pos_list = pos_list->next;
			continue;
			
		}
		
		if( *(int*)literal->data > 0 && atribute->assignment[ abs( *(int*)literal->data) ] == 0
		||	 *(int*)literal->data < 0 && atribute->assignment[ abs( *(int*)literal->data) ] == 1){
			//printf(" removed %i %p %p \n", *(int*)literal->data, group, 	atribute->first_removed[abs( *(int*)literal->data)] );

			if( atribute->first_removed[abs( *(int*)literal->data)] == NULL)
				atribute->first_removed[abs( *(int*)literal->data)] = group;

			if ( RemoveFromSet(&literal, &group, &atribute->removed_set ) == 2 ){
				//printf(" ?\n ");
				return 0;
				//exit(0);
			}

		}
		
		if( (group)->first == (group)->end  ){
		
			literal = group->first;
			
			//if it's not been assigned, assign it
			if(atribute->assigned[ abs( *(int*)literal->data ) ] == 0){
				//	//printf(" this is implied data\n");
				////printf(" this will produce a single %i  \n", *(int*)literal->data);

				atribute->propagated[abs( *(int*)literal->data )] = 1;	
				atribute->assigned[ abs( *(int*)literal->data ) ] = 1 ;
				
				atribute->prop_cause[abs( *(int*)literal->data )] = group;
				
				
				//printf(" assigned %i \n", *(int*)literal->data);
				
				if( *(int*)literal->data > 0 ){
				// 	//printf( "assigned 1 \n");
					atribute->assignment[ abs( *(int*)literal->data ) ] = 1;
				}else{
				//	//printf( "assigned 0 \n");
					atribute->assignment[ abs( *(int*)literal->data ) ] = 0;
				}
				
				
				// propagate -- return 0 if mis match
				if( propagate( abs( *(int*)literal->data ) , atribute) == 0){
					//printf(" nope \n");
					//ReinstateVariable( &atribute->removed_set );
					//atribute->assigned[ abs( *(int*)literal->data ) ] = 0 ;
					//GroupReduceSet( atribute->guessed_literals->end, &atribute->guessed_literals);
					return(0);
				}
			
			}
		}
		
		pos_list = pos_list->next;

	}
	//printf(" returned \n");

	return 1;
}


int main(int argc, char* argv[]){


	int count =0;

	int* guessed;

	formula_atribute* problem	= malloc ( sizeof(*problem));
	
	problem->formula				= MakeGroup();
	
	
	problem->pre_set				= MakeSet();

	// imports cnf into local memory
	read_cnf_list(argv[1], problem);
	
	bool changed_clause 			= 0;
	bool* assigned 				= problem->assigned;
	bool* assignment				= problem->assignment;
	bool* propagated				= problem->propagated;
	set_s* guessed_literals 	= MakeSet();;
	problem->removed_set			= MakeSet();
	problem->removed_list		= MakeSet();
	problem->guessed_literals	= guessed_literals;
	
	problem->known = calloc( nr_variables, sizeof(bool));
	
	
	//printf(" var coun %i \n", nr_variables);
	
	problem->prop_cause= calloc(	nr_variables, sizeof(void*) ); 
	for ( int i = 0; i <nr_variables; i++){
		problem->prop_cause[i] = malloc( sizeof(void*));
		problem->prop_cause[i]= NULL;
	}

	problem->init_neg= calloc(	nr_variables, sizeof(void*) ); 
	for ( int i = 0; i <nr_variables; i++){
		problem->init_neg[i] = malloc( sizeof(void*));
		problem->init_neg[i]= NULL;
	}
	problem->first_removed= calloc(	nr_variables, sizeof(void*) ); 
	for ( int i = 0; i <nr_variables; i++){
		problem->first_removed[i] = malloc( sizeof(void*));
		problem->first_removed[i]= NULL;
	}

	//list of clause
	group_s* clause_container	= problem->formula;
	clause_container->list		= clause_container->first;
	list_s* clause					= clause_container->list;
	GS_mem* m1 ;
	list_s* l1 ;
	group_s* group2;
	group_s* variable_container ;
	list_s* variable ;
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
	
	// propagate the set variables
	problem->pre_set->list = problem->pre_set->first;
	while (problem->pre_set->list !=NULL){
	//printf("pre_Set -%i \n", *(int*)problem->pre_set->list->data);
		problem->assigned[ abs( *(int*)problem->pre_set->list->data)  ] 	= 1;
		
		problem->known [ abs( *(int*)problem->pre_set->list->data)  ] 	= 1;
		
		if (*(int*)problem->pre_set->list->data > 0){
			problem->assignment[ abs( *(int*)problem->pre_set->list->data)  ] 	= 1; 
		}else{
			problem->assignment[ abs( *(int*)problem->pre_set->list->data)  ] 	= 0; 
		}
		if(!propagate( abs( *(int*)problem->pre_set->list->data ), problem) ){
			//printf(" not doable \n");
			exit(0);
		}
		problem->pre_set->list= problem->pre_set->list->next;
		
		
	}
//halt();
	//printf(" ***************************************************************\n\n\n");
	for ( int variable =1; variable<= nr_variables; variable++){
			//printf(" var->dat %i %i \n", variable, assigned[ variable] );
			// been assigned?
			if( assigned[ abs( variable ) ] == 0 ){
			
				// assign
				//printf(" assigning this %i \n", variable);
				assigned[ abs( variable ) ] 		= 1;
				assignment[ abs( variable ) ] 	= 0; 
				guessed													= malloc( sizeof( *guessed) );
				*guessed													= variable;

				ExtendSet( guessed , guessed_literals);
				
				//printf(" \n init guessed %i  \n", variable );
				
				

				//propagate
				while( !propagate( abs( variable ), problem)  ){
					
					if( problem->guessed_literals->end ==NULL){
						//printf ("No data \n");
						exit(0);
					}
					
					temp_bool = assignment[ abs( variable ) ] ;
					
					m1 = (GS_mem*) problem->removed_set->end->data;
					l1 = (list_s*)m1->list;		
					group = (group_s*) m1->group;	
					
					//printf(" pre reset %p \n", m1->group);
					//printf(" assignment %i %i %p \n", abs( variable), temp_bool, previous_temp );
					
					// this saves the address fo the clause of the conflict
					if( temp_bool == 0 ){
						ResetSet( implication);
						ExtendSet( m1->group, implication);
						Find_Partial_Bijection(group_s* end, group_s* variable_container,formula_atribute* problem, group_s* group1, lut* conflict){
					
						conflict->first = m1->group;
						
					}else{
						ExtendSet( m1->group, implication);
					}

					// reset to the point this was guessed
					while(1 ){
						////printf(" group %p first rem %p l1 %i var %i \n", group, problem->first_removed[ variable] , abs(*(int*)l1->data), variable); 
						////printf(" reinstated %i at %p  %i \n", *(int*)l1->data, group, problem->known[*(int*)l1->data] );
						if( problem->first_removed[abs( *(int*)l1->data)] == group ){
							problem->assignment[ abs( *(int*)l1->data ) ]		= 0;
							problem->assigned[ abs( *(int*)l1->data ) ]			= 0;
							problem->propagated[abs( *(int*)l1->data )] 			= 0;
							problem->first_removed[ abs( *(int*)l1->data) ]=NULL;
						}
						if(group == problem->first_removed[ variable] && abs(*(int*)l1->data) == variable ){
							ReinstateVariable( &problem->removed_set );
					 		break;
					 	}
						ReinstateVariable( &problem->removed_set );
						
						if(problem->removed_set->end == NULL)
							exit(0);;
					
						m1		 = (GS_mem*) problem->removed_set->end->data;
						l1		 = (list_s*)m1->list;		
						group	 = (group_s*) m1->group;
						
						if(group == problem->first_removed[ variable] && abs(*(int*)l1->data) == variable ){
							ReinstateVariable( &problem->removed_set );
					 		break;
					 	}
					}
					problem->first_removed[ variable ]=NULL;

					// if the variable under test was guessed 0
					// flip the guess
					if ( temp_bool == 0 ){
						problem->init_neg[abs(variable )] = m1->group;
						//printf(" reasigned %i \n", abs( variable));
						assignment[ abs( variable ) ]  = 1;
						assigned[ abs( variable ) ]	 = 1;
						//halt();
						continue;
					}else{
						
						set_s** position							= problem-> variable_position;
						position[ abs(variable)  ]->list 	= position[ abs(variable) ]->first;
						list_s* pos_list							= position[ abs(variable) ]->list;
						mem 											= (GS_mem*) pos_list->data;
						literal	 									= mem->list;
						group											= mem->group;
						temp											= m1->group;
						
						while( pos_list !=NULL){
							//printf( "address %p - %i  \n", group, *(int*) literal->data);
							pos_list	= pos_list->next;	
							if(pos_list==NULL)
								break;
							mem		= (GS_mem*)pos_list->data;
							literal	= (list_s*) mem->list;
							group		= (group_s*)mem->group;			
						}
						
						
						problem->removed_set->list = problem->removed_set->end;
						mem 								= (GS_mem*)problem->removed_set->list->data;
						literal 							= (list_s*)mem->list;
						group 							= (group_s*)mem->group;	
						
						problem->guessed_literals->list = problem->guessed_literals->end;
						
						//printf(" %i %i %i %i\n", problem->propagated[ abs(variable)], variable, assigned[ abs(variable) ], assignment[ abs(variable) ] );
						
						//find a previous remove
						Find_Bijection(variable_container,problem, group, conflict);

						mem 			= (GS_mem*)problem->removed_set->list->data;
						literal 		= (list_s*)mem->list;
						group 		= (group_s*)mem->group;	
						reset_too 	= *(int*) literal->data;
						
						Reset_Guesses_Till(problem);
						
						if ( problem->guessed_literals->end ==NULL){
							//printf("exhausted all the guesses \n");
							exit(0);
						}

						//printf(" comp \n");
						
						// reset as it's about to run down another branch
						mem 		= (GS_mem*) problem->removed_set->end->data;
						literal  = (list_s*)mem->list;	
						group 	= (group_s*)mem->group;						
						while( 1 ){
							////printf(" %p %p %i %i  \n", group, problem->first_removed[abs( reset_too )], *(int*) literal->data, reset_too );
						
							if(group == problem->first_removed[abs( *(int*)literal->data)] ){
								problem->assignment[ abs( *(int*)literal->data ) ]		= 0;
								problem->assigned[ abs( *(int*)literal->data ) ]		= 0;
								problem->propagated[abs( *(int*)literal->data )] 		= 0;
								problem->prop_cause [ abs( *(int*)literal->data ) ] 	= NULL;
								problem->init_neg[ abs(*(int*)literal->data) ] 			= NULL;
								problem->first_removed [ abs( *(int*)literal->data ) ] = NULL;
								ReinstateVariable( &problem->removed_set );
						 	}

								
							mem 		= (GS_mem*) problem->removed_set->end->data;
							literal  = (list_s*)mem->list;	
							group 	= (group_s*)mem->group;			
							ReinstateVariable( &problem->removed_set );				
							if ( *(int*)literal->data == reset_too && group == problem->first_removed[abs( *(int*)literal->data)] ){
							
								mem = (GS_mem*) problem->removed_set->end->data;
								literal = (list_s*)mem->list;	
								group = (group_s*)mem->group;
								ReinstateVariable( &problem->removed_set );
								//printf(" end %p %p %i %i  \n", group, problem->first_removed[abs( reset_too )], *(int*) literal->data, reset_too);
								//halt();
								break;
							}		
						}

						if( problem->removed_set->end == NULL){
							//printf(" null end \n");
							exit(0);
						}
						
						//	found a negate correlation,
						// this variable needs to be changed because of this clause
						problem->init_neg[abs(reset_too)] = temp;
						
						assignment[ abs( reset_too) ]		= 1;
						assigned[ abs( reset_too)  ]		= 1;
						*(int*)literal->data = reset_too;
						*guessed = reset_too;
						ExtendSet( guessed , guessed_literals);
						//printf("reassignement %i %i  \n",  reset_too , *(int*)literal->data	);
						//exit(0);
						//clause 							= group->list;
						variable = *(int*)literal->data	;
						//halt();
						continue;

					}
				}
			}

	}
	
	//output results

	// scratch end	
	//DeleteGroup(&problem->formula);
	//FreeSetArray(problem->variable_position,total_lit);
	//DelSet( &problem->removed_set);
	//DelSet( &guessed_literals);
	problem->formula->list = problem->formula->first;	
	char name[10] = "fini.cnf";
	export_cnf(name, problem->formula);
		
	free( problem->assignment);
	free( problem->assigned);
	free(problem);
	//printf(" finished \n");
	return 0;


}
