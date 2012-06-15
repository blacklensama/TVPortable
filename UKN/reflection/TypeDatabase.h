//
//  Class.h
//  Project Unknown
//
//  Created by Robert Bu on 12/5/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_rf_TypeDatabase_h
#define Project_Unknown_rf_TypeDatabase_h

#include "UKN/Platform.h"

#include "UKN/Preprocessor.h"
#include "UKN/Interfaces.h"
#include "UKN/Basic.h"
#include "UKN/reflection/Type.h"

#include "UKN/Singleton.h"

#include <map>
#include <vector>

namespace ukn {
        
    // support POD only
    
    namespace reflection {
        
        class TypeDB: public virtual IEnumerable<Type>, Uncopyable {
        public:
            static TypeDB& Default();

            TypeDB();
            ~TypeDB();
            
            friend class ClassInstantiator<TypeDB>;
            
            template<typename T>
            Type& createType();
            
            template<typename T>
            Type* getType() const;
            
            template<typename T, size_t N>
            void setTypeFields(const FieldInfo (&init_fields)[N]);
            
            Type* getType(const Name& name) const;
            Type* getType(const TypeInfo& info) const;
            
        public:
            // enumerator
            IEnumerator<Type>* createEnumerator() const;
            
        private:
            friend class TypeDBEnumerator;
                        
            typedef std::map<uint32, Type*> TypeMap;
            TypeMap mTypes;
        };
        
        template<typename T>
        Type& TypeDB::createType() {
            Name tname(GetTypeName<T>(), GetTypeId<T>());
            
            Type* type = 0;
            TypeMap::const_iterator itType = mTypes.find(tname.hash);
            if(itType == mTypes.end()) {
                type = new Type(TypeInfo::Create<T>());

                this->mTypes[tname.hash] = type;

            } else {
                type = itType->second;
            }            
            return *type;
        }
        
        template<typename T>
        Type* TypeDB::getType() const {
            Name tname = TypeNameRetriever<typename traits::strip_pointer<T>::type>::Name();
            
            return this->getType(tname);
        }
        
        template<typename T, size_t N>
        void TypeDB::setTypeFields(const FieldInfo (&init_fields)[N]) {
            Type* type = this->getType(GetTypeName<T>());
            if(type != 0) {
                type->defFields(&init_fields[0], N);
            }
        }
        
        template<typename T>
        inline T* FieldCast(const Field* field, void* object) {
            Type* type = TypeDB::Default().getType<T>();
            if(type == field->type) {
                return reinterpret_cast<T*>((char*)object+field->offset);
            }
            return 0;
        }
        
        template<typename T>
        inline T* FieldCast(const Field* field, void* object, const TypeDB& db) {
            Type* type = db.getType<T>();
            if(type == field->type) {
                return reinterpret_cast<T*>((char*)object+field->offset);
            }
            return 0;
        }
        
        template<typename T>
        inline T* UnsafeFieldCast(const Field* field, void* object) {
            return reinterpret_cast<T*>((char*)object+field->offset);
        }
        
#define UKN_RF_BEGIN_TYPE_FIELDS(type) \
        struct TypeFields {    \
            typedef type Type; \
            \
            static void Set() { \
                ukn::reflection::FieldInfo fields[] = { \

#define UKN_RF_TYPE_FIELD(name) ukn::reflection::FieldInfo(#name, &Type::name, offsetof(Type, name))
#define UKN_RF_TYPE_FIELD_F(name, flags) ukn::reflection::FieldInfo(#name, &Type::name, offsetof(Type, name)).defAttr(flags)
#define UKN_RF_TYPE_FIELD_F_D(name, flags, desc) ukn::reflection::FieldInfo(#name, &Type::name, offsetof(Type, name)).defAttr(flags).defDesc(desc)
        
#define UKN_RF_TYPE_FIELD_2(type, name) ukn::reflection::FieldInfo(#name, &type::name, offsetof(type, name))
#define UKN_RF_TYPE_FIELD_2_F(type, name, flags) ukn::reflection::FieldInfo(#name, &type::name, offsetof(type, name)).defAttr(flags)
#define UKN_RF_TYPE_FIELD_2_F_D(type, name, flags, desc) ukn::reflection::FieldInfo(#name, &type::name, offsetof(type, name)).defAttr(flags).defDesc(desc)
        
#define UKN_RF_END_TYPE_FIELDS() \
                }; \
                ukn::reflection::TypeDB& type_db = ukn::reflection::TypeDB::Instance(); \
                type_db.setTypeFields<Type>(fields); \
            } \
            \
            TypeFields() { \
            } \
        }; \
        static TypeFields Fields; \
        
    } // namespace detail
        
} // namespace ukn

#endif