//
//  CCMacEventDelegate.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/15/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_CCMacEventDelegate_h
#define TVPortable_CCMacEventDelegate_h

#include "CCEventDispatcher.h"
// simple delegate that delivers events to CCInputDispatcher

@interface CCMacEventDelegate: NSObject<CCMouseEventDelegate, CCKeyboardEventDelegate, CCTouchEventDelegate> {
    
}

// CCMouseEventDelegate
-(BOOL) ccMouseDown:(NSEvent*)event;
-(BOOL) ccMouseDragged:(NSEvent*)event;
-(BOOL) ccMouseMoved:(NSEvent*)event;
-(BOOL) ccMouseUp:(NSEvent*)event;
-(BOOL) ccRightMouseDown:(NSEvent*)event;
-(BOOL) ccRightMouseDragged:(NSEvent*)event;
-(BOOL) ccRightMouseUp:(NSEvent*)event;
-(BOOL) ccOtherMouseDown:(NSEvent*)event;
-(BOOL) ccOtherMouseDragged:(NSEvent*)event;
-(BOOL) ccOtherMouseUp:(NSEvent*)event;
- (BOOL)ccScrollWheel:(NSEvent *)theEvent;
- (void)ccMouseEntered:(NSEvent *)theEvent;
- (void)ccMouseExited:(NSEvent *)theEvent;

// CCKeyboardEventDelegate
-(BOOL) ccKeyUp:(NSEvent*)event;
-(BOOL) ccKeyDown:(NSEvent*)event;
-(BOOL) ccFlagsChanged:(NSEvent*)event;

// CCTouchEventDelegate
- (BOOL)ccTouchesBeganWithEvent:(NSEvent *)event;
- (BOOL)ccTouchesMovedWithEvent:(NSEvent *)event;
- (BOOL)ccTouchesEndedWithEvent:(NSEvent *)event;
- (BOOL)ccTouchesCancelledWithEvent:(NSEvent *)event;

@end

#endif
