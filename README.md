# memorypool
一个C++实现的内存池

使用方法：

1、为使用内存池的类对象重载new/delete操作符，静态成员M_pool为指向内存池对象的指针
使用时需要定义一个内存池类对象，并用其地址初始化M_pool。例如：

Test::M_pool = new MemoryPool(sizeof(Test));

Test是使用内存池的类，M_pool是该类定义的静态成员，sizeof(Test)实参是初始化内存池中
一个内存单元的大小。

可以如下编写测试类：

class Test{
public: 
	Test():val(0),cnt(0){}
	~Test(){}
	
static void *operator new(size_t){
	if(M_pool == NULL)
		return NULL;
	return M_pool->Alloc();
}
static void operator delete(void *p,size_t){
	M_pool->Free(p);
}
static MemoryPool *M_pool;

private:
	int val;
	int cnt;
};
MemoryPool * Test::M_pool = new MemoryPool(sizeof(Test));

编写完成后就可以正常使用new和delete操作符来从内存池中分配和释放Test对象,

Test *p = new Test();

delete p;


通过简单的程序测试，得到两组数据，可以看出使用内存池和不使用内存池性能上的差异

使用内存池：
using memorypool...
start time is: 1429239289
end   time is: 1429239304
total time is: 15 seconds

使用标准操作符：
using standard new...
start time is: 1429239423
end   time is: 1429239463
total time is: 40 seconds

测试环境：windows 7

具体测试程序见main.cpp
