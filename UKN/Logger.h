//
//  Logger.h
//  Project Unknown
//
//  Created by Robert Bu on 11/24/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_Logger_h
#define Project_Unknown_Logger_h

#include "UKN/Platform.h"
#include "UKN/PreDeclare.h"
#include "UKN/StringUtil.h"

#include <deque>

namespace ukn {
    
    /**
     * Simple logger implementation
     **/
    enum LoggerFeature {
        // prepend time since app start
        LF_PrependRunningTime,
        // prepend log level name except LL_Info
        LF_PrependLevelName,
        // also output to console
        LF_OutputToConsole,
    };
    
    enum LogLevel {
        LL_Error,
        LL_Warning,
        LL_Notice,
        LL_Info,
    };
    
    class Logger {
    protected:
        Logger(StreamPtr outputStream = StreamPtr());
        ~Logger();
        
    public:
        static Logger& Instance();
        
        // redirect to another stream
        // will output to standard output when a null stream is passed(StreamPtr())
        void redirect(StreamPtr);
        void setFeature(LoggerFeature feature, bool flag);
        
        void log(const ukn_wstring& log, LogLevel level=LL_Info);

        // log with LL_Info level
        Logger& operator<<(const ukn_wstring& log);
        
        void clear();
        
        size_t getLogSize() const;
        const std::deque<ukn_wstring>& getLogQueue() const;
        
    private:
        std::deque<ukn_wstring> mLogQueue;
        
        StreamPtr mOutputStream;
        
        bool mPrependTime;
        bool mPrependLevel;
        bool mOutputToConsole;
    };
    
    void log_error(const ukn_wstring& log);
    void log_notice(const ukn_wstring& log);
    void log_warning(const ukn_wstring& log);
    void log_info(const ukn_wstring& log);

	void log_error(const ukn_string& log);
    void log_notice(const ukn_string& log);
    void log_warning(const ukn_string& log);
    void log_info(const ukn_string& log);

#define ukn_logged_assert(cond, log) \
    if(!(cond)) { \
        ukn::log_error(ukn::format_string("ukn::assertion failed with mssg %s at function %s, file %s, line %d", log, __FUNCTION__, __FILE__, __LINE__)); \
        ukn_assert(0); \
    }
} // namespace ukn

#endif
