//==============================================================================
//                      Procmutex.cpp
//
//begin   : 2016-08-09
//describe: ���ļ��ǽ��̼�ͬ�����Դ�ļ���֧�ֿ�ƽ̨Windows/Linux
//          Windows�½��̼�ͬ�����û�������Linux�½��̼�ͬ�������ź���
//==============================================================================

#include "procmutex.h"

#ifdef WIN32
//==============================================================================               
//                    CProcMutex
//					        �����ͬ����
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
//					  �����ͬ����
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
  clock_gettime(CLOCK_REALTIME, &ts );    //��ȡ��ǰʱ��  
  ts.tv_sec += (mswait / 1000 );        //���ϵȴ�ʱ�������  
  ts.tv_nsec += ( mswait % 1000 ) * 1000; //���ϵȴ�ʱ��������  
  int nRet = 0;                            
  while( ((nRet=sem_timedwait(m_pSem, &ts ))!=0) && (errno ==  EINTR) )   //�ȴ��ź�����errno==EINTR���������ź��¼�����ĵȴ��ж�  
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
