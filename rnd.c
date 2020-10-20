#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "cnf_llist.h"
#include <stdio.h>
#include "group.h"
#include "rnd.h"

 int nr_clauses, nr_variables;
int main(int argc, char* argv[]){


	group_s* formula = MakeGroup();
	group_s* variable_position = MakeGroup();
	
	//group_s* formula =NULL; 
	// imports cnf into local memory
	
	read_cnf_list(argv[1], formula);
 printf(" nr_clause %i %i  \n", nr_clauses, nr_variables);
	//bool set[31]={0};



	
	return 0;


}
