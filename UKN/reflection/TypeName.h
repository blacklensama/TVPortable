//
//  TypeName.h
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/27/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#ifndef Project_Unknown_rf_TypeName_h
#define Project_Unknown_rf_TypeName_h

#include "UKN/Hashing.h"

namespace ukn {
    
    namespace reflection {
        
        // to support custom type,
        // implement TypeNameRetriever with a static function 
        //      static const char* Name()
        // and register it with typedb
        template<typename TYPE>
        struct TypeNameRetriever;
        
        template<> struct TypeNameRetriever<int64>{ 
            static const char* Name() { return "int64"; }
        };
        
        template<> struct TypeNameRetriever<uint64>{ 
            static const char* Name() { return "uint64"; }
        };
        
        template<> struct TypeNameRetriever<int32>{ 
            static const char* Name() { return "int32"; }
        };
        
        template<> struct TypeNameRetriever<uint32>{ 
            static const char* Name() { return "uint32"; }
        };
        
        template<> struct TypeNameRetriever<int16>{ 
            static const char* Name() { return "int16"; }
        };
        
        template<> struct TypeNameRetriever<uint16>{ 
            static const char* Name() { return "uint16"; }
        };
        
        template<> struct TypeNameRetriever<int8>{ 
            static const char* Name() { return "int8"; }
        };
        
        template<> struct TypeNameRetriever<uint8>{ 
            static const char* Name() { return "uint8"; }
        };
        
        template<> struct TypeNameRetriever<float>{ 
            static const char* Name() { return "float"; }
        };
        
        template<> struct TypeNameRetriever<double>{ 
            static const char* Name() { return "double"; }
        };
        
        template<> struct TypeNameRetriever<ukn_string>{ 
            static const char* Name() { return "string"; }
        };
        
        template<> struct TypeNameRetriever<ukn_wstring>{ 
            static const char* Name() { return "wstring"; }
        };
        
        template<size_t LENGTH> struct TypeNameRetriever<int64[LENGTH]>{ 
            static const char* Name() { return "int64"; }
        };
        
        template<size_t LENGTH> struct TypeNameRetriever<uint64[LENGTH]>{ 
            static const char* Name() { return "uint64"; }
        };
        
        template<size_t LENGTH> struct TypeNameRetriever<int32[LENGTH]>{ 
            static const char* Name() { return "int32"; }
        };
        
        template<size_t LENGTH> struct TypeNameRetriever<uint32[LENGTH]>{ 
            static const char* Name() { return "uint32"; }
        };
        
        template<size_t LENGTH> struct TypeNameRetriever<int16[LENGTH]>{ 
            static const char* Name() { return "int16"; }
        };
        
        template<size_t LENGTH> struct TypeNameRetriever<uint16[LENGTH]>{ 
            static const char* Name() { return "uint16"; }
        };
        
        template<size_t LENGTH> struct TypeNameRetriever<int8[LENGTH]>{ 
            static const char* Name() { return "int8"; }
        };
        
        template<size_t LENGTH> struct TypeNameRetriever<uint8[LENGTH]>{ 
            static const char* Name() { return "uint8"; }
        };
        
        template<size_t LENGTH> struct TypeNameRetriever<float[LENGTH]>{ 
            static const char* Name() { return "float"; }
        };
        
        template<size_t LENGTH> struct TypeNameRetriever<double[LENGTH]>{ 
            static const char* Name() { return "double"; }
        };
        
        template<size_t LENGTH> struct TypeNameRetriever<ukn_string[LENGTH]>{ 
            static const char* Name() { return "string"; }
        };
        
        template<size_t LENGTH> struct TypeNameRetriever<ukn_wstring[LENGTH]>{ 
            static const char* Name() { return "wstring"; }
        };
        
        template<typename TYPE>
        static uint32 GetTypeId() {
            static uint32 type_id = Hash::Adler32(TypeNameRetriever<TYPE>::Name());
            return type_id;
        }
        
        template<typename TYPE>
        static const char* GetTypeName() {
            static const char* name = TypeNameRetriever<TYPE>::Name();
            return name;
        }
        
        struct Name {
            uint32      hash;
            const char* text;
            
            Name():
            hash(0),
            text(0) {
                
            }
            
            Name(const char* _name):
            text(_name) {
                hash = Hash::Adler32(_name);
            }
            
            Name(const char* _name, uint32 _hash):
            text(_name),
            hash(_hash){
                
            }
            
            // faster comparison
            bool operator == (const Name& rhs) const {
                return this->hash == rhs.hash;
            }
            
            bool operator < (const Name& rhs) const {
                return this->hash < rhs.hash;
            }
        };

        
    } // namespace reflection
    
}

#endif
