//
//  SysUtil.h
//  Project Unknown
//
//  Created by Robert Bu on 12/1/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_SysUtil_h
#define Project_Unknown_SysUtil_h

#include "UKN/Platform.h"
#include "UKN/Util.h"

namespace ukn {
    
    enum MessageBoxOption {
        MBO_OK          = 0x00000000L,
        MBO_OKCancel    = 0x00000001L,
        MBO_IconError   = 0x00000010L,
        MBO_IconWarning = 0x00000030L,
        MBO_IconInfo    = 0x00000040L,
    };
    
    enum MessageBoxButton {
        MBB_OK          = 1,
        MBB_Cancel      = 2,
    };
    
    class UKN_API MessageBox {
    public:
        static MessageBoxButton Show(const ukn_string& mssg, const ukn_string& title, int option);
        static MessageBoxButton Show(const ukn_wstring& mssg, const ukn_wstring& title, int option);
    };
    
    class UKN_API SystemInformation {
    public:
        // in mhz
        static uint64 GetProcessorSpeed();
        // in bytes
        static uint64 GetMemorySize();
        // formatted string
        static ukn_string GetOSVersion();
        
        struct DesktopMode {
            uint32 width;
            uint32 height;
            
            uint32 bpp;
        };
        
        // array size at least 1
        // and the first one is current desktop resolution
        static Array<SystemInformation::DesktopMode> EnumDesktopMode();
    };
       
    /**
     * Retrives cpuinfo from current platform
     **/
    
    class UKN_API CpuInfo {
    public:
        enum Feature {
            HTT             = 1UL << 0,
            MMX             = 1UL << 1,
            SSE             = 1UL << 2,
            SSE2            = 1UL << 3,
            SSE3            = 1UL << 4,
            SSSE3           = 1UL << 5,
            SSE41           = 1UL << 6,
            SSE42           = 1UL << 7,
            SSE4A           = 1UL << 8,
            MisalignedSSE   = 1UL << 9,
            X64             = 1UL << 10,
            FMA             = 1UL << 11,
            MOVBE           = 1UL << 12,
            POPCNT          = 1UL << 13,
            AES             = 1UL << 14,
            AVX             = 1UL << 15,
            LZCNT           = 1UL << 16
        };
        
    public:
        CpuInfo();
        
        const std::string& getCPUString() const {
            return mCPUString;
        }
        
        const std::string& getCPUBrandString() const {
            return mCPUBrandString;
        }
        
        bool testFeature(Feature feature) const {
            return mFeatureMask & feature ? true : false;
        }
        
        int getNumHWThreads() const {
            return mNumHWThreads;
        }
        
        int getNumCores() const {
            return mNumCores;
        }
        
    private:
        std::string mCPUString;
        std::string mCPUBrandString;
        uint64 mFeatureMask;
        
        int mNumCores;
        int mNumHWThreads;
    };

}


#endif
