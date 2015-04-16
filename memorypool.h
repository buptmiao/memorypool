/*
 * 
 * 
 * 
 * 
 * 
 */
#include <stdio.h>
#define MEMPOOL_ALIGNMENT 4
#define DEFAULE_UNITSIZE  4
#define DEFAULT_INITSIZE  4096
#define DEFAULT_GROWSIZE  1024

typedef unsigned short  ushort;
typedef unsigned long   ulong;
typedef unsigned int   	uint;

/* 
 * 
 */
struct MemoryBlock{
		ushort size;
		ushort numfree;
		ushort first;
		MemoryBlock *next;
		char   Data[1];
		
		static void *operator new(size_t,ushort,ushort);
		static void operator delete(void *p,size_t);
		
		MemoryBlock(ushort n,ushort unitsize);
		~MemoryBlock(){}
};

/*
 * 
 */

class MemoryPool{
public:
	MemoryPool(ushort _unitsize=DEFAULE_UNITSIZE,
			   ushort _initsize=DEFAULT_INITSIZE,
			   ushort _growsize=DEFAULT_GROWSIZE,
			   bool   _prealloc = false);
	~MemoryPool();
	
	void *Alloc();
	void Free(void *p);

private:
	MemoryBlock *pMem;
	ushort	unitsize;
	ushort	initsize;
	ushort	growsize;
	bool	prealloc;
};
