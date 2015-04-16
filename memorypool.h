
/*	
 *基本概念：内存池，内存块，内存单元
 *	内存池:   MemoryPool  它可以是内存池信息 和 多个内存块链接起来的整体
 *	内存块:	  MemoryBlock 进程堆中的一段连续内存，适用new表达式分配,包含多个内存单元
 *	内存单元: Alloc和Free操作的单位，存在内存块中
 *内存池预先分配一个初始内存块，当一个内存块满时，重新分配一个内存块，用链表连起来
 *当分配内存时从内存池中找到空闲单元并返回,释放内存时，根据指针找到所属内存块,得到偏移地址
 *
 *内存块中的内存单元是链接起来的，这个技巧可以充分利用内存。每一个空闲内存单元的前两个字节
 *表示下一个空闲内存单元的编号，初始在构造函数中初始化，当Alloc时，根据first找到空闲单元
 *读出空闲单元中的前两个字节内容赋值给first，返回内存单元，这时候前两个字节的作用失效。当
 *Free时，计算出相应的存储单元编号，将first的值写入该单元的前两个字节，并给first重新赋值为
 *该单元编号，实现链接各个存储单元。
 */

#include <stdio.h>

#define MEMPOOL_ALIGNMENT 4
#define DEFAULE_UNITSIZE  4
#define DEFAULT_INITSIZE  4096
#define DEFAULT_GROWSIZE  1024

typedef unsigned short  ushort;
typedef unsigned long   ulong;
typedef unsigned int   	uint;

/* 内存块类
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

/* 内存池类
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

