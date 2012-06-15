//
//  Class.cpp
//  Project Unknown
//
//  Created by Ruiwei Bu on 12/25/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#include "UKN/reflection/TypeDatabase.h"
#include "UKN/Color.h"
#include "UKN/MathUtil.h"

namespace ukn {
        
    namespace reflection {
        
        class TypeDBEnumerator: public IEnumerator<Type> {
        public:
            TypeDBEnumerator(const TypeDB::TypeMap& tmap):
            mTypeMap(tmap) {
                mIndex = 0;
                mCurrent = mTypeMap.begin();
            }
            
            IEnumerator<Type>* clone() const {
                return new TypeDBEnumerator(this->mTypeMap);
            }
            
            const Type& current() const {
                return *mCurrent->second;
            }
            
            intPtr index() const {
                return mIndex;
            }
            
            bool next() {
                if(mCurrent != mTypeMap.end()) {
                    ++mIndex;
                    ++mCurrent;
                    
                    return true;
                }
                return false;
            }
            
            bool available() const {
                return mCurrent != mTypeMap.end();
            }
            
            void reset() {
                mIndex = 0;
                mCurrent = mTypeMap.begin();
            }
            
        private:
            const TypeDB::TypeMap& mTypeMap;
            
            intPtr mIndex;
            TypeDB::TypeMap::const_iterator mCurrent;
        };
        
        TypeDB::TypeDB() {
            createType<int32>();
            createType<uint32>();
            createType<int64>();
            createType<uint64>();
            createType<int16>();
            createType<uint16>();
            createType<int8>();
            createType<uint8>();
            createType<float>();
            createType<double>();
            createType<ukn_string>();
            createType<ukn_wstring>();
            
        }
        
        namespace {
            SingletonHolder<TypeDB, void> instance;
        }
        
        TypeDB& TypeDB::Default() {
            return *instance.get();
        }
        
        TypeDB::~TypeDB() {
            
        }
        
        Type* TypeDB::getType(const Name& name) const {
            TypeMap::const_iterator itType = mTypes.find(name.hash);
            if(itType == mTypes.end()) {
                return 0;
            } else {
                return itType->second;
            }
        }
        
        Type* TypeDB::getType(const TypeInfo& info) const {
            TypeMap::const_iterator itType = mTypes.find(info.name.hash);
            if(itType == mTypes.end()) {
                return 0;
            } else {
                return itType->second;
            }
        }
        
        IEnumerator<Type>* TypeDB::createEnumerator() const {
            return new TypeDBEnumerator(this->mTypes);
        }
        
    } // namespace reflection

}