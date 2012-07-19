//
//  ObjcWrappers.mm
//  TVPortable
//
//  Created by Ruiwei Bu on 6/18/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "ObjcWrappers.h"

#ifdef UKN_OS_OSX

#import <Cocoa/Cocoa.h>

#endif

TVP_NS_VISUAL_BEGIN
    
#ifdef UKN_OS_OSX
    void* GetApplicationMainMenu() {
        return (void*)[[NSApplication sharedApplication] mainMenu];
    }
    
    void* GetApplicationKeyWindow() {
        NSApplication* app = [NSApplication sharedApplication];
                        
        NSWindow* window = [app keyWindow];
        if(window == nil) {
            return [app mainWindow];
        }
        return window;
    }
    
    void SetWindowCaption(uintPtr windowHandle, const char* title) {
        NSWindow* window = (NSWindow*)windowHandle;
        if(!window)
            window = [[NSApplication sharedApplication] keyWindow];
        [window setTitle:[[[NSString alloc] initWithUTF8String:title] autorelease]];
    }
    
    void SetWindowSize(uintPtr windowHandle, uint32 width, uint32 height) {
        NSWindow* window = (NSWindow*)windowHandle;
        if(!window)
            window = [[NSApplication sharedApplication] keyWindow];
        
        NSRect prevFrame = [window frame];
        
        NSRect newFrame = prevFrame;
        newFrame.size.width = width;
        newFrame.size.height = height;
        [window setFrame:newFrame display:YES animate:NO];
    }
    
#else 
    void* GetApplicationMainMenu() {
        return NULL;
    }
    
    void SetWindowCaption(const char* title) {
        // void
    }
    
    void* GetApplicationKeyWindow() {
        return (void*)[[UIApplication sharedApplication] keyWindow];
    }
    
    void SetWindowSize(uintPtr windowHandle, uint32 width, uint32 height) {
        // void
    }
    
#endif
    
TVP_NS_VISUAL_END