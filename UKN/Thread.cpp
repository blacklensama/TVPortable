//
//  Thread.cpp
//  Project Unknown
//
//  Created by Robert Bu on 12/13/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#include "UKN/Thread.h"

#if defined(UKN_OS_WINDOWS)
#include <process.h>

#elif defined(UKN_OS_FAMILY_UNIX)
#include <unistd.h>
#include <map>

#endif

#include "UKN/Logger.h"
#include "UKN/Singleton.h"
#include "UKN/Common.h"

namespace ukn {
    
    namespace thread {
        
        Mutex::Mutex() {
#ifdef UKN_OS_WINDOWS 
            mLocked = false;
            InitializeCriticalSectionAndSpinCount(&cs, 4000);
#elif defined(UKN_OS_FAMILY_UNIX)
            pthread_mutex_init(&mutex, NULL);
#endif
        }
            
        Mutex::~Mutex() {
#ifdef UKN_OS_WINDOWS
            DeleteCriticalSection(&cs);
#elif defined(UKN_OS_FAMILY_UNIX)
            pthread_mutex_destroy(&mutex);
#endif
        }
            
        inline void Mutex::lock() {
#ifdef UKN_OS_WINDOWS
            EnterCriticalSection(&cs);
            while(mLocked) Sleep(1000);
            mLocked = true;
#elif defined(UKN_OS_FAMILY_UNIX)
            pthread_mutex_lock(&mutex);
#endif
        }
            
        inline bool Mutex::tryLock() {
#ifdef UKN_OS_WINDOWS
            bool ret = (TryEnterCriticalSection(&cs) ? true: false);
            if(ret && mLocked) {
                LeaveCriticalSection(&cs);
                ret = false;
            }
            return ret;
#elif defined(UKN_OS_FAMILY_UNIX)
            return (pthread_mutex_trylock(&mutex) == 0) ? true: false;
#endif
        }
            
        inline void Mutex::unlock() {
#ifdef UKN_OS_WINDOWS
            mLocked= false;
            LeaveCriticalSection(&cs);
#elif defined(UKN_OS_FAMILY_UNIX)
            pthread_mutex_unlock(&mutex);
#endif
        }
            
        void* Mutex::getSysMutex() {
#ifdef UKN_OS_WINDOWS
            return &cs;
#elif defined(UKN_OS_FAMILY_UNIX)
            return &mutex;
#endif
        }
            
        RecursiveMutex::RecursiveMutex() {
#ifdef UKN_OS_WINDOWS
            InitializeCriticalSection(&cs);
#elif defined(UKN_OS_FAMILY_UNIX)
            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            pthread_mutex_init(&mutex, &attr);
#endif
        }
            
        RecursiveMutex::~RecursiveMutex() {
#ifdef UKN_OS_WINDOWS
            DeleteCriticalSection(&cs);
#elif defined(UKN_OS_FAMILY_UNIX)
            pthread_mutex_destroy(&mutex);
#endif
        }
            
        inline void RecursiveMutex::lock() {
#ifdef UKN_OS_WINDOWS
            EnterCriticalSection(&cs);
#elif defined(UKN_OS_FAMILY_UNIX)
            pthread_mutex_lock(&mutex);
#endif
        }
            
        inline bool RecursiveMutex::try_lock() {
#ifdef UKN_OS_WINDOWS
            return TryEnterCriticalSection(&cs) ? true : false;
#elif defined(UKN_OS_FAMILY_UNIX)
            return (pthread_mutex_trylock(&mutex) == 0) ? true : false;
#endif
        }
            
        inline void RecursiveMutex::unlock() {
#ifdef UKN_OS_WINDOWS
            LeaveCriticalSection(&cs);
#elif defined(UKN_OS_FAMILY_UNIX)
            pthread_mutex_unlock(&mutex);
#endif
        }

        Semaphore::Semaphore(int _n)
#ifndef UKN_OS_WINDOWS
        :
        mutex(),
        cond(mutex),
        n(_n),
        max(_n)     
#endif
        {
                
#ifdef UKN_OS_WINDOWS
            sema = CreateSemaphoreW(NULL, _n, _n, NULL);
#endif
        }
                
        Semaphore::Semaphore(int _n, int _max)
#ifndef UKN_OS_WINDOWS
        :
        mutex(),
        cond(mutex),
        n(_n),
        max(_max)
#endif
        {
            assert( _n>=0 && _max>0 && _n<=_max);

#ifdef UKN_OS_WINDOWS
                    
            sema = CreateSemaphoreW(NULL, _n, _max, NULL);
#endif
        }
                    
        Semaphore::~Semaphore() { 
#ifdef UKN_OS_WINDOWS
            CloseHandle(sema);
#endif
        }
                    
        void Semaphore::wait() {
#ifdef UKN_OS_WINDOWS
            switch(WaitForSingleObject(sema,INFINITE)) {
                case WAIT_OBJECT_0:
                    return;
                default:
                    UKN_THROW_EXCEPTION("ukn::Thread::Semaphore: error wait");
            }
#elif defined(UKN_OS_FAMILY_UNIX)
            MutexGuard<Mutex> lock(mutex);
            while(n < 1) {
                cond.wait();
            }
            --n;
#endif
        }
                    
        void Semaphore::set() {
#ifdef UKN_OS_WINDOWS
            if(!ReleaseSemaphore(sema, 1, NULL)) {
                    UKN_THROW_EXCEPTION("ukn::Thread::Semaphore: error set");
            }
#elif defined(UKN_OS_FAMILY_UNIX)
            MutexGuard<Mutex> lock(mutex);
            if(n < max) {
                ++n;
            } else {
                UKN_THROW_EXCEPTION("ukn::Thread::Semaphore: Cannot signal semaphore, exceed maximun");
            }
            cond.notify();
#endif
        }
             
#ifdef UKN_OS_WINDOWS
        
        static int 
        _win32_pthread_cond_init (pthread_cond_t *cv,
                                  const void *) {
            cv->waiters_count_ = 0;
            cv->was_broadcast_ = 0;
            cv->sema_ = CreateSemaphoreA (NULL,       // no security
                                          0,          // initially 0
                                          0x7fffffff, // max count
                                          NULL);      // unnamed 
            InitializeCriticalSection (&cv->waiters_count_lock_);
            cv->waiters_done_ = CreateEventA (NULL,  // no security
                                              FALSE, // auto-reset
                                              FALSE, // non-signaled initially
                                              NULL); // unnamed
            return 0;
        }
        
        static void _win32_pthread_cond_destroy(pthread_cond_t *cv) {
            CloseHandle(cv->waiters_done_);
            DeleteCriticalSection (&cv->waiters_count_lock_);
        }
        
        static int
        _win32_pthread_cond_wait (pthread_cond_t *cv, 
                                  pthread_mutex_t *external_mutex) {
            // Avoid race conditions.
            EnterCriticalSection (&cv->waiters_count_lock_);
            cv->waiters_count_++;
            LeaveCriticalSection (&cv->waiters_count_lock_);
            
            SignalObjectAndWait (*external_mutex, cv->sema_, INFINITE, FALSE);
            EnterCriticalSection (&cv->waiters_count_lock_);
            
            cv->waiters_count_--;
            
            int last_waiter = cv->was_broadcast_ && cv->waiters_count_ == 0;
            
            LeaveCriticalSection (&cv->waiters_count_lock_);
            
            if (last_waiter)
                SignalObjectAndWait (cv->waiters_done_, *external_mutex, INFINITE, FALSE);
            else
                WaitForSingleObject (*external_mutex, INFINITE);
            return 0;
        }
        
        static int
        _win32_pthread_cond_signal (pthread_cond_t *cv) {
            EnterCriticalSection (&cv->waiters_count_lock_);
            int have_waiters = cv->waiters_count_ > 0;
            LeaveCriticalSection (&cv->waiters_count_lock_);
            
            if (have_waiters)
                ReleaseSemaphore (cv->sema_, 1, 0);
            return 0;
        }
        
        static int
        _win32_pthread_cond_broadcast (pthread_cond_t *cv) {
            EnterCriticalSection (&cv->waiters_count_lock_);
            int have_waiters = 0;
            
            if (cv->waiters_count_ > 0) {
                cv->was_broadcast_ = 1;
                have_waiters = 1;
            }
            
            if (have_waiters) {
                ReleaseSemaphore (cv->sema_, cv->waiters_count_, 0);
                LeaveCriticalSection (&cv->waiters_count_lock_);
                
                WaitForSingleObject (cv->waiters_done_, INFINITE);
                cv->was_broadcast_ = 0;
            }
            else
                LeaveCriticalSection (&cv->waiters_count_lock_);
            return 0;
        }
#endif // UKN_OS_WINDOWS
        
        Condition::Condition(Mutex& mutex):
        _mutex(mutex) {
#ifdef UKN_OS_WINDOWS
            _win32_pthread_cond_init(&cond, NULL);
#elif defined(UKN_OS_FAMILY_UNIX)
            pthread_cond_init(&cond, NULL);
#endif
        }
        
        Condition::~Condition() {
#ifdef UKN_OS_WINDOWS
            _win32_pthread_cond_destroy(&cond);
#elif defined(UKN_OS_FAMILY_UNIX)
            pthread_cond_destroy(&cond);
#endif
        }
        
        void Condition::wait() {
#ifndef UKN_OS_WINDOWS
            if(pthread_cond_wait(&cond,
                                 static_cast<pthread_mutex_t*>(_mutex.getSysMutex())))
                UKN_THROW_EXCEPTION("ukn::Thread::Condition: error wait");
#elif defined(UKN_OS_FAMILY_UNIX)
            if(_win32_pthread_cond_wait(&cond,
                                        static_cast<pthread_mutex_t*>(_mutex.getSysMutex())))
                UKN_THROW_EXCEPTION("ukn::Thread::Condition: error wait");
#endif
        }
        
        void Condition::notify() {
#ifndef UKN_OS_WINDOWS
            if(pthread_cond_signal(&cond))
                UKN_THROW_EXCEPTION("ukn::Thread::Condition: error notify");
#elif defined(UKN_OS_FAMILY_UNIX)
            if(_win32_pthread_cond_signal(&cond))
                UKN_THROW_EXCEPTION("ukn::Thread::Condition: error notify");
#endif
        }
        
        void Condition::notifyAll() {
#ifndef UKN_OS_WINDOWS
            if(pthread_cond_broadcast(&cond))
                UKN_THROW_EXCEPTION("ukn::Thread::Condition: error broadcast");
#elif defined(UKN_OS_FAMILY_UNIX)
            if(_win32_pthread_cond_broadcast(&cond))
                UKN_THROW_EXCEPTION("ukn::Thread::Condition: error broadcast");
#endif
        }
        
        ThreadTask::ThreadTask():
        mArg(0) {
            
        }
        
        ThreadTask::ThreadTask(const ThreadFunc& func):
        mArg(0),
        mFunc(func) {
            
        }
        
        ThreadTask::ThreadTask(const ThreadFunc& func, void* arg):
        mArg(arg),
        mFunc(func) {
            
        }
        
        void ThreadTask::setArg(void* arg) {
            mArg = arg;
        }
        
        void* ThreadTask::getArg() const {
            return mArg;
        }
        
        void ThreadTask::operator()() const {
            if(mFunc)
                mFunc(mArg);
        }
        
        bool ThreadTask::isValid() const {
            return mFunc;
        }
        
#ifdef UKN_OS_FAMILY_UNIX
        static ThreadId _pthread_t_to_ID(const pthread_t& handle) {
            static Mutex idMapLoc;
            static std::map<pthread_t, uint32> idMap;
            static uint32 idCount(1);
            
            MutexGuard<Mutex> guard(idMapLoc);
            if(idMap.find(handle) == idMap.end()) 
                idMap[handle] = idCount++;
            return ThreadId(idMap[handle]);
        }
#endif
        
#ifdef UKN_OS_WINDOWS
        
        unsigned int WINAPI Thread::WrapperFunc(LPVOID pthis) {
            
#elif defined(UKN_OS_FAMILY_UNIX)
            
            void* Thread::WrapperFunc(void* pthis) {
                
#endif
                Thread* pthread = (Thread*)pthis;
                
                try {
                    pthread->mTask();
                } catch(...) {
                    std::terminate();
                }
                
                MutexGuard<Mutex> guard(pthread->mDataMutex);
                pthread->mIsActive = false;
                
                return 0;
            }
            
            Thread::Thread():
            mIsActive(false) {
                
            }
            
            Thread::~Thread() {
                
            }
            
            bool Thread::start(const ThreadTask& task) {
                if(mIsActive || mTask.isValid()) 
                    return 0;
                
                mIsActive = true;
                
#ifdef UKN_OS_WINDOWS
                mHandle = (HANDLE)_beginthreadex(0, 0, Thread::WrapperFunc, (void*)this, 0, &mWin32ThreadId);
                
#elif defined(UKN_OS_FAMILY_UNIX)
                if(pthread_create(&mHandle, NULL, Thread::WrapperFunc, (void*)this) != 0)
                    mHandle = 0;
#endif
                if(!mHandle) {
                    mIsActive = false;
                }
                
                return mIsActive;
            }
            
            Thread::native_handle_type Thread::getNativeHandle() {
                return mHandle;
            }
            
            bool Thread::isActive() const {
                return mIsActive;
            }
            
            bool Thread::joinable() {
                mDataMutex.lock();
                bool result = !mIsActive;
                mDataMutex.unlock();
                return result;
            }
            
            void Thread::join() {
                if(joinable()) {
#ifdef UKN_OS_WINDOWS
                    WaitForSingleObject(mHandle, INFINITE);
#elif defined(UKN_OS_FAMILY_UNIX)
                    pthread_join(mHandle, 0);
#endif
                }
            }
            
            ThreadId Thread::getId() const {
#ifdef UKN_OS_WINDOWS
                return ThreadId(mWin32ThreadId);
#elif defined(UKN_OS_FAMILY_UNIX)
                return _pthread_t_to_ID(mHandle);
#endif
            }
            
            uint32 Thread::HardwareConcurrency() {
#ifdef UKN_OS_WINDOWS
                SYSTEM_INFO si;
                GetSystemInfo(&si);
                return (uint32)si.dwNumberOfProcessors;
#elif defined(UKN_OS_FAMILY_UNIX)
                return (uint32)sysconf(_SC_NPROCESSORS_ONLN);
#endif
                return 0;
            }
            
            void Thread::exit() {
#ifdef UKN_OS_WINDOWS
                TerminateThread(mHandle, 0);
#elif defined(UKN_OS_FAMILY_UNIX)
                pthread_exit(&mHandle);
#endif
            }
            
            ThreadId Thread::GetCurrentThreadId() {
#ifdef UKN_OS_WINDOWS
                return ThreadId((uint32)::GetCurrentThreadId());
#elif defined(UKN_OS_FAMILY_UNIX)
                return _pthread_t_to_ID(pthread_self());
#endif
            }
            
#ifdef UKN_OS_WINDOWS
            
            RWLock::RWLock(): 
            readers(0),
            writersWaiting(0),
            writers(0) {
                mutex = CreateMutexW(NULL, FALSE, NULL);
                if(mutex != NULL) {
                    readEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
                    if(readEvent != NULL) {
                        writeEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
                    }
                }
            }
            
#elif defined(UKN_OS_FAMILY_UNIX)
            RWLock::RWLock() {
                pthread_rwlock_init(&rwl, NULL);
            }
#endif         
            
            RWLock::~RWLock() {
#ifdef UKN_OS_WINDOWS
                CloseHandle(mutex);
                CloseHandle(readEvent);
                CloseHandle(writeEvent);
#elif defined(UKN_OS_FAMILY_UNIX)
                pthread_rwlock_destroy(&rwl);
#endif
            }
            
            void RWLock::readLock() {
#ifdef UKN_OS_WINDOWS
                HANDLE h[2];
                h[0] = mutex;
                h[1] = readEvent;
                switch(WaitForMultipleObjects(2, h, TRUE, INFINITE)) {
                    case WAIT_OBJECT_0:
                    case WAIT_OBJECT_0+1:
                        ++readers;
                        ResetEvent(writeEvent);
                        ReleaseMutex(mutex);
                        break;
                    default:
                        UKN_THROW_EXCEPTION("ukn::Thread::RWLock: Cannot lock reader/writer lock");
                }
#elif defined(UKN_OS_FAMILY_UNIX)
                if(pthread_rwlock_rdlock(&rwl))
                    UKN_THROW_EXCEPTION("ukn::Thread::RWLock: Cannot lock reader/writer lock");
#endif
            }
            
            void RWLock::writeLock() {
#ifdef UKN_OS_WINDOWS
                addWriter();
                HANDLE h[2];
                h[0] = mutex;
                h[1] = writeEvent;
                switch(WaitForMultipleObjects(2, h, TRUE, INFINITE)) {
                    case WAIT_OBJECT_0:
                    case WAIT_OBJECT_0+1:
                        --writersWaiting;
                        ++readers;
                        ++writers;
                        ResetEvent(readEvent);
                        ResetEvent(writeEvent);
                        ReleaseMutex(mutex);
                        break;
                    default:
                        removeWriter();
                        UKN_THROW_EXCEPTION("ukn::Thread::RWLock: Cannot lock reader/writer lock");
                }
#elif defined(UKN_OS_FAMILY_UNIX)
                if(pthread_rwlock_wrlock(&rwl))
                    UKN_THROW_EXCEPTION("ukn::Thread::RWLock: Cannot lock reader/writer lock");
#endif     
            }
            
            void RWLock::unlock() {
#ifdef UKN_OS_WINDOWS
                switch(WaitForSingleObject(mutex, INFINITE)) {
                    case WAIT_OBJECT_0:
                        writers = 0;
                        if(writersWaiting == 0)
                            SetEvent(readEvent);
                        if(--readers == 0)
                            SetEvent(writeEvent);
                        ReleaseMutex(mutex);
                        break;
                    default:
                        UKN_THROW_EXCEPTION("ukn::Thread::RWLock: Cannot unlock reader/writer lock");
                }
#elif defined(UKN_OS_FAMILY_UNIX)
                if(pthread_rwlock_unlock(&rwl))
                    UKN_THROW_EXCEPTION("ukn::Thread::RWLock: Cannot unlock reader/writer lock");
#endif   
            }
            
#ifdef UKN_OS_WINDOWS
            
            inline bool RWLock::isValid() const {
                return mutex && readEvent && writeEvent;
            }
            
            inline void RWLock::addWriter() {
                switch(WaitForSingleObject(mutex, INFINITE)) {
                    case WAIT_OBJECT_0:
                        if(++writersWaiting == 1)
                            ResetEvent(readEvent);
                        ReleaseMutex(mutex);
                        break;
                    default:
                        UKN_THROW_EXCEPTION("ukn::Thread::RWLock: Cannot add writer");
                }
            }
            
            inline void RWLock::removeWriter() {
                switch(WaitForSingleObject(mutex, INFINITE)) {
                    case WAIT_OBJECT_0:
                        if(--writersWaiting == 0 && writers == 0)
                            SetEvent(readEvent);
                        ReleaseMutex(mutex);
                        break;
                    default:
                        UKN_THROW_EXCEPTION("ukn::Thread::RWLock: Cannot remove writer");
                }
            }  
			  
#endif

        ThreadPool::ThreadPool():
        mMutex(),
        mCond(mMutex),
        mRunning(false) {
            
        }
        
        ThreadPool::~ThreadPool() {
            mRunning = false;
            join();
            
            std::for_each(mThreads.begin(),
                          mThreads.end(),
                          DeleteSTLPtr());
        }
        
        void ThreadPool::start(uint32 numThreads) {
            if(!mThreads.empty()) {
                log_error("ukn::ThreadPool::Start: thread pool already started");
            }
            
            mRunning = false;
            mThreads.reserve(numThreads);
            for(uint32 i=0; i<numThreads; ++i) {
                ThreadTask task(Bind(this, &ThreadPool::runInThread));
                
                Thread* thread = new Thread;
                mThreads.push_back(thread);
                thread->start(task);
            }
        }
        
        void ThreadPool::join() {
            if(mRunning) {
                mRunning = false;
                mCond.notifyAll();
                
                ThreadList::const_iterator it = mThreads.begin();
                for(; it != mThreads.end(); ++it)
                    (*it)->join();
            }
        }
        
        void ThreadPool::run(const ThreadTask& task) {
            if(mThreads.empty()) {
                task();
            } else {
                MutexGuard<Mutex> lock(mMutex);
                mTasks.push(task);
                mCond.notify();
            }
        }
        
        ThreadTask ThreadPool::take() {
            MutexGuard<Mutex> lock(mMutex);
            while(mTasks.empty() && mRunning)
                mCond.wait();
            
            ThreadTask task;
            if(!mTasks.empty()) {
                task = mTasks.front();
                mTasks.pop();
            }
            return task;
        }
        
        void ThreadPool::runInThread(void* arg) {
            try {
                while(mRunning) {
                    ThreadTask task(take());
                    task();
                }
            } catch(...) {
                log_error("ukn::ThreadPool::runInThread: error executing task");
            }
        }
        
        bool ThreadPool::isRunning() const {
            return mRunning;
        }
        
        namespace {
            static SingletonHolder<ThreadPool, void> instance;
        }
        
        ThreadPool& ThreadPool::DefaultObject() {
            return *instance.get();
        }
    }
    
} // namespace ukn
