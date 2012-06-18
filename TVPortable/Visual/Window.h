//
//  Window.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/17/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_Window_h
#define TVPortable_Window_h

#include "Prerequisites.h"

TVP_NS_VISUAL_BEGIN

    class Window {
    public:
        Window();
        ~Window();
        
        TVP_PROPERTY(Menu, MenuItem*);
        
    public:
        static Window* MainWindow();
    };

TVP_NS_VISUAL_END

#endif
