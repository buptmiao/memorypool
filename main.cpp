#include "memorypool.h"
#include <iostream>
#include <queue>
#include <time.h>
//#define TEST_MEM

using namespace std;
class Test{
public:
	Test():val(0),cnt(0){}
//static void *operator new(size_t);
//static void operator delete(void *p,size_t);
private:
	int val;
	int cnt;

};


int main()
{
	queue<Test *> q;
	time_t start;
	time(&start);
#ifdef TEST_MEM
	MemoryPool M_pool(sizeof(Test),1024,256,false);
	for(int i = 0;i < 100000;i++){
		for(int j = 0;j < 10000;j++){
			q.push( (Test *)M_pool.Alloc() );
		}
		while(!q.empty()){
			M_pool.Free(q.front());
			q.pop();
		}
	}
	cout<<"using memorypool..."<<endl; //26s
#else
	for(int i = 0;i < 100000;i++){
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
	cout <<"total time is: " << end-start<<endl;

	return 0;
}
