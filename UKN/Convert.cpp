//
//  Convert.cpp
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/22/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#include "UKN/Convert.h"
#include "UKN/Serializer.h"
#include "UKN/StringUtil.h"

namespace ukn {
    
    int16 Convert::ToInt16(const ukn_string& str) {
        int16 result;
        SerializeHelper::FromString(str, &result);
        return result;
    }
    
    int32 Convert::ToInt32(const ukn_string& str) {
        int32 result;
        SerializeHelper::FromString(str, &result);
        return result;
    }
    
    int64 Convert::ToInt64(const ukn_string& str) {
        int64 result;
        SerializeHelper::FromString(str, &result);
        return result;
    }
    
    uint16 Convert::ToUInt16(const ukn_string& str) {
        uint16 result;
        SerializeHelper::FromString(str, &result);
        return result;
    }
    
    uint32 Convert::ToUInt32(const ukn_string& str) {
        uint32 result;
        SerializeHelper::FromString(str, &result);
        return result;
    }
    
    uint64 Convert::ToUInt64(const ukn_string& str) {
        uint64 result;
        SerializeHelper::FromString(str, &result);
        return result;
    }
    
    uint8 Convert::ToByte(const ukn_string& str) {
        uint8 result;
        SerializeHelper::FromString(str, &result);
        return result;
    }
    
    Array<uint16> Convert::ToUnicode(const ukn_string& str) {
        uint16* buffer = ukn_malloc_t(uint16, str.size()+1);
        utf8_to_unicode(buffer, str.c_str(), str.size());
        return Array<uint16>(buffer, str.size()+1);
    }
    
    double Convert::ToDouble(const ukn_string& str) {
        double result;
        SerializeHelper::FromString(str, &result);
        return result;
    }
    
    float Convert::ToFloat(const ukn_string& str) {
        float result;
        SerializeHelper::FromString(str, &result);
        return result;
    }
    
} // namespace ukn
