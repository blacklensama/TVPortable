//
//  CCInputDispatcher.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/15/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_CCInputDispatcher_h
#define TVPortable_CCInputDispatcher_h

#include <list>
#include "CCInputEvent.h"

namespace cocos2d {
    
    class CCInputListener;
    
    /* 
     Simple Singleton dispatcher for input events on desktop
     you should add support by yourself in platform specific codes
     */
    class CCInputDispatcher {
    public:
        // singleton instance
        static CCInputDispatcher& Instance(); 
        
    public:
        void addListener(CCInputListener* listener, int priority);
        void removeListener(CCInputListener* listener);
        
        void adjustPriority(int priority, CCInputListener* listener);
        
        bool listenerExists(CCInputListener* listener) const;
        
        
        void publishMouseEvent(const CCMouseEvent& evt);
        void publishKeyboardEvent(const CCKeyEvent& evt);
        void publishJoystickEvent(const CCJoyStickEvent& evt);
        
    private:
        typedef std::list<CCInputListener*> ListenerList;
        ListenerList mListeners;
        
    protected:
        CCInputDispatcher();
        ~CCInputDispatcher();
    };
    
}


#endif
