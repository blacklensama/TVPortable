//
//  ObjcWrappers.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/18/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_ObjcWrappers_h
#define TVPortable_ObjcWrappers_h

#include "UKN/platform.h"
#include "Prerequisites.h"

#ifdef UKN_OS_FAMILY_APPLE

TVP_NS_VISUAL_BEGIN

    void* GetApplicationMainMenu();
    void* GetApplicationKeyWindow();
    
    void SetWindowCaption(uintPtr window, const char* title);
    
    void SetWindowSize(uintPtr window, uint32 width, uint32 height); 

TVP_NS_VISUAL_END

#endif

#endif
