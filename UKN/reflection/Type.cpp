//
//  ReflectionType.cpp
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/27/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#include "UKN/reflection/Type.h"
#include "UKN/reflection/TypeDatabase.h"

namespace ukn {
    
    namespace reflection {
        
        Type::Type(const TypeInfo& info):
        name(info.name),
        size(info.size),
        is_pointer(info.is_pointer),
        is_pod(info.is_pod),
        constructor(info.constructor),
        destructor(info.destructor),
        mBaseTypes(0) {
            
        }
        
        const Field* Type::getField(const Name& name) const {
            FieldMap::const_iterator it = mFields.find(name.hash);
            if(it != mFields.end())
                return &it->second;
            
            if(mBaseTypes)
                return mBaseTypes->getField(name);
            
            return 0;
        }
        
        Type& Type::defFields(const FieldInfo* fields, size_t size, TypeDB& db) {
            for(int i = 0; i < size; ++i) {
                const FieldInfo& f = fields[i];
                
                this->mFields[f.name.hash] = Field(f, db);
            }
            return *this;
        }
        
        Type& Type::defBase(Type* type) {
            this->mBaseTypes = type;
            return *this;
        }
        
        Type* Type::getBase() const {
            return mBaseTypes;
        }
        
    } // namesapce reflection
    
}
