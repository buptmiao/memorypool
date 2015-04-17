#include <iostream>
#include "memorypool.h"

/* 功能：内存块类构造函数
 * 参数：n 存储单元数量，unitsize每个存储单元大小
 * 返回值：无
 */
MemoryBlock::MemoryBlock(ushort n,ushort unitsize)
	:size(n * unitsize),numfree(n-1),first(1),next(NULL)
{
	char *pdata = Data;
	/*把内存池中的每一个存储单元的前两个字节初始化为下一个存储单元的编号*/
	/*最后一个存储单元不设置值，故i从1~n*/
	for(ushort i = 1;i < n;i++){
		*(ushort *)pdata = i;
		pdata += unitsize;
	}
}


/* 功能：重载MemoryBlock的new操作符
 * 参数：_size 存储单元数量，_unitsize每个存储单元大小
 * 返回值：分配内存的指针
 */
static void * MemoryBlock::operator new(size_t , ushort _size , ushort _unitsize){
	return ::operator new(sizeof(MemoryBlock) + _size * _unitsize);
}

/* 功能：重载MemoryBlock的delete操作符
 * 参数：将释放的指针
 * 返回值：无
 */
static void MemoryBlock::operator delete(void *p,size_t){
	::operator delete (p);
}


/* 功能：内存池构造函数
 * 参数：_unitsize单元大小，_initsize初始化分配的大小，_growsize扩展大小,_prealloc预分配标志
 * 返回值：无
 */
MemoryPool::MemoryPool(ushort _unitsize,ushort _initsize,ushort _growsize,bool _prealloc)
	:pMem(NULL),unitsize(_unitsize),initsize(_initsize),growsize(_growsize),prealloc(_prealloc)
{
	/*对unitsize进行调整*/
	if(_unitsize > 4)
		unitsize = (_unitsize + (MEMPOOL_ALIGNMENT)-1) &~ (MEMPOOL_ALIGNMENT-1);
	else if(_unitsize <= 2)
		unitsize = 2;
	else
		unitsize = 4;
	/*预分配内存*/
	if(_prealloc){
		Alloc();
		pMem->first = 0;
		pMem->numfree = initsize;
	}
}

/* 功能：内存池析构函数，清楚内存池所分配的所有内存块
 * 参数：n 无
 * 返回值：无
 */
MemoryPool::~MemoryPool(){
	while(pMem){
		MemoryBlock *tmp = pMem;
		pMem = pMem->next;
		delete tmp;
	}
}

/* 功能：内存池分配函数，分配固定大小的内存
 * 参数：无
 * 返回值：分配内存的指针
 */
void * MemoryPool::Alloc(){
	if(!pMem){//尚未分配内存
		pMem = new(initsize,unitsize) MemoryBlock(initsize,unitsize);
		if(!pMem)
			return NULL;
		return pMem->Data;
	}
	MemoryBlock *ptmp = pMem;
	/*ptmp 非空并且无空闲单元，则跳到下一块*/
	while(ptmp && !ptmp->numfree)
		ptmp = ptmp->next;
		
	if(ptmp){ //找到内存块
		char *pfree = ptmp->Data + ptmp->first*unitsize;
		ptmp->first = *((ushort *)pfree);//更新first为pfree中的前2字节，即下一个空闲块
		ptmp->numfree--; //空闲单元--
		return (void *)pfree;
		
	}else{  //之前几块都满，需要重新开一块MemoryBlock 
		if(!growsize)
			return NULL;
		ptmp = new(growsize,unitsize) MemoryBlock(growsize,unitsize);
		if(!ptmp)
			return NULL;
		/*把新开的存储空间放在表头，提高下一次效率*/
		ptmp->next = pMem;
		pMem = ptmp;
		return (void *)ptmp->Data;
	}
}


/* 功能：内存池释放内存函数
 * 参数：待释放内存单元的指针
 * 返回值：无
 */
void MemoryPool::Free(void *pfree){
	MemoryBlock *ptmp = pMem;
	MemoryBlock *prev = pMem; //记录ptmp前一个内存块的地址，在修改表结构的时候会用到
	while(ptmp && ((ulong)pfree < (ulong)ptmp || (ulong)pfree >= (ulong)(ptmp->Data) + ptmp->size)){
		prev = ptmp;
		ptmp = ptmp->next;
	}
	if(!ptmp)
		return;
	ptmp->numfree++;//空闲单元数++
	*(ushort *)pfree = ptmp->first;
	ptmp->first = ((ulong)pfree - (ulong)ptmp->Data)/unitsize;//根据指针偏移，计算first的位置
	
	/*需要清除,清理完要保持表的结构*/
	if(ptmp->numfree * unitsize == ptmp->size){
		if(ptmp == pMem){  //在表头
			pMem = pMem->next;
			delete ptmp;
		}
		else{
			prev->next = ptmp->next;
			delete ptmp;
		}
	}else{
		if(ptmp != pMem){ //不是头结点需要把当前节点放到开头，否则不需要动
			prev->next = ptmp->next;
			ptmp->next = pMem;
			pMem = ptmp;
		}
	}
}


