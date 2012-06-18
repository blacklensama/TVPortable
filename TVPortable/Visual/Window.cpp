//
//  Window.cpp
//  TVPortable
//
//  Created by Ruiwei Bu on 6/17/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "Window.h"
#include "MenuItem.h"

#include "UKN/Platform.h"

#include "ObjcWrappers.h"

#include "cocos2d.h"

TVP_NS_VISUAL_BEGIN

Window::Window() {
#ifdef UKN_OS_OSX
    mMenu = new MenuItem(this, GetApplicationMainMenu());
#elif defined(UKN_OS_WINDOWS)
    HWND hWnd = cocos2d::CCEGLView::sharedOpenGLView().getHWnd();
	HMENU hMenu = ::GetMenu(hWnd);
	if(!hMenu) {
		hMenu = ::CreateMenu();
		::SetMenu(hWnd, hMenu);
		AppendMenu(hMenu, MF_STRING, 0, L"Test");
	}
	mMenu = new MenuItem(this, (void*)hMenu);
	
#else
    mMenu = NULL;
#endif
}

Window::~Window() {
    
}

Window* Window::MainWindow() {
    static Window* window = new Window;
    return window;
}

TVP_NS_VISUAL_END