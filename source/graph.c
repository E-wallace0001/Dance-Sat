#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "headers/cnf_llist.h"
#include "headers/group.h"



void graph( int root_var, set_s* list,unsigned int* graphed,unsigned int layer, unsigned int limit, formula_atribute* atribute){
	
	set_s** position						= atribute-> variable_position;
	position[ abs(root_var) ]->list  = position[ abs(root_var) ]->first;
	list_s* clause							= position[ abs(root_var) ]->first;
	
	list_s* literal;
	group_s* group;
	GS_mem* mem;
	unsigned int variable;

	if ( clause == NULL)
		return ; 

	while( 1 ){
	
		mem 			= (GS_mem*)clause->data;
		group			= (group_s*)mem->group;
		literal		= (list_s*)group->first;

		if (literal == NULL)
			exit(0);
		while (1){
		
			variable = abs( *(int*)literal->data);
			
			if (graphed[abs(variable)] ==0){
		
				//ExtendSet( abs(variable), list); 
				graphed[ abs(variable) ] =1;
				InsertNumToSet( abs(variable), list );
				
				
				
			}
			if ( literal->next == NULL)
				break;
			literal = literal->next;
		}
			if( clause->next == NULL)
				break;
		clause = clause->next;
	}
}


void SortByConnection( set_s** new_list, formula_atribute* atribute ){
int* tried;
set_s* list = *new_list;
tried = calloc(total_lit+1, sizeof(*tried));

int count =0;

	set_s** array;
	array = MakeSetArray( 401);
	set_s** position						= atribute-> variable_position;
	set_s* var_list;
	list_s* variable;
	for ( int i =1; i <= total_lit; i++){
		var_list = position[i];
		InsertNumToSet( i, array [ CountGroupSet( var_list) ] );
	}
	for ( int i =400; i >= 1; i--){
		if( array[i] == NULL )
			continue;
		
		variable = array[i]->first;
		while(variable){
			if( tried[ abs( *(int*)variable->data)] ==0 ){
				tried[ abs( *(int*)variable->data)] =1;
				InsertNumToSet( abs( *(int*)variable->data), list );
				
			}
			variable= variable->next;
			
		}
		
	}
	

}

