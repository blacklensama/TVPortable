//
//  CCInputDispatcher.cpp
//  TVPortable
//
//  Created by Ruiwei Bu on 6/15/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "CCInputDispatcher.h"
#include "CCInputListener.h"

#include <algorithm>

namespace cocos2d {
    
    CCInputDispatcher::CCInputDispatcher() {
    
    }
    
    CCInputDispatcher::~CCInputDispatcher() {
        
    }
    
    CCInputDispatcher& CCInputDispatcher::Instance() {
        static CCInputDispatcher instance;
        return instance;
    }
    
    void CCInputDispatcher::addListener(CCInputListener* listener, int priority) {
        if(listenerExists(listener)) {
            printf("Listener already exists\n");
            return;
        }
        
        listener->setPriority(priority);
        
        if(mListeners.empty())
            mListeners.push_back(listener);
        else {
            ListenerList::iterator it = mListeners.begin();
            while(it != mListeners.end()) {
                if((*it)->getPriority() < priority) {
                    mListeners.insert(it, listener);
                    break;
                }
                
                ++it;
                if(it == mListeners.end()) {
                    mListeners.push_back(listener);
                }
            }
        }
    }
    
    void CCInputDispatcher::removeListener(CCInputListener* listener) {
        ListenerList::iterator it = std::find(mListeners.begin(),
                                              mListeners.end(),
                                              listener);
        if(it != mListeners.end()) {
            mListeners.erase(it);
        }
    }
    
    void CCInputDispatcher::adjustPriority(int priortity, CCInputListener* listener) {
        ListenerList::iterator it = std::find(mListeners.begin(),
                                              mListeners.end(),
                                              listener);
        if(it != mListeners.end()) {
            mListeners.erase(it);
            
            addListener(listener, priortity);
        }
    }
    
    bool CCInputDispatcher::listenerExists(CCInputListener* listener) const {
        return (std::find(mListeners.begin(),
                          mListeners.end(),
                          listener) != mListeners.end());
    }
    
    void CCInputDispatcher::publishMouseEvent(const CCMouseEvent& mouseEvent) {
        if(mListeners.empty())
            return;
        
        ListenerList::const_iterator it = mListeners.begin();
        for(; it != mListeners.end(); ++it) {
            CCInputListener* listener = *it;
            if(listener->onMouseEvent(mouseEvent))
                break;
        }
    }
    
    void CCInputDispatcher::publishKeyboardEvent(const CCKeyEvent& evt) {
        if(mListeners.empty())
            return;
        
        ListenerList::const_iterator it = mListeners.begin();
        for(; it != mListeners.end(); ++it) {
            CCInputListener* listener = *it;
            if(listener->onKeyEvent(evt))
                break;
        }
    }
    
    void CCInputDispatcher::publishJoystickEvent(const CCJoyStickEvent& evt) {
        
        if(mListeners.empty())
            return;
        
        ListenerList::const_iterator it = mListeners.begin();
        for(; it != mListeners.end(); ++it) {
            CCInputListener* listener = *it;
            if(listener->onJoystickEvent(evt))
                break;
        }
    }
    
}