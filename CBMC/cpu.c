#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
int main(){

	int a, b ;

	int a1, b1 ;
	a1=7;
	b1=3;
	a= 11;
	b=7;

	typedef unsigned short inst_t; // 16bit isntructions

	inst_t program[32]  ;
	// first 4 bits 
	uint8_t reg[16] = {a,b,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uint8_t reg2[16] = {a1,b1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	unsigned int size = 8	;
	
	inst_t ip=0; //instruction pointer	
	int op;
	uint8_t dest ;
	uint8_t src1;
	uint8_t src2;
	
	

	for(int pc = 0 ; pc< size; pc++){
		ip = program[pc];
		op = ( ip >> 12)&0xf; //opcode
		dest = ( ip >> 8)&0xf;//destination
		src1 = ( ip >> 4)&0xf;//soruce1}
		src2 = ( ip >> 0)&0xf;//source2
		switch ( op ) {
			case 0xC:
				reg[dest] = reg[src1] ^ reg[src2];
				reg2[dest] = reg2[src1] ^ reg2[src2];
				break;
			case 0XA:
				reg[dest] = reg[src1] & reg[src2];
				reg2[dest] = reg2[src1] & reg2[src2];
				break;
			case 0XD:
				reg[dest] = reg[src1] >> reg[src2];
				reg2[dest] = reg2[src1] >> reg2[src2];
				break;
			
			case 0XE:
				reg[dest] = reg[src1] << reg[src2];
				reg2[dest] = reg2[src1] << reg2[src2];
				break;
		}
	}
		
	
	//__CPROVER_assume( reg[0] == b*a);
	//__CPROVER_assume( reg2[0] == b1*a1);
	//__CPROVER_assume(src1<2);
	//__CPROVER_assume(src2<2);

	bool found = reg[0] == b*a ;
	bool found2 = reg2[0] == b1*a1 ; 
	assert(!found);
	assert(!found2);


	return 0;
}

// p cnf 45878 246157
