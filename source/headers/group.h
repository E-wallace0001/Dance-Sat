
#ifndef GROUP_H
#define GROUP_H

typedef struct set_s{

	struct list_s* list;
	struct list_s* first;
	struct list_s* end;

}set_s;


typedef struct list_s{
	void* data;
	struct list_s* next;
	struct list_s* previous;
} list_s;

typedef  set_s group_s;

typedef struct GS_mem{
	group_s* group;
	list_s* list;
} GS_mem;


set_s* MakeSet();

void ExtendSet(void*, set_s*);
void InsertNumToSet( int, set_s*);

int ReduceSet(list_s*, set_s**);
int GroupReduceSet(list_s* list_t, set_s** set);
int RemoveFromSet(list_s** , set_s** , set_s** );
void ReinstateVariable(set_s** );
void Del_GS_Mem( set_s** set);

void DelSet(set_s**);
void RemoveSetElements(set_s**);

group_s* MakeGroup();

void ExtendGroup(group_s*);

void ReduceGroup(group_s**, set_s**);

void DeleteGroup(group_s**);

set_s* NextSet(group_s*);
set_s* PreviousSet(group_s*);
void ResetSet(set_s**);


set_s** MakeSetArray( size_t );
void FreeSetArray ( set_s** , size_t);

void CopyGroupSet(set_s*, set_s*);

int CountGroupSet(set_s*);

void SetPostInsert( void*  , set_s** );


#endif
