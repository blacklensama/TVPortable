//
//  Hashing.h
//  Project Unknown
//
//  Created by Robert Bu on 12/5/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_Hashing_h
#define Project_Unknown_Hashing_h

#include "UKN/Platform.h"
#include "UKN/PreDeclare.h"

namespace ukn {
    
    class Hash {
    public:
        static uint64 Crc32(const char *buf, size_t size);
        static uint64 Crc32(const wchar_t *buf, size_t size);
        static uint64 Crc32(const ukn_string& str);
        static uint64 Crc32(const ukn_wstring& str);
        
        static uint32 Adler32(const char* buf);
        static uint32 Adler32(const char* buf, size_t size);
        
        static ukn_string GetMD5(const StreamPtr stream);
    };
    
} // namespace ukn


#endif
