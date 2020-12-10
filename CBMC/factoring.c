#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
int main(){

	uint64_t z, a ,b;
	uint64_t prime = 99839	;
	bool found=0;
	//x=5;
	
	__CPROVER_assume( a>1 && a < prime && b > 1 && b<prime  );
		
//	assert(a!=2);
//	assert(b!=2_
	//__CPROVER_assume( a!=prime );
	
	z = a * b;
	//bool found;
	found = ( z==prime);
	//__CPROVER_assume( found!=prime  );
	assert ( !found );

	return 0;
}

// p cnf 45878 246157
