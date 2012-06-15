//
//  Field.h
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/27/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#ifndef Project_Unknown_rf_Field_h
#define Project_Unknown_rf_Field_h

#include "UKN/reflection/TypeName.h"
#include "UKN/reflection/TypeInfo.h"
#include "UKN/reflection/Container.h"
#include "UKN/reflection/ArrayContainer.h"

#include "UKN/Ptr.h"

namespace ukn {
    
    namespace reflection {
        
        struct Type;
        
        enum AttributeFlags {
            // ignore during serialization ?
            AF_Transient    = 1UL << 1,
        };
        
        struct FieldAttribute {
            FieldAttribute(uint32 attr):
            attribute(attr) {
                
            }
            
            uint32 attribute;
        };

        struct FieldInfo {
          
            template<typename CLASS, typename FIELD>
            FieldInfo(Name _name, FIELD (CLASS::*field), size_t _offset):
            name(_name),
            type_info(TypeInfo::Create<FIELD>()),
            is_pointer(traits::is_pointer<FIELD>::value),
            offset(_offset),
            attributes(FieldAttribute(0)) {              
                // figure out the field is a container or not
                container = CreateContainerFactory(((CLASS*)0)->*field, container_value_info);
            }
            
            Name     name;
            TypeInfo type_info;
            bool     is_pointer;
            size_t   offset;
            
            // container info
            IContainerFactory* container;
            TypeInfo           container_value_info;
            
            FieldInfo& defAttr(FieldAttribute f) {
                attributes = f;
                return *this;
            }
            
            FieldInfo& defDesc(const char* desc) {
                description = desc;
                return *this;
            }
            
            Name            description;
            FieldAttribute  attributes;
        };
        
        struct Field {
            Field();
            explicit Field(const FieldInfo& info, class TypeDB& db);
            
            // extra attributes
            // attribute flags
            Field& defAttr(FieldAttribute f);            
            Field& defDesc(const char* desc);
            Field& defGroup(const char* group);
            
            // get the actual pointer to the field
            void* getPtr(void* object) const;
            
            // containers, such as vector and CArray
            bool isContainer() const;
            
            SharedPtr<IReadIterator, 
                      SharedPtrFreeReleasePolicy<IReadIterator> >   getReadIterator(void* object) const;
            SharedPtr<IWriteIterator, 
                      SharedPtrFreeReleasePolicy<IWriteIterator> >   getWriteIterator(void* object) const;
            
            Name    name;
            Name    description;
            Name    group;
            
            Type*   type;
            
            bool    is_pointer;
            size_t  offset;
            
            FieldAttribute attributes;
            
            // container factory, if the field is a container
            IContainerFactory* container;
        };
        
    } // namespace reflection
    
}

#endif
