//==============================================================================
//                      lock.h
//
//begin   : 2016-07-08
//describe: 该文件是为了实现多线程同步锁类，Windows/linux平台均适用
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
//									互斥对象锁类                    
//==============================================================================

class CMutex
{
public:
	//创建一个匿名互斥对象
	CMutex()
	{
#ifdef WIN32
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
#else
	pthread_mutex_init(&m_Mutex, NULL);
#endif
	}  
	   
	//销毁互斥对象，释放资源
	~CMutex()
	{
#ifdef WIN32
	CloseHandle(m_hMutex);
#else
	pthread_mutex_destroy(&m_Mutex);
#endif
	}
	
public:
	//确保拥有互斥对象的线程对被保护资源的独自访问
	void Lock() const 
	{
#ifdef WIN32
	DWORD d = WaitForSingleObject(m_hMutex, INFINITE);
#else
	pthread_mutex_lock(const_cast<pthread_mutex_t*>(&m_Mutex));
#endif
	}
	
	//释放当前线程拥有的互斥对象，以使其它线程可以拥有互斥对象，对被保护资源进行访问
	void UnLock() const
	{
#ifdef WIN32
	ReleaseMutex(m_hMutex);
#else
	pthread_mutex_unlock(const_cast<pthread_mutex_t*>(&m_Mutex));
#endif
	}	
	
#ifndef WIN32
	//仅供Condition调用，严禁用户代码调用
	pthread_mutex_t* GetPthreadMutex()
	{
		return &m_Mutex;
	}
#endif
	
private:
#ifdef WIN32
	HANDLE m_hMutex; //互斥对象
#else
	pthread_mutex_t m_Mutex;
#endif
};


//==============================================================================               
//                    CLock
//                 多线程同步锁
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

