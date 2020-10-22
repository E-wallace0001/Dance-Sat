#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include "group.h"
#include "cnf_llist.h"
#include "h_table.h"
	//stores temporary literals
	
	int f_variable_count = 0;
	int nr_variables, nr_clauses;
	size_t total_lit;
	
	
int count_variables( char* argv){

	int count=0;
	hash_t* h_table = hasht_create( 65537);
	
	FILE *fp2;
	fp2 = fopen(argv, "r");
	if(!fp2){
		printf("Cannot find %s \n", argv);
		exit(0);
	}
	
	fscanf(fp2, "p cnf %i	 %u\n", &nr_variables, &nr_clauses);
	int literal=0;
	int clause =0;
	while (1){
		if ( clause == nr_clauses){
			break;
		}
		
		if (fscanf(fp2, "%d", &literal) != 1){
			printf( " cl %i \n", clause);
			printf("error: expected literal\n");
			exit(EXIT_FAILURE);
			//break;//return count;
		}
		
		if ( literal != 0) {
			
			if ( check_table( abs(literal), h_table) == 1){
				continue;
			}else{
				//printf (" %i:  %d \n ", literal, count );
				
				table_add( abs(literal), h_table);
				count++;
			}
		
		}else{
			clause++;
		}
		
	}
	
	fclose(fp2);
	hash_t_destroy( & h_table);
	
return count;

}
	
void read_cnf_list(char* argv,formula_atribute* problem){
// group_s* formula, set_s*** variable_position){
	group_s* formula = problem->formula;
	set_s** variable_position;


	set_s* s1;

	FILE *fp;
	fp = fopen(argv, "r");

	if(!fp){printf("Cannot find %s \n", argv);exit(0);}
	
	fscanf(fp, "p cnf %i	 %u\n", &nr_variables, &nr_clauses);
	
	int** variable;
	int f_clause_count =0;
	
	
	// needs to be the size of largest literal as most software skips numbers and takes the largest literal as the literal count - waste of mem!
	
	variable = calloc(nr_variables, sizeof(int*));
	
	for( int i =0; i <= nr_variables; i++){
	
		variable[i] = calloc(2,sizeof(int));
		variable[i][0]=0;
		variable[i][1]=0;
		
		
		
		if( variable[i]==NULL)
			exit(0);
	}
	variable[0][0]=0;
	
	total_lit						= count_variables(argv)+1;
	printf(" total_lit %lu \n", total_lit);
	
	problem->assigned				= calloc(nr_variables, sizeof(bool));
	problem->assignment			= calloc(nr_variables, sizeof(bool));
	problem->propagated			= calloc(nr_variables, sizeof(bool));
	variable_position				= MakeSetArray(nr_variables); 
	problem->variable_position	= variable_position;
	assert(variable_position  != NULL);
	
	int tmp[400]={0};
	int b=0;
	int cl=1;
	//scans file
	int literal;
	int clause;
	int* var_this;
	GS_mem* group_set;
	while (1){
		if (fscanf(fp, "%d", &literal) != 1){
			printf("error: expected literal\n");
			exit(EXIT_FAILURE);
		}
		if (literal!=0){
			//stores which literal
			tmp[b]= literal;
			b++;			
			continue;
		}
		if(literal == 'c') return;

		//at the end of each line
		if (literal == 0){
			
			s1 = (set_s*)formula->end->data;
			
			//this is where the number of clauseses is stored
			for (int unload=0;unload<b;unload++){
				//convert variable to progessive number
				if(variable[abs(tmp[unload])][1]==0){
					f_variable_count++;
					variable[0][0]++;
					
					variable[abs(tmp[unload])][0]=variable[0][0];
					variable[abs(tmp[unload])][1]=1;
				}
				
				if(tmp[unload]>0){
					var_this =  malloc( sizeof(*var_this)) ;
					*var_this = variable[abs(tmp[unload])][0] ;
					ExtendSet( var_this , s1);
				}

				if(tmp[unload]<0){
					var_this =  malloc( sizeof(int)) ;
					*var_this = -variable[abs(tmp[unload])][0] ;
					ExtendSet( var_this , s1);
				}
				group_set			= malloc(sizeof(*group_set));
				
				group_set->list	= s1->end;
				
				group_set->group 	= s1;
				
				ExtendSet( group_set, variable_position[ abs(variable[abs(tmp[unload])][0] )] ) ; 
				
				// this is a set variable, mark it
				if( b == 1){
					problem->assigned[ variable[abs(tmp[unload])][0] ] 		= 1;
					
					if(tmp[unload]>0){
						problem->assignment[ variable[abs(tmp[unload])][0] ] 	= 1; 
					}

					if(tmp[unload]<0){
						problem->assignment[ variable[abs(tmp[unload])][0] ] 	= 0; 
					}
					ExtendSet( var_this, problem->pre_set);
				}
			}
			b=0;
		}
		//break;
		ExtendGroup( formula );
		cl++;
		f_clause_count++;
		//end of the documents
		if(cl>nr_clauses){
			break;
		}

	}
	
	//fclose(fp);
	

}

void export_cnf( char* file, set_s* formula){

	set_s* clause = (set_s*)formula->list->data;
	FILE* fp2;
	fp2=fopen(file,"w");
	while( 1 ){
		
		list_s* variable = clause->list;
		if( clause == NULL)
			break;
		while( variable != NULL){
				fprintf (fp2,"%i ", ( *(int*)variable->data) );
				
			variable = variable->next;		
		}
		if( formula->list->next == NULL) 
			break;
		fprintf(fp2,"0\n");

		formula->list= formula->list->next;
		clause = (set_s*)formula->list->data;
	}
	fclose(fp2);	

}


