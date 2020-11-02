#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "headers/cnf_llist.h"
#include "headers/group.h"



void graph( int root_var, set_s* list,unsigned int graphed[],unsigned int layer, unsigned int limit, formula_atribute* atribute){
	
	set_s** position						= atribute-> variable_position;
	position[ abs(root_var) ]->list  = position[ abs(root_var) ]->first;
	list_s* clause							= position[ abs(root_var) ]->first;
	
	list_s* literal;
	group_s* group;
	GS_mem* mem;
	unsigned int variable;

	while( clause ){
	
		mem 			= (GS_mem*)clause->data;
		group			= (group_s*)mem->group;
		literal		= (list_s*)group->first;


		while (literal){
		
			variable = abs( *(int*)literal->data);
			
			if (graphed[variable] ==0){
		
				//ExtendSet( abs(variable), list); 
				graphed[ abs(variable) ] =1;
				InsertNumToSet( abs(variable), list );
				
				if (layer != limit){
					layer++;
					graph( abs(variable), list, graphed, layer, limit, atribute );
					layer --;
				}
				
			}
			literal = literal->next;
		}
		clause = clause->next;
	}
}


void SortByConnection( set_s* list, formula_atribute* atribute ){
int* tried;

tried = calloc(total_lit+1, sizeof(*tried));
	set_s** array;
	array = MakeSetArray( 200);
	set_s** position						= atribute-> variable_position;
	set_s* var_list;
	list_s* variable;
	for ( int i =1; i <= total_lit; i++){
		var_list = position[i];
		InsertNumToSet( i, array [ CountGroupSet( var_list) ] );
	}
	int* num;
	for ( int i =200; i >= 1; i--){
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

