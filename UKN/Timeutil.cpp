//
//  Timeutil.cpp
//  Project Unknown
//
//  Created by Robert Bu on 11/24/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#include "UKN/TimeUtil.h"
#include "UKN/Platform.h"

#if !defined(UKN_OS_WINDOWS)
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#include <errno.h>

#else
#include <windows.h>
#endif

namespace ukn {
    
    Timestamp::Timestamp() {
        update();
    }
    
    Timestamp::Timestamp(TimeVal tv) {
        _ts = tv;
    }
    
    Timestamp::Timestamp(const Timestamp& other) {
        _ts = other._ts;
    }
    
    Timestamp::~Timestamp() {
    }
    
    Timestamp& Timestamp::operator = (const Timestamp& other) {
        _ts = other._ts;
        return *this;
    }
    
    Timestamp& Timestamp::operator = (TimeVal tv) {
        _ts = tv;
        return *this;
    }
    
    void Timestamp::swap(Timestamp& timestamp) {
        std::swap(_ts, timestamp._ts);
    }
    
    Timestamp Timestamp::FromEpochTime(std::time_t t) {
        return Timestamp(TimeVal(t)*Resolution());
    }
    
    Timestamp Timestamp::FromUtcTime(UtcTimeVal val) {
        val -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
        val /= 10;
        return Timestamp(val);
    }
    
    Timestamp::TimeVal Timestamp::CurrentTime() {
        TimeVal ts;
#if defined(UKN_OS_WINDOWS)
        
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
        epoch.LowPart  = 0xD53E8000;
        epoch.HighPart = 0x019DB1DE;
        
        ULARGE_INTEGER tts;
        tts.LowPart  = ft.dwLowDateTime;
        tts.HighPart = ft.dwHighDateTime;
        tts.QuadPart -= epoch.QuadPart;
        ts = tts.QuadPart/10;
        
#else
        
        struct timeval tv;
        if (gettimeofday(&tv, NULL))
            throw ("cannot get time of day");
        ts = TimeVal(tv.tv_sec)*Resolution() + tv.tv_usec;
        
#endif
        return ts;
    }
    
    void Timestamp::update() {
#if defined(UKN_OS_WINDOWS)
        
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
        epoch.LowPart  = 0xD53E8000;
        epoch.HighPart = 0x019DB1DE;
        
        ULARGE_INTEGER ts;
        ts.LowPart  = ft.dwLowDateTime;
        ts.HighPart = ft.dwHighDateTime;
        ts.QuadPart -= epoch.QuadPart;
        _ts = ts.QuadPart/10;
        
#else
        
        struct timeval tv;
        if (gettimeofday(&tv, NULL))
            throw ("cannot get time of day");
        _ts = TimeVal(tv.tv_sec)*Resolution() + tv.tv_usec;
        
#endif
    }
    
#if defined(UKN_OS_WINDOWS)
    
    Timestamp Timestamp::fromFileTimeNP(uint32 fileTimeLow, uint32 fileTimeHigh) {
        ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
        epoch.LowPart  = 0xD53E8000;
        epoch.HighPart = 0x019DB1DE;
        
        ULARGE_INTEGER ts;
        ts.LowPart  = fileTimeLow;
        ts.HighPart = fileTimeHigh;
        ts.QuadPart -= epoch.QuadPart;
        
        return Timestamp(ts.QuadPart/10);
    }
    
    void Timestamp::toFileTimeNP(uint32& fileTimeLow, uint32& fileTimeHigh) const {
        ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
        epoch.LowPart  = 0xD53E8000;
        epoch.HighPart = 0x019DB1DE;
        
        ULARGE_INTEGER ts;
        ts.QuadPart  = _ts*10;
        ts.QuadPart += epoch.QuadPart;
        fileTimeLow  = ts.LowPart;
        fileTimeHigh = ts.HighPart;
    }
 
#endif
   
#ifndef UKN_OS_WINDOWS
    // sleep for milliseconds
    // see my http://cc.byexamples.com/2007/05/25/nanosleep-is-better-than-sleep-and-usleep/ for more information
    void msleep(uint32 msec) {
        struct timespec timeout0;
        struct timespec timeout1;
        struct timespec* tmp;
        struct timespec* t0 = &timeout0;
        struct timespec* t1 = &timeout1;
        
        t0->tv_sec = msec / 1000;
        t0->tv_nsec = (msec % 1000) * (1000 * 1000);	
        while ((nanosleep(t0, t1) == (-1)) && (errno == EINTR)) {
            
            tmp = t0;
            t0 = t1;
            t1 = tmp;
        }
    }
    
} // namespace ukn

#else

	void msleep(uint32 msec) {
		Sleep(msec);
	}

} // namespace ukn

#endif

#if defined(UKN_OS_FAMILY_APPLE)

#include <mach/mach.h>
#include <mach/mach_time.h>
#if defined(UKN_OS_OSX)
#include <CoreServices/CoreServices.h>
#endif
#include <time.h>

namespace ukn {
    
    static uint64 TIME_PRECISION = 1000000;
    
    static void mach_absolute_difference(uint64_t end, uint64_t start, struct timespec *tp) {  
        uint64_t difference = end - start;  
        static mach_timebase_info_data_t info = {0,0};  
        
        if (info.denom == 0)  
            mach_timebase_info(&info);  
        
        uint64_t elapsednano = difference * (info.numer / info.denom);  
        
        tp->tv_sec = elapsednano * 1e-9;  
        tp->tv_nsec = elapsednano - (tp->tv_sec * 1e9);  
    }
    
    
    static uint64_t mach_time_to_nanosec(uint64_t dt) {
#if defined(UKN_OS_IOS)
        return dt;
#elif defined(UKN_OS_OSX) 
        Nanoseconds elapsedNano = (AbsoluteToNanoseconds( *(AbsoluteTime *) &dt ));
        return * (uint64_t *) &elapsedNano;
#endif
    }
    
    static double delta_to_milisec(uint64_t dt) {
        return (double)mach_time_to_nanosec(dt) / TIME_PRECISION;
    }

    class AppleFrameCounter: public FrameCounter {
    public:
        float doWaitToNextFrame() {
            static uint64_t oldtime = mach_absolute_time();
            uint64_t time;
            uint64_t fpsInterval;
            double deltaTime;
            
            if(getDesiredFps() == FpsUnlimited) {
				fpsInterval = 0;
			} else {
                fpsInterval = 1.f / getDesiredFps() * (1000*1000*1000);
            }
			
			if(mach_time_to_nanosec(mach_absolute_time() - oldtime) < fpsInterval) {
				for(;;) {
					uint64 nanodt = mach_time_to_nanosec(mach_absolute_time() - oldtime);
					if(nanodt >= fpsInterval - TIME_PRECISION) {
						for(;;)
							if(mach_time_to_nanosec(mach_absolute_time() - oldtime) >= fpsInterval)
								break;
						break;
					} else {
						msleep(1);
					}
				}
			}
			time = mach_absolute_time();
			
			if(oldtime <= time - fpsInterval && oldtime >= time - (fpsInterval*2)) {
				deltaTime = delta_to_milisec(fpsInterval) / 1000.f;
				oldtime += fpsInterval;
			} else {
				deltaTime = delta_to_milisec(time-oldtime) / 1000.f;
				oldtime = time;
			}
			if(deltaTime >= 1.f)
				deltaTime = 1.f / getDesiredFps();
			
			return deltaTime;
        }
    };
       
} // namespace ukn

#endif // UKN_OS_OSX

#if defined(UKN_OS_PSP)

namespace ukn {
    
    
} // namespace ukn

#endif // UKN_OS_PSP


#if defined(UKN_OS_WINDOWS)

namespace ukn {
    
    /* Orz by Advance */    
    typedef __int64 INT64, *PINT64, *LPINT64;

    INT64 GetCurrentSystemTime(void) {
        LARGE_INTEGER Counter;
        
        if (!QueryPerformanceCounter(&Counter))
            return 0;
        
        return (INT64)Counter.QuadPart;
    }
    
    void DelayExecution(unsigned int Milliseconds) {
        Sleep(Milliseconds);
    }
    
    void RaisePriority(void) {
        int CurrentPriority = GetThreadPriority(GetCurrentThread());
        
        if (CurrentPriority < THREAD_PRIORITY_HIGHEST)
            SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
    }
    
    INT64 GetSystemTimePrecision(void) {
        LARGE_INTEGER Frequency;
        
        if (!QueryPerformanceFrequency(&Frequency))
            return 0;
        
        return (INT64)Frequency.QuadPart;
    }
    
    void LowerPriority(void) {
        int CurrentPriority = GetThreadPriority(GetCurrentThread());
        
        if (CurrentPriority > THREAD_PRIORITY_NORMAL)
            SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
    }

    class WindowsFrameCounter: public FrameCounter {
    public:
        virtual float doWaitToNextFrame() {
			int bPriorityRaised = 0;
		    LONGLONG TimeInterval, TimePrecision, NowTime;
			static LONGLONG lastTime = GetCurrentSystemTime();
			double deltaTime;
            
			LARGE_INTEGER Frequency;
		    QueryPerformanceFrequency(&Frequency);
			TimePrecision = Frequency.QuadPart;
			if (getDesiredFps() > 0) {
				TimeInterval = TimePrecision / (LONGLONG)getDesiredFps();
			}
			else {
				TimeInterval = 0;
			}
            
			if (GetCurrentSystemTime() - lastTime >= TimeInterval) {
				if (GetThreadPriority(GetCurrentThread()) > THREAD_PRIORITY_NORMAL)
					SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
		    }
		    else {
		        for (;;) {
		            if (GetCurrentSystemTime() - lastTime >= (TimeInterval - (TimePrecision / (1000 * 2)))) {
		                if (!bPriorityRaised && GetThreadPriority(GetCurrentThread()) > THREAD_PRIORITY_NORMAL)
		                    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
						for (;;) {
						    if (GetCurrentSystemTime() - lastTime >= TimeInterval)
					            break;
				        }
			            break;
					}
		            else {
		                if (!bPriorityRaised) {
		                    if (GetThreadPriority(GetCurrentThread()) < THREAD_PRIORITY_HIGHEST)
								SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
		                    bPriorityRaised = 1;
		                }
		                Sleep(1);
		            }
		        }
		    }
            
			NowTime = GetCurrentSystemTime();
            
		    if (lastTime <= (NowTime - TimeInterval) && lastTime >= (NowTime - (TimeInterval * 2))) {
				deltaTime = (float)((double)(TimeInterval) / (double)(TimePrecision));
		        lastTime += TimeInterval;
			}
		    else {
				deltaTime = (float)((double)(NowTime - lastTime) / (double)(TimePrecision));
		        lastTime = NowTime;
			}
    
            // delta too long(thread maybe hanged by MessageBox etc, force set to desired fps
			if(deltaTime >= 1.f) 
				deltaTime = 1.f / getDesiredFps();
            
			return (float)deltaTime;
		} 
	};
    
} // namespace ukn


#endif // UKN_OS_WINDOWS

namespace ukn {
    
    class DefaultFrameCounter: public FrameCounter {
    public:
        float doWaitToNextFrame() {
            uint64 fpsInterval;
            static Timestamp oldtime = Timestamp();
            Timestamp time;
            double deltaTime;
            
            if(getDesiredFps() == FpsUnlimited) {
				fpsInterval = 0;
			} else {
                fpsInterval = (uint64)((double)(1.0 / getDesiredFps()) * 1000 * 1000);
            }
            
			if(Timestamp::CurrentTime() - oldtime.epochMicroseconds() < fpsInterval) {
				for(;;) {
                    uint64 ldt = Timestamp::CurrentTime() - oldtime.epochMicroseconds();
					if(ldt >= fpsInterval - 1000) {
						for(;;)
							if(Timestamp::CurrentTime() - oldtime.epochMicroseconds() >= fpsInterval)
								break;
						break;
					} else {
						msleep(1);
					}
				}
			}
            time.update();
            
            if(oldtime <= time - fpsInterval && oldtime >= time - (fpsInterval*2)) {
				deltaTime = fpsInterval / (double)(1000*1000);
				oldtime += fpsInterval;
			} else {
				deltaTime = (time - oldtime) / (double)(1000*1000);
				oldtime = time;
			}
			/* too long */
			if(deltaTime >= 1.f)
				deltaTime = 1.f / getDesiredFps();
			
			return (float)deltaTime;
        }
    };    
} // namespace ukn

namespace ukn {
   
    FrameCounter::FrameCounter():
    mDesiredFps(FpsUnlimited),
    mPrevDelta(0),
    mCurrentFps(0),
    mFrameCount(0),
    mDelta(0) {
        getRunningTime();
    }
    
    FrameCounter::~FrameCounter() {
        
    }
    
    void FrameCounter::setDesiredFps(int32 fps) {
        mDesiredFps = (float)fps;
        
        if(mDesiredFps != FpsUnlimited)
            mPrevDelta = 1.f / mDesiredFps;
    }
    
    float FrameCounter::getDesiredFps() const {
        return mDesiredFps;
    }
    
    float FrameCounter::getPrevDelta() const {
        return mPrevDelta;
    }
    
    float FrameCounter::getCurrentFps() const {
        return mCurrentFps;
    }
    
    uint64 FrameCounter::getDeltaTime() const {
        return mDelta;
    }
    
    void FrameCounter::waitToNextFrame() {
        mDelta = mDetalTime.elapsed();

        mDetalTime.update();

        mPrevDelta = doWaitToNextFrame();
        mCurrentFps = 1.0f / mPrevDelta;
        
        ++mFrameCount;
    }
    
    uint64 FrameCounter::getFrameCount() const {
        return mFrameCount;
    }
    
    void FrameCounter::setDelta(float delta) {
        mPrevDelta = delta;
    }
    
    void FrameCounter::setCurrentFps(float fps) {
        mCurrentFps = fps;
    }
    
    FrameCounter& FrameCounter::Instance() {
#if defined(UKN_OS_FAMILY_APPLE)
        static AppleFrameCounter static_counter;
        
#elif defined(UKN_OS_WINDOWS)
        static WindowsFrameCounter static_counter;
        
#elif defined(UKN_OS_PSP)
        static PSPFrameCounter static_counter
        
#else
        static DefaultFrameCounter static_counter;
        
#endif
        return static_counter;
    }
    
    uint64 FrameCounter::getRunningTime() const {
        static Timestamp starttime = Timestamp();
        return starttime.elapsed() / 1000;
    }
    
} // namespace ukn



