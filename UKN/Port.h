//
//  Port.h
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/8/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#ifndef Project_Unknown_Port_h
#define Project_Unknown_Port_h

#include "UKN/Platform.h"
#include "UKN/RefCounted.h"

#include <map>

#ifdef UKN_OS_WINDOWS
#include <unordered_map>
#endif

namespace ukn {
    
    enum PortManagerResult {
        PMR_Success,
        PMR_Duplicate,
        PMR_NotFound,
        PMR_WrongType,
    };
    
    typedef uintPtr ClassId;
    
    class PortManager {
    public:
        static PortManager& Instance();
        
        PortManagerResult add(const ukn_string& name,
                              void* obj,
                              ClassId cid);
        
        PortManagerResult remove(void* obj);
        
        void* attach(const ukn_string& name,
                     ClassId cid);
        PortManagerResult detach(void* obj);
    
    private:
        PortManagerResult remove(const ukn_string& name);

        struct PortInfo;

        typedef ukn_hash_map<ukn_string, PortInfo> PortMap;
        PortMap mPorts;
        
    };
    
    // get a unique class id for each class
    template<typename T>
    ClassId GetClassId() {
        typedef ClassId (*ClassIdGetFunc)();
        ClassIdGetFunc func = GetClassId<T>;
        return (ClassId)func;
    }
    
    template<typename T>
    class Port: public RefCounted {
    protected:
        
        Port() {
            
        }
        
        virtual ~Port() {
            
        }
        
    public:
        // forbid creation on stack
        static Port<T>* Create() {
            return new Port<T>();
        }
        
        static PortManagerResult Register(const ukn_string& name,
                                          Port* port) {
            ClassId cid = GetClassId<Port<T> >();
            return PortManager::Instance().add(name,
                                               (void*)port,
                                               cid);
        }
        
        static PortManagerResult DeRegister(Port* port) {
            return PortManager::Instance().remove((void*)port);
        }
        
        static Port<T>* Attach(const ukn_string& name) {
            ClassId cid = GetClassId<Port<T> >();
            Port<T>* obj = static_cast<Port<T>*>(PortManager::Instance().attach(name, cid));
            if(obj)
                obj->duplicate();
            return obj;
        }
        
        static PortManagerResult Detach(Port<T>* port) {
            PortManagerResult result = PortManager::Instance().detach((void*)port);
            if(result == PMR_Success)
                port->release();
            return result;
        }
        
        // register port to PortManager
        // name cannot duplicate
        PortManagerResult registerSelf(const ukn_string& name) {
            return Register(name, this);
        }
        
        PortManagerResult deRegisterSelf() {
            return DeRegister(this);
        }
        
        // data access
        T& data() {
            return mData;
        }
        
        const T& data() const {
            return mData;
        }
        
    private:
        T mData;
    };
    
} // namespace ukn


#endif
