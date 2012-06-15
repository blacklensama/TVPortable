//
//  CCInputListener.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/15/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_CCInputListener_h
#define TVPortable_CCInputListener_h

#include "CCInputEvent.h"

namespace cocos2d {
    
    /*
     Simple listener class
     */
    
    class CCInputListener {
    public:
        CCInputListener():mPriority(0) { }
        virtual ~CCInputListener() { }
        
        virtual bool onMouseEvent(const CCMouseEvent& evt) {
            return false;
        }
        virtual bool onKeyEvent(const CCKeyEvent& evt) { 
            return false;
        }
        virtual bool onJoystickEvent(const CCJoyStickEvent& evt) {
            return false;
        }
   
        int getPriority() const { return mPriority; }
    
    private:     
        void setPriority(int priority) { mPriority = priority; }
        friend class CCInputDispatcher;
        
        int mPriority;
    };
   
} // namespace cocos2d;

#endif
