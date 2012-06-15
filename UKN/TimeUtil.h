//
//  TimeUtil.h
//  Project Unknown
//
//  Created by Robert Bu on 11/24/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_TimeUtil_h
#define Project_Unknown_TimeUtil_h

#include "UKN/Platform.h"

#include <ctime>

namespace ukn {
    
    // Timestamp from Poco C++ Library
    
    class UKN_API Timestamp
	/// A Timestamp stores a monotonic time value
	/// with (theoretical) microseconds resolution.
	/// Timestamps can be compared with each other
	/// and simple arithmetics are supported.
	/// Timestamps are UTC (Coordinated Universal Time)
	/// based and thus independent of the timezone
	/// in effect on the system.
    {
    public:
        typedef int64 TimeVal;    /// monotonic UTC time value in microsecond resolution
        typedef int64 UtcTimeVal; /// monotonic UTC time value in 100 nanosecond resolution
        typedef int64 TimeDiff;   /// difference between two timestamps in microseconds
        
        Timestamp();
		/// Creates a timestamp with the current time.
		
        Timestamp(TimeVal tv);
		/// Creates a timestamp from the given time value.
		
        Timestamp(const Timestamp& other);
		/// Copy constructor.
		
        ~Timestamp();
		/// Destroys the timestamp
		
        Timestamp& operator = (const Timestamp& other);
        Timestamp& operator = (TimeVal tv);
        
        void swap(Timestamp& timestamp);
		/// Swaps the Timestamp with another one.
        
        void update();
		/// Updates the Timestamp with the current time.
        
        bool operator == (const Timestamp& ts) const;
        bool operator != (const Timestamp& ts) const;
        bool operator >  (const Timestamp& ts) const;
        bool operator >= (const Timestamp& ts) const;
        bool operator <  (const Timestamp& ts) const;
        bool operator <= (const Timestamp& ts) const;
        
        Timestamp  operator +  (TimeDiff d) const;
        Timestamp  operator -  (TimeDiff d) const;
        TimeDiff   operator -  (const Timestamp& ts) const;
        Timestamp& operator += (TimeDiff d);
        Timestamp& operator -= (TimeDiff d);
        
        std::time_t epochTime() const;
		/// Returns the timestamp expressed in time_t.
		/// time_t base time is midnight, January 1, 1970.
		/// Resolution is one second.
		
        UtcTimeVal utcTime() const;
		/// Returns the timestamp expressed in UTC-based
		/// time. UTC base time is midnight, October 15, 1582.
		/// Resolution is 100 nanoseconds.
        
        TimeVal epochMicroseconds() const;
		/// Returns the timestamp expressed in microseconds
		/// since the Unix epoch, midnight, January 1, 1970.
        
        static TimeVal CurrentTime();
        
        TimeDiff elapsed() const;
		/// Returns the time elapsed since the time denoted by
		/// the timestamp. Equivalent to Timestamp() - *this.
        
        bool isElapsed(TimeDiff interval) const;
		/// Returns true iff the given interval has passed
		/// since the time denoted by the timestamp.
        
        static Timestamp FromEpochTime(std::time_t t);
		/// Creates a timestamp from a std::time_t.
		
        static Timestamp FromUtcTime(UtcTimeVal val);
		/// Creates a timestamp from a UTC time value.
		
        static TimeVal Resolution();
		/// Returns the resolution in units per second.
		/// Since the timestamp has microsecond resolution,
		/// the returned value is always 1000000.
        
#if defined(_WIN32)
        static Timestamp fromFileTimeNP(uint32 fileTimeLow, uint32 fileTimeHigh);
        void toFileTimeNP(uint32& fileTimeLow, uint32& fileTimeHigh) const;
#endif
        
    private:
        TimeVal _ts;
    };
    
    inline bool Timestamp::operator == (const Timestamp& ts) const {
        return this->_ts == ts._ts;
    }
    
    inline bool Timestamp::operator != (const Timestamp& ts) const {
        return this->_ts != ts._ts;
    }
    
    inline bool Timestamp::operator >  (const Timestamp& ts) const {
        return this->_ts > ts._ts;
    }
    
    inline bool Timestamp::operator >= (const Timestamp& ts) const {
        return this->_ts >= ts._ts;
    }
    
    inline bool Timestamp::operator <  (const Timestamp& ts) const {
        return this->_ts < ts._ts;
    }
    
    inline bool Timestamp::operator <= (const Timestamp& ts) const {
        return this->_ts <= ts._ts;
    }
    
    inline Timestamp Timestamp::operator + (Timestamp::TimeDiff d) const {
        return Timestamp(this->_ts + d);
    }
    
    inline Timestamp Timestamp::operator - (Timestamp::TimeDiff d) const {
        return Timestamp(this->_ts - d);
    }
    
    inline Timestamp::TimeDiff Timestamp::operator - (const Timestamp& ts) const {
        return this->_ts - ts._ts;
    }
    
    inline Timestamp& Timestamp::operator += (Timestamp::TimeDiff d) {
        this->_ts += d;
        return *this;
    }
    
    inline Timestamp& Timestamp::operator -= (Timestamp::TimeDiff d) {
        this->_ts -= d;
        return *this;
    }
    
    inline std::time_t Timestamp::epochTime() const {
        return std::time_t(this->_ts/Resolution());
    }
    
    inline Timestamp::UtcTimeVal Timestamp::utcTime() const {
        return this->_ts*10 + (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
    }
    
    inline Timestamp::TimeVal Timestamp::epochMicroseconds() const {
        return this->_ts;
    }
    
    inline Timestamp::TimeDiff Timestamp::elapsed() const {
        Timestamp now;
        return now - *this;
    }
    
    inline bool Timestamp::isElapsed(Timestamp::TimeDiff interval) const {
        Timestamp now;
        Timestamp::TimeDiff diff = now - *this;
        return diff >= interval;
    }
    
    inline Timestamp::TimeVal Timestamp::Resolution() {
        return 1000000;
    }
    
    inline void swap(Timestamp& s1, Timestamp& s2) {
        s1.swap(s2);
    }
    
    class UKN_API FrameCounter {
    public:
        enum { FpsUnlimited = 0 };
        
        static FrameCounter& Instance();
        
        void setDesiredFps(int32 fps);
        
        float getDesiredFps() const;
        // frame delta
        float getPrevDelta() const;
        float getCurrentFps() const;
        
        void waitToNextFrame();
        uint64 getFrameCount() const; 
        uint64 getRunningTime() const;
        
        // actual running delta time, not frame delta
        // in ms
        uint64 getDeltaTime() const;
        
    protected:    
        void setDelta(float delta);
        void setCurrentFps(float fps);
        
        virtual float doWaitToNextFrame() = 0;
        
        FrameCounter();
        ~FrameCounter();
        
    private:
        float mDesiredFps;
        float mPrevDelta;
        float mCurrentFps;
        uint64 mFrameCount;
        
        uint64 mDelta;
        Timestamp mDetalTime;
    };
    
    inline void msleep(uint32 msec);

    
} // namespace ukn

#endif
