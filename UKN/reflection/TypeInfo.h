//
//  TypeInfo.h
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/27/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#ifndef Project_Unknown_rf_TypeInfo_h
#define Project_Unknown_rf_TypeInfo_h

#include "UKN/detail/TypeTraits.h"

namespace ukn {
    
    namespace reflection {
        
        // placement new
        template<typename T> 
        struct ObjectContructor {
            static void ConstructObject(void* obj) {
                new (obj) T;
            }   
        };
        
        template<typename T> 
        struct ObjectDestructor {
            static void DestructObject(void* obj) {
                ((T*)obj)->T::~T();
            }   
        };
        
        template<typename T, size_t LENGTH> 
        struct ObjectDestructor<T[LENGTH]> {
            static void DestructObject(void* obj) {
                for(size_t i=0; i<LENGTH; ++i)
                    (((T*)obj)+i)->T::~T();
            }   
        };
        
        typedef void (*ConstructObjectFunc)(void*);
        typedef void (*DestructObjectFunc)(void*);
        
        struct TypeInfo {
            
            template<typename T>
            static TypeInfo Create() {
                TypeInfo info;
                
                info.name           = GetTypeName<typename traits::strip_pointer<T>::type>();
                info.size           = sizeof(T);
                info.is_pointer     = traits::is_pointer<T>::value;
                info.constructor    = ObjectContructor<T>::ConstructObject;
                info.destructor     = ObjectDestructor<T>::DestructObject;
                info.is_pod         = traits::is_pod<T>::value;
                
                return info;
            };
            
            Name    name;
            size_t  size;
            bool    is_pointer;
            bool    is_pod;
            
            ConstructObjectFunc constructor;
            DestructObjectFunc  destructor;
            
            bool operator == (const TypeInfo& rhs) const {
                return (this->name == rhs.name &&
                        this->size == rhs.size &&
                        this->is_pointer == rhs.is_pointer);
            }
            
            bool operator != (const TypeInfo& rhs) const {
                return !(*this == rhs);
            }
        };
        
    } // namespace reflection
    
}

#endif
