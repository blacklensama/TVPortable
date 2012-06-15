//
//  Thread.h
//  Project Unknown
//
//  Created by Robert Bu on 12/13/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_Thread_h
#define Project_Unknown_Thread_h

#include "UKN/Platform.h"
#include "UKN/Uncopyable.h"

#ifndef UKN_OS_WINDOWS
#include <pthread.h>
#include <signal.h>
#include <sched.h>
#include <unistd.h>
#else
#include <Windows.h>
#endif // UKN_OS_WINDOWS

#include "UKN/Exception.h"
#include "UKN/Function.h"

#include <vector>
#include <deque>
#include <queue>

namespace ukn {
    
    namespace thread {
        
        class UKN_API Mutex {
        public:
            Mutex();
            ~Mutex();
            
            void lock();
            bool tryLock();
            
            void unlock();
            void* getSysMutex();
            
        private:
#ifdef UKN_OS_WINDOWS
            CRITICAL_SECTION cs;
            bool mLocked;
#elif defined(UKN_OS_FAMILY_UNIX)
            pthread_mutex_t mutex;
#endif
        };
        
        class RecursiveMutex {
        public:
            RecursiveMutex();
            ~RecursiveMutex();
            
            void lock();
            bool try_lock();
            void unlock();
            
        private:
#ifdef UKN_OS_WINDOWS
            CRITICAL_SECTION cs;
#elif defined(UKN_OS_FAMILY_UNIX)
            pthread_mutex_t mutex;
#endif
            friend class Condition;
        };

#ifdef UKN_OS_WINDOWS
        typedef struct {
            int waiters_count_;            
            CRITICAL_SECTION waiters_count_lock_;
            
            HANDLE sema_;
            HANDLE waiters_done_;
            size_t was_broadcast_;
        } pthread_cond_t;
        
        typedef HANDLE pthread_mutex_t;
#endif
        
        template<typename T>
        class MutexGuard: Uncopyable {
        public:
            typedef T mutex_type;
            
            explicit MutexGuard(mutex_type& mutex): _mutex(mutex) {
                _mutex.lock();
            }
            
            ~MutexGuard() {
                _mutex.unlock();
            }
            
        private:
            mutex_type& _mutex;
        };
        
        class UKN_API Condition: Uncopyable {
        public:
            explicit Condition(Mutex& mutex);
            
            ~Condition();
            
            void wait();
            void notify();
            void notifyAll();
            
        private:
            pthread_cond_t cond;
            
        protected:
            Mutex& _mutex;
        };
        
        
        class Thread;
        
        class UKN_API ThreadId {
        public:
            ThreadId(): mId(0) { }
            
            ThreadId(uint32 aid): mId(aid) { }
            
            ThreadId(const ThreadId& rhs): mId(rhs.mId) { }
            
            inline ThreadId& operator=(const ThreadId& rhs) {
                if(this != &rhs) {
                    mId  = rhs.mId;
                }
                return *this;
            }
            
            inline friend bool operator==(const ThreadId& id1, const ThreadId& id2) {
                return id1.mId == id2.mId;
            }
            
            inline friend bool operator!=(const ThreadId& id1, const ThreadId& id2) {
                return id1.mId != id2.mId;
            }
            
            inline friend bool operator<=(const ThreadId& id1, const ThreadId& id2) {
                return id1.mId <= id2.mId;
            }
            
            inline friend bool operator>=(const ThreadId& id1, const ThreadId& id2) {
                return id1.mId >= id2.mId;
            }
            
            inline friend bool operator<(const ThreadId& id1, const ThreadId& id2) {
                return id1.mId < id2.mId;
            }
            
            inline friend bool operator>(const ThreadId& id1, const ThreadId& id2) {
                return id1.mId > id2.mId;
            }
            
            inline friend std::ostream& operator<<(std::ostream& os, const ThreadId& aid) {
                os << aid.mId;
                return os;
            }
            
        private:
            uint32 mId;
        };
        
        class UKN_API ThreadTask {
        public:
            typedef Function<void(void*)> ThreadFunc;
            
            ThreadTask();
            ThreadTask(const ThreadFunc& func);
            ThreadTask(const ThreadFunc& func, void* arg);
            
            void    setArg(void* arg);
            void*   getArg() const;
            
            void operator()() const;
            
            bool isValid() const;
            
            
        private:
            void* mArg;
            ThreadFunc mFunc;
        };
        
        class UKN_API Thread {
        public:
#ifdef UKN_OS_WINDOWS
            typedef HANDLE native_handle_type;
#elif defined(UKN_OS_FAMILY_UNIX)
            typedef pthread_t native_handle_type;
#endif
            
        public:
            Thread();
            ~Thread();
            
            bool start(const ThreadTask& task);
            
            bool isActive() const;
            
            void exit();
            void join();
            
            bool joinable();
            
            ThreadId getId() const;
            
            native_handle_type getNativeHandle();
            
            static uint32 HardwareConcurrency();
            static ThreadId GetCurrentThreadId();
            
        private:
            ThreadTask mTask;
            bool mIsActive;
            Mutex mDataMutex;
            
            ThreadId mId;
            
#ifdef UKN_OS_WINDOWS
            uint32 mWin32ThreadId;
            HANDLE mHandle;
            
            static unsigned int WINAPI WrapperFunc(LPVOID param);
#elif defined(UKN_OS_FAMILY_UNIX)
            static void* WrapperFunc(void* pthis);
            
            pthread_t mHandle;
            pthread_attr_t mAttr;
#endif
        };
                
        class UKN_API Semaphore {
        public:
            Semaphore(int _n);
            Semaphore(int _n, int _max);
            ~Semaphore();
            
            void wait();
            void set();
            
        private:
#ifdef UKN_OS_WINDOWS
            HANDLE sema;
#elif defined(UKN_OS_FAMILY_UNIX)
            volatile int n;
            int max;
            
            mutable Mutex mutex;
            Condition cond;
#endif
        };
        
        template<typename T>
        class BlockingQueue: Uncopyable {
        public:
            BlockingQueue():
            mutex(),
            cond(mutex),
            queue() {
            }
            
            void put(const T& x) {
                MutexGuard<Mutex> lock(mutex);
                queue.push_back(x);
                cond.notify();
            }
            
            T take() {
                while(queue.empty())  
                    cond.wait();
                
                MutexGuard<Mutex> lock(mutex);
                assert(!queue.empty());
                T front(queue.front());
                queue.pop_front();
                return front;
            }
            
            size_t size() const {
                MutexGuard<Mutex> lock(mutex);
                return queue.size();
            }
            
        private:
            mutable Mutex mutex;
            Condition cond;
            
            std::deque<T> queue;
        };
                
        class UKN_API CountDownLatch: Uncopyable {
        public:
            explicit CountDownLatch(int32 _count):
            mutex(),
            condition(mutex),
            count(_count) {
                
            }
            
            void wait() {
                MutexGuard<Mutex> lock(mutex);
                while(count > 0) {
                    condition.wait();
                }
            }
            
            void countDown() {
                MutexGuard<Mutex> lock(mutex);
                --count;
                if(count == 0)
                    condition.notifyAll();
            }
            
            int32 getCount() const {
                MutexGuard<Mutex> lock(mutex);
                return count;
            }
            
        private:
            mutable Mutex mutex;
            Condition condition;
            int32 count;
        };
        
        class UKN_API RWLock {
        public:
            RWLock();
            ~RWLock();
            
            void readLock();
            void writeLock();
            void unlock();
                
        private:
            bool isValid() const;            
            void addWriter();
            
            void removeWriter();
                
#ifdef UKN_OS_WINDOWS
            HANDLE mutex;
            HANDLE readEvent;
            HANDLE writeEvent;
            unsigned readers;
            unsigned writersWaiting;
            unsigned writers;
#elif defined(UKN_OS_FAMILY_UNIX)
            pthread_rwlock_t rwl;
#endif
        };
        
        class UKN_API ThreadPool: Uncopyable {
        public:
            ThreadPool();
            ~ThreadPool();
            
            void start(uint32 numThreads);
            void join();
            
            void run(const ThreadTask&);
            
            bool isRunning() const;
            
            static ThreadPool& DefaultObject();
            
        private:
            void runInThread(void* arg);
            
            ThreadTask take();
            
            Mutex mMutex;
            Condition mCond;
            
            typedef std::vector<Thread*> ThreadList;
            ThreadList mThreads;
            
            typedef std::queue<ThreadTask> ThreadTaskQueue;
            ThreadTaskQueue mTasks;
            
            bool mRunning;
        };
            
    } // namespace thread
    
} // namespace ukn

#endif
