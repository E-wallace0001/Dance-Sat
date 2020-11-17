
#ifndef CNF_LLIST_h_
#define CNF_LLIST_h_

#include "group.h"

typedef struct formula_atribute{
	group_s* formula ;
	set_s** variable_position;
	bool* assigned;
	bool* assignment;
	bool* propagated;
	
	bool* known;
	
	set_s* removed_set;
	set_s* guessed_literals;
	set_s* removed_list;
	set_s* pre_set;

	void** propagated_end;	
	void** prop_cause;
	void** init_neg;
	void** first_removed;
	
	set_s* to_prop;
	
	struct hash_t* h_table;
	
}formula_atribute;

int nr_variables, nr_clauses;

void read_cnf_list(char*,formula_atribute* );
void export_cnf( char* , set_s*);

size_t total_lit;

#endif
