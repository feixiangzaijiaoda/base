//==============================================================================
//                      lock.h
//
//begin   : 2016-07-08
//describe: ���ļ���Ϊ��ʵ�ֶ��߳�ͬ�����࣬Windows/linuxƽ̨������
//==============================================================================
#ifndef ZDHR_LOCK_H_
#define ZDHR_LOCK_H_

#ifdef WIN32
	#include <windows.h>
#else
	#include <pthread.h>
#endif


//==============================================================================                
//                    CMutex
//									�����������                    
//==============================================================================

class CMutex
{
public:
	//����һ�������������
	CMutex()
	{
#ifdef WIN32
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
#else
	pthread_mutex_init(&m_Mutex, NULL);
#endif
	}  
	   
	//���ٻ�������ͷ���Դ
	~CMutex()
	{
#ifdef WIN32
	CloseHandle(m_hMutex);
#else
	pthread_mutex_destroy(&m_Mutex);
#endif
	}
	
public:
	//ȷ��ӵ�л��������̶߳Ա�������Դ�Ķ��Է���
	void Lock() const 
	{
#ifdef WIN32
	DWORD d = WaitForSingleObject(m_hMutex, INFINITE);
#else
	pthread_mutex_lock(const_cast<pthread_mutex_t*>(&m_Mutex));
#endif
	}
	
	//�ͷŵ�ǰ�߳�ӵ�еĻ��������ʹ�����߳̿���ӵ�л�����󣬶Ա�������Դ���з���
	void UnLock() const
	{
#ifdef WIN32
	ReleaseMutex(m_hMutex);
#else
	pthread_mutex_unlock(const_cast<pthread_mutex_t*>(&m_Mutex));
#endif
	}	
	
#ifndef WIN32
	//����Condition���ã��Ͻ��û��������
	pthread_mutex_t* GetPthreadMutex()
	{
		return &m_Mutex;
	}
#endif
	
private:
#ifdef WIN32
	HANDLE m_hMutex; //�������
#else
	pthread_mutex_t m_Mutex;
#endif
};


//==============================================================================               
//                    CLock
//                 ���߳�ͬ����
//==============================================================================
class CLock
{
public:
	explicit CLock(const CMutex& lock)
		:m_pLock(&lock)
		{
			m_pLock->Lock();
		}
		
	~CLock()
	{
		m_pLock->UnLock();
	}
			
private:
	const CMutex* m_pLock;	
};


#endif

