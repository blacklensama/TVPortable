//
//  ArrayContainer.h
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/27/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#ifndef Project_Unknown_ArrayContainer_h
#define Project_Unknown_ArrayContainer_h

#include "UKN/reflection/Container.h"

namespace ukn {
    
    namespace reflection {
        
        template<typename TYPE, size_t LENGTH>
        struct ArrayReadIterator: public IReadIterator {
            ArrayReadIterator(const TYPE* container):
            pointer(container),
            index(0) {
                
            }
            
            const void* value() const {
                return pointer + index;
            }
            
            size_t count() const {
                return LENGTH;
            }
            
            void next() {
                if(index < LENGTH)
                    index++;
            }
            
            bool available() const {
                return index < LENGTH;
            }
            
        private:
            const TYPE* pointer;
            size_t index;
        };
        
        template<typename TYPE, size_t LENGTH>
        struct ArrayWriteIterator: public IWriteIterator {
            ArrayWriteIterator(TYPE* container):
            pointer(container),
            index(0) {
                
            }
            
            void add(void* object) {
                pointer[index++] = *(TYPE*)object;
            }
            
            void* addEmpty() {
                pointer[index] = TYPE();
                return &pointer[index++];
            }
            
            bool available() const {
                return index < LENGTH;
            }
            
        private:
            TYPE* pointer;
            size_t index;
        };
        
        
        template<typename TYPE, size_t LENGTH>
        IContainerFactory* CreateContainerFactory(TYPE (&)[LENGTH], TypeInfo& value_type) {
            value_type = TypeInfo::Create<TYPE>();
            
            return new ContainerFactory<TYPE,
                                        ArrayReadIterator<TYPE, LENGTH>,
                                        ArrayWriteIterator<TYPE, LENGTH> >();
        }
    }
    
}



#endif
