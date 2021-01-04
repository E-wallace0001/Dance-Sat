#ifndef GRAPH_H
#define GRAPH_H

#include "dsat.h"

void graph( int ,group_s* ,unsigned int*,unsigned int , unsigned int , formula_atribute*  );

void SortByConnection( set_s** , formula_atribute*  );
	
void BFSClause( lut* , formula_atribute*   );

#endif
