#include "procmutex.h"
#include <stdio.h>
#include <iostream>

using namespace std;

#ifdef WIN32
int main()
{
	CProcMutex mymutex("hello");

	mymutex.Lock();
	printf("I am 1111!\n");
	Sleep(1000*10);
	mymutex.UnLock();

	return 0;
}
#else
int main()
{
	CProcMutex mymutex("processmutex");
	mymutex.Lock();
	printf("i am test1\n");
	sleep(30);
	printf("hello\n");
	mymutex.UnLock();

	return 0;
	
}

#endif