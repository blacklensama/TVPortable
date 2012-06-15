//
//  Port.cpp
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/8/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#include "UKN/Port.h"

namespace ukn {
    
    struct PortManager::PortInfo {
        ClassId PortClassId;
        void* PortObj;
        ukn_string PortName;
    };
    
    PortManager& PortManager::Instance() {
        static PortManager instance;
        return instance;
    }
    
    PortManagerResult PortManager::add(const ukn_string& name, void* obj, ClassId cid) {
        PortMap::iterator it = mPorts.find(name);
        if(it != mPorts.end())
            return PMR_Duplicate;
        
        PortInfo info;
        info.PortClassId = cid;
        info.PortObj = obj;
        info.PortName = name;
        
        mPorts[name] = info;
        return PMR_Success;
    }
    
    PortManagerResult PortManager::remove(void* obj) {
        PortMap::iterator it = mPorts.begin();
        while(it != mPorts.end()) {
            if(it->second.PortObj == obj) {
                mPorts.erase(it);
                return PMR_Success;
            }
            ++it;
        }
        return PMR_NotFound;
    }
    
    void* PortManager::attach(const ukn_string& name, ClassId cid) {
        PortMap::iterator it = mPorts.find(name);
        if(it != mPorts.end())
            return it->second.PortObj;
        return 0;
    }
    
    PortManagerResult PortManager::detach(void* obj) {
        // no use
        return PMR_Success;
    }
    
    PortManagerResult PortManager::remove(const ukn_string& name) {
        PortMap::iterator it = mPorts.find(name);
        if(it != mPorts.end()) {
            mPorts.erase(it);
            return PMR_Success;
        }
        return PMR_NotFound;
    }
    
    
} // namespace ukn
