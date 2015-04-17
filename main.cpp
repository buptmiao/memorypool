#include "memorypool.h"
#include <iostream>
#include <queue>
#include <time.h>

#define TEST_MEM

using namespace std;

class Test{
public: 
	Test():val(0),cnt(0){}
	~Test(){}
	
#ifdef TEST_MEM
	static void *operator new(size_t){
		if(M_pool == NULL)
			return NULL;
		return M_pool->Alloc();
	}
	static void operator delete(void *p,size_t){
		M_pool->Free(p);
	}
	static MemoryPool *M_pool;
#endif

private:
	int val;
	int cnt;
};

#ifdef TEST_MEM
	MemoryPool * Test::M_pool = new MemoryPool(sizeof(Test),1024,512,false);
#endif

int main()
{
	queue<Test *> q;
	time_t start;
	time(&start);
#ifdef TEST_MEM
	for(int i = 0;i < 10000;i++){
		for(int j = 0;j < 10000;j++){
			q.push( new Test() );
		}
		while(!q.empty()){
			delete q.front() ;
			q.pop();
		}
	}
	cout<<"using memorypool..."<<endl; //26s
#else
	for(int i = 0;i < 10000;i++){
		for(int j = 0;j < 10000;j++){
			q.push( new Test() );
		}
		while(!q.empty()){
			delete q.front();
			q.pop();
		}
	}
	cout<<"using standard new..."<<endl;//91s
#endif
	time_t end;
	time(&end);

	cout <<"start time is: " << start << endl;
	cout <<"end   time is: " << end   << endl;
	cout <<"total time is: " << end-start<<" seconds"<<endl;

	return 0;
}
