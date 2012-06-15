//
//  Container.h
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/27/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#ifndef Project_Unknown_rf_Container_h
#define Project_Unknown_rf_Container_h

#include "UKN/Interfaces.h"

namespace ukn {
    
    namespace reflection {
        
        struct TypeInfo;
        struct Type;
        
        // read only access
        struct IReadIterator: public virtual Interface {
            virtual ~IReadIterator() { }
            
            virtual const void* value() const = 0;
            virtual size_t      count() const = 0;
            virtual void        next() = 0;
            virtual bool        available() const = 0;
        };
        
        // write only access
        struct IWriteIterator: public virtual Interface {
            virtual ~IWriteIterator() { }
            
            virtual void    add(void* object) = 0;
            virtual void*   addEmpty() = 0;
            virtual bool    available() const = 0;
        };
        
        struct IContainerFactory: public virtual Interface {
            Type* key_type;
            Type* value_type;
            
            bool key_is_pointer;
            bool value_is_pointer;
            
            IContainerFactory():
            key_type(0),
            value_type(0),
            key_is_pointer(0),
            value_is_pointer(0) {
                
            }
            
            virtual ~IContainerFactory() {
                
            }
            
            virtual size_t          getReadIteratorSize() const = 0;
            virtual size_t          getWriteIteratorSize() const = 0;
            // placement new
            virtual IReadIterator*  newReadIterator(void* destMemory, const void* container) = 0;
            virtual IWriteIterator* newWriteIterator(void* destMemory, const void* container) = 0;
            
            virtual void            destructReadIterator(IReadIterator* iterator) = 0;
            virtual void            destructWriteIterator(IWriteIterator* iterator) = 0;
        };
        
        template<typename T, typename READ_ITERATOR, typename WRITE_ITERATOR>
        struct ContainerFactory: public IContainerFactory {
            size_t getReadIteratorSize() const {
                return sizeof(READ_ITERATOR);
            }
            
            size_t getWriteIteratorSize() const {
                return sizeof(WRITE_ITERATOR);
            }
            
            IReadIterator* newReadIterator(void* destMemory, const void* container) {
                return new (destMemory) READ_ITERATOR((T*)container);
            }
            
            IWriteIterator* newWriteIterator(void* destMemory, const void* container) {
                return new (destMemory) WRITE_ITERATOR((T*)container);
            }
            
            void destructReadIterator(IReadIterator* iterator) {
                ((READ_ITERATOR*)iterator)->READ_ITERATOR::~READ_ITERATOR();
            }
            
            void destructWriteIterator(IWriteIterator* iterator) {
                ((WRITE_ITERATOR*)iterator)->WRITE_ITERATOR::~WRITE_ITERATOR();
            }
        };
        
        template<typename TYPE> IContainerFactory* CreateContainerFactory(TYPE&, TypeInfo& value_type) {
            return 0;
        }
        
    } // reflection
    
}

#endif
