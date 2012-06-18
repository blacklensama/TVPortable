//
//  ObjcWrappers.mm
//  TVPortable
//
//  Created by Ruiwei Bu on 6/18/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "ObjcWrappers.h"

#import <Cocoa/Cocoa.h>

namespace TVPortable {
    
#ifdef UKN_OS_OSX
    void* GetApplicationMainMenu() {
        return (void*)[[NSApplication sharedApplication] mainMenu];
    }
    
#else 
    void* GetApplicationMainMenu() {
       
        
    }
    
#endif
    
}