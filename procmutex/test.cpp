#include "procmutex.h"
#include <stdio.h>
#include <iostream>

using namespace std;



#ifdef WIN32
int main()
{
	CProcMutex mymutex("hello");
	for (int i=0; i<200; ++i)
	{
		mymutex.Lock();
		printf("%d\n", i);
		mymutex.UnLock();
		Sleep(1000);
		
	}

}

#else
int main()
{
	CProcMutex mymutex("processmutex");
	mymutex.SetTimeWait(1000*5);
	for(int i=0; i<30; ++i)
	{
		mymutex.Lock();
		printf("I am first process%d!\n", i);
		sleep(1);
		mymutex.UnLock();
		sleep(1);
		
		
	}
	return 0;
	              
}


#endif