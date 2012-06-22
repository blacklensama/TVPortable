//
//  ListenerManager.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/22/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_ListenerManager_h
#define TVPortable_ListenerManager_h

#include "Predefine.h"

#include <map>
#include <deque>

namespace TVPortable {
    
    // abstract listener manager
    // can manage listeners with custom tag
    template<typename T, typename TAG_TYPE = std::string>
    struct ListenerManager {  
        typedef T ListenerType;
        
        typedef std::map<TAG_TYPE, ListenerType*> ListenerMap;
        typedef std::deque<std::pair<ListenerType*, bool> > ListenerList;
        
        virtual ~ListenerManager() {
            typename ListenerList::iterator it = mListenerList.begin();
            while(it != mListenerList.end()) {
                if(it->second) {
                    delete it->first;
                }
                ++it;
            }
        }
        
        void attachListener(ListenerType* listener, const TAG_TYPE& tag = std::string(), bool autoRelease=false) {
            {
                assert(listener);
                if(!listener)
                    return;
                
                mListenerList.push_back(typename ListenerList::value_type(listener, autoRelease));
                if(!tag.empty()) {
                    typename ListenerMap::const_iterator it = mListenerMap.find(tag);
                    if(it != mListenerMap.end()) {
                        mListenerMap.insert(std::make_pair(tag, listener));
                    }
                }
            }
        }
        
        ListenerType* getListener(const TAG_TYPE& tag) const {
            typename ListenerMap::const_iterator it = mListenerMap.find(tag);
            if(it != mListenerMap.end()) 
                return it->second;
            return NULL;
        }
        
        void publishListenerEvent(void (ListenerType::*Memfun)()) {
            typename ListenerMap::iterator it = mListenerMap.begin();
            for(; it != mListenerMap.end(); ++it) {
                ((it->second)->*Memfun)();
            }
        }
        
        template<typename ARG1>
        void publishListenerEvent(void (ListenerType::*Memfun)(ARG1), ARG1 arg1) {
            typename ListenerMap::iterator it = mListenerMap.begin();
            for(; it != mListenerMap.end(); ++it) {
                ((it->second)->*Memfun)(arg1);
            }
        }
        
        template<typename ARG1, typename ARG2>
        void publishListenerEvent(void (ListenerType::*Memfun)(ARG1, ARG2), ARG1 arg1, ARG2 arg2) {
            typename ListenerMap::iterator it = mListenerMap.begin();
            for(; it != mListenerMap.end(); ++it) {
                ((it->second)->*Memfun)(arg1, arg2);
            }
        }
        
        template<typename ARG1, typename ARG2, typename ARG3>
        void publishListenerEvent(void (ListenerType::*Memfun)(ARG1, ARG2, ARG3), ARG1 arg1, ARG2 arg2, ARG3 arg3) {
            typename ListenerMap::iterator it = mListenerMap.begin();
            for(; it != mListenerMap.end(); ++it) {
                ((it->second)->*Memfun)(arg1, arg2, arg3);
            }
        }
        
        template<typename ARG1, typename ARG2, typename ARG3, typename ARG4>
        void publishListenerEvent(void (ListenerType::*Memfun)(ARG1, ARG2, ARG3, ARG4), ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4) {
            typename ListenerMap::iterator it = mListenerMap.begin();
            for(; it != mListenerMap.end(); ++it) {
                ((it->second)->*Memfun)(arg1, arg2, arg3, arg4);
            }
        }
        
    protected:
        ListenerMap mListenerMap;
        
        ListenerList mListenerList;
    };
    
}

#endif
