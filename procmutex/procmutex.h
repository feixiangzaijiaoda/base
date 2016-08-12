//==============================================================================
//                      Procmutex.h
//
//begin   : 2016-08-09
//describe: 该文件是进程间同步类的头文件，支持跨平台Windows/Linux
//          Windows下进程间同步采用互斥锁，Linux下进程间同步采用信号量
//==============================================================================
#ifndef __PROC_MUTEX_H__
#define __PROC_MUTEX_H__

#ifdef WIN32
#include <Windows.h>
#endif

#ifdef linux
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

const int TIMEWAIT = 1000*2;
#endif

class CProcMutex
{
public:
    /* 默认创建匿名的互斥 */
    CProcMutex(const char* name = NULL);
    ~CProcMutex();

    void Lock();
    void UnLock();
    
#ifdef linux
	 //设置最大等待时间
	 bool SetTimeWait(int msTimeWait);
private:
	//SemWaitTime是为了防止进程意外退出造成的死锁，可以通过设置最大等待时间，超过等待时间后，进程会自动获取该锁的所有权
	int SemWaitTime(int mswait = TIMEWAIT);
#endif
	
private:
#ifdef WIN32
    HANDLE m_hMutex;
#endif

#ifdef linux
    sem_t* m_pSem;
    int m_msTimeWait;
#endif
    char m_cMutexName[100];
};

#endif
