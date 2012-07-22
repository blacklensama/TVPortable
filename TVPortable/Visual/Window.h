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
    protected:
        // since there's no multiple window support on some platforms, we just limit max window count to 1
        Window(size_t width, size_t height);
        ~Window();
        
    public:
        TVP_PROPERTY(Menu, MenuItem*);
        TVP_PROPERTY_READ_ONLY(NativeHandle, uintPtr);
        
        TVP_PROPERTY_READ_ONLY(MaxWidth, int32);
        TVP_PROPERTY_READ_ONLY(MaxHeight, int32);
        
    public:
        int32 getWidth() const;
        int32 getHeight() const;
        void setWidth(int32 width);
        void setHeight(int32 height);
        void setSize(int32 width, int32 height);
        
        const char* getCaption() const;
        void setCaption(const char* caption);
        
    public:
        static Window* Create(size_t width, size_t height);
        
    private:
        int32 mWidth;
        int32 mHeight;
        std::string mCaption;
    };

TVP_NS_VISUAL_END

#endif