//
//  Window.cpp
//  TVPortable
//
//  Created by Ruiwei Bu on 6/17/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "Window.h"
#include "MenuItem.h"

#include "ObjcWrappers.h"
#include "WindowsWrappers.h"

#include "cocos2d.h"


#include "UKN/Platform.h"
#include "UKN/SysUtil.h"

TVP_NS_VISUAL_BEGIN

using namespace cocos2d;

Window* Window::Create(size_t width, size_t height) {
    return new Window(width, height);
}

Window::Window(size_t width, size_t height) {
#ifdef UKN_OS_OSX
    mMenu = new MenuItem(this, GetApplicationMainMenu());
    mNativeHandle = (uintPtr)GetApplicationKeyWindow();

#elif defined(UKN_OS_WINDOWS)
    HWND hWnd = cocos2d::CCEGLView::sharedOpenGLView().getHWnd();
	HMENU hMenu = ::GetMenu(hWnd);
	if(!hMenu) {
		hMenu = ::CreateMenu();
		::SetMenu(hWnd, hMenu);
	}
	mMenu = new MenuItem(this, (void*)hMenu);
    
    mNativeHandle = (uintPtr)hWnd;
	
#elif defined(UKN_OS_IOS)
    mMenu = NULL;
    mNativeHandle = (uintPtr)GetApplicationKeyWindow();
    
#else
    // to do with android/linux etc
    mMenu = NULL;
    mNativeHandle = 0;
    
#endif
    
    mWidth = width;
    mHeight = height;
    
#ifndef UKN_OS_ANDROID
    // ukn currently not implemented on Android
    
    // get max window width & height
    ukn::Array<ukn::SystemInformation::DesktopMode> desktopModes = ukn::SystemInformation::EnumDesktopMode();
    
    mMaxWidth = 0;
    mMaxHeight = 0;
    
    for(int i = 0; i < desktopModes.size(); ++i) {
        ukn::SystemInformation::DesktopMode mode = desktopModes[i];
        if(mMaxWidth < mode.width)
            mMaxWidth = mode.width;
        if(mMaxHeight < mode.height)
            mMaxHeight = mode.height;
    }
    
    printf("TVP initialized with MaxWidth = %d, MaxHeight = %d\n", mMaxWidth, mMaxHeight);
    
#endif
}

Window::~Window() {
    
}

int32 Window::getWidth() const {
    return mWidth;
}

int32 Window::getHeight() const {
    return mHeight;
}

void Window::setWidth(int32 width) {
    setSize(width, mHeight);
}

void Window::setHeight(int32 height) {
    setSize(mWidth, height);
}

void Window::setSize(int32 width, int32 height) {
#ifdef UKN_OS_OSX
    mWidth = width;
    mHeight = height;

    SetWindowSize(mNativeHandle, width, height);
#elif defined(UKN_OS_WINDOWS)
    
    // to do with windows
    mWidth = width;
    mHeight = height;

    
#else
    // cannot change window size on other platforms
    
#endif
    
}

const char* Window::getCaption() const {
    return mCaption.c_str();
}

void Window::setCaption(const char* caption) {
    mCaption = caption;
    
#ifdef UKN_OS_OSX
    SetWindowCaption(mNativeHandle, caption);
    
#elif defined(UKN_OS_WINDOWS)
    
    
#endif
    
}

TVP_NS_VISUAL_END