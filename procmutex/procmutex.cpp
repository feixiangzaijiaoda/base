//==============================================================================
//                      Procmutex.cpp
//
//begin   : 2016-08-09
//describe: 该文件是进程间同步类的源文件，支持跨平台Windows/Linux
//          Windows下进程间同步采用互斥锁，Linux下进程间同步采用信号量
//==============================================================================

#include "procmutex.h"

#ifdef WIN32
//==============================================================================               
//                    CProcMutex
//					        多进程同步锁
//==============================================================================
CProcMutex::CProcMutex(const char* name)
{
    memset(m_cMutexName, 0 ,sizeof(m_cMutexName));
    int min = strlen(name)>(sizeof(m_cMutexName)-1)?(sizeof(m_cMutexName)-1):strlen(name);
    strncpy(m_cMutexName, name, min);
		m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, m_cMutexName);
		if (!m_hMutex)
		{
			m_hMutex = CreateMutex(NULL, FALSE, m_cMutexName);
		}
}

CProcMutex::~CProcMutex()
{
    CloseHandle(m_hMutex);
}

void CProcMutex::Lock()
{
    DWORD nRet = WaitForSingleObject(m_hMutex, INFINITE);
}

void CProcMutex::UnLock()
{
     ReleaseMutex(m_hMutex);
}

#endif

//==============================================================================               
//                    CProcMutex
//					  多进程同步锁
//==============================================================================
#ifdef linux
CProcMutex::CProcMutex(const char* name)
	:m_msTimeWait(TIMEWAIT)
{
   memset(m_cMutexName, 0 ,sizeof(m_cMutexName));
   int min = strlen(name)>(sizeof(m_cMutexName)-1)?(sizeof(m_cMutexName)-1):strlen(name);
   strncpy(m_cMutexName, name, min);
   m_pSem = sem_open(name, O_RDWR | O_CREAT, 0644, 1);
}

CProcMutex::~CProcMutex()
{
  int ret = sem_close(m_pSem);
  if (0 != ret)
  {
       printf("sem_close error %d\n", ret);
  }
  sem_unlink(m_cMutexName);
}

bool CProcMutex::SetTimeWait(int msTimeWait)
{
	if(msTimeWait < 0)
	{
		return false;
	}
	m_msTimeWait = msTimeWait;
	return true;
}

int CProcMutex::SemWaitTime(int mswait)
{
	timespec ts;                           
  clock_gettime(CLOCK_REALTIME, &ts );    //获取当前时间  
  ts.tv_sec += (mswait / 1000 );        //加上等待时间的秒数  
  ts.tv_nsec += ( mswait % 1000 ) * 1000; //加上等待时间纳秒数  
  int nRet = 0;                            
  while( ((nRet=sem_timedwait(m_pSem, &ts ))!=0) && (errno ==  EINTR) )   //等待信号量，errno==EINTR屏蔽其他信号事件引起的等待中断  
  {
  	;
  }
  return nRet;     
}

void CProcMutex::Lock()
{
	if(-1 == SemWaitTime(m_msTimeWait))
	{
		return;
	}
	
}

void CProcMutex::UnLock()
{
   sem_post(m_pSem);
}

#endif
