//
//  Serializer.h
//  Project Unknown
//
//  Created by Robert Bu on 12/14/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_Serializer_h
#define Project_Unknown_Serializer_h

#include "UKN/Platform.h"
#include "UKN/PreDeclare.h"
#include "UKN/StringUtil.h"
#include "UKN/MathUtil.h"
#include "UKN/Logger.h"
#include "UKN/StreamWrapper.h"
#include "UKN/Basic.h"

#include <iostream>
#include <string>
#include <sstream>

namespace ukn {
    
    enum SerializableTypeId {
        STI_Unknown = -1,
        
        STI_BOOL,
        
        STI_UINT16,
        STI_INT16,
        
        STI_UINT,
        STI_INT,
        
        STI_ULONG,
        STI_LONG,
        
        STI_BYTE,
        STI_CHAR,
        
        STI_FLOAT,
        STI_DOUBLE,
        
        STI_UINT64,
        STI_INT64,
        
        STI_STRING,
        
        STI_RAW
    };
    
    class SerializeHelper {
    public:
        template<class T> static void               FromString(const ukn_string& str, T* val);
        template<class T> static T                  DefaultValue();
        
        template<class T> static ukn_string         ToString(T* val);
        template<class T> static SerializableTypeId GetTypeId();
    };
        
    class IBinarySerializable: public virtual Interface {
    public:
        virtual bool serialize(const SharedPtr<StreamWriter>& serializer) = 0;
        virtual bool deserialize(const SharedPtr<StreamWriter>& serializer) = 0;
    };
    
    class IConfigSerializable: public virtual Interface {
    public:
        virtual bool serialize(const ConfigParserPtr& config) = 0;
        virtual bool deserialize(const ConfigParserPtr& config) = 0;
    };
    
} // namespace ukn

#endif
