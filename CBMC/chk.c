#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
int main(){

	long long z, a ,b;
	long long  prime =7	;
	bool found=0;
	//x=5;
	
	__CPROVER_assume( a>1 && a < prime && b > 1 && b<prime  );
	//	__CPROVER_assume( a!=2 && a !=5 && b != 2 && b!=5  );
//	assert(a!=2);
//	assert(b!=2_
	//__CPROVER_assume( a!=prime );
	
	z = a * b;
	//bool found;
	found = ( z==prime);
	assert ( !found );

	return 0;
}

// p cnf 45878 246157
