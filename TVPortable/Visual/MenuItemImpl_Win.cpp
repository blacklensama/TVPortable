//
//  MenuItemImpl_Win.cpp
//  TVPortable
//
//  Created by Ruiwei Bu on 6/18/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "MenuItemImpl_Win.h"

#include "MenuItem.h"
#include "Window.h"

#include <windows.h>
#include "cocos2d.h"

TVP_NS_VISUAL_BEGIN

	void MenuItemImpl::initWithNativeMenu(void* menu) {
		HMENU hMenu = (HMENU)menu;

		mSubMenuHandle = 0;
		mItem->mMenu = (uintPtr)hMenu;
	}

    void MenuItemImpl::initWithWindow(Window* window, const char*title) {
		HMENU hSubMenu = CreatePopupMenu();
		AppendMenuA(hSubMenu, 
					MF_STRING | MF_POPUP, 
					(UINT)hSubMenu, 
					(LPCSTR)title);

		mSubMenuHandle = (uintPtr)hSubMenu;
		mItem->mMenu = (uintPtr)0;
	}

    void MenuItemImpl::add(MenuItem* item) {
		HMENU menuHandle;
		if(mItem->mMenu == 0)
			menuHandle = (HMENU)mSubMenuHandle;
		else
			menuHandle = (HMENU)mItem->mMenu;
		 
		AppendMenuA(menuHandle, 
					MF_STRING, 
					(uintPtr)item, 
					item->getCaption());
	}

    void MenuItemImpl::insert(MenuItem* item, int index) {
		HMENU menuHandle;
		if(mItem->mMenu == 0)
			menuHandle = (HMENU)mSubMenuHandle;
		else
			menuHandle = (HMENU)mItem->mMenu;

		InsertMenuA(menuHandle,
					index,
					MF_BYPOSITION,
					(uintPtr)item,
					item->getCaption());
	}

    void MenuItemImpl::popup(int flags, int32 x, int32 y) {
		cocos2d::CCDirector::sharedDirector()->pause();

		HMENU menuHandle;
		if(mItem->mMenu == 0)
			menuHandle = (HMENU)mSubMenuHandle;
		else
			menuHandle = (HMENU)mItem->mMenu;

		uint32 button = TrackPopupMenu(menuHandle,
										TPM_RETURNCMD | flags,
										x,
										y,
										0,
										cocos2d::CCEGLView::sharedOpenGLView().getHWnd(),
										0);

		cocos2d::CCDirector::sharedDirector()->resume();
	}

    void MenuItemImpl::remove(MenuItem* item) {
		HMENU menuHandle;
		if(mItem->mMenu == 0)
			menuHandle = (HMENU)mSubMenuHandle;
		else
			menuHandle = (HMENU)mItem->mMenu;

		RemoveMenu(menuHandle,
				   (uintPtr)item,
				   MF_BYCOMMAND);
	}

    void MenuItemImpl::setCaption(const char* caption) {
		HMENU menuHandle;
		if(mItem->mMenu == 0)
			menuHandle = (HMENU)mSubMenuHandle;
		else
			menuHandle = (HMENU)mItem->mMenu;

		AppendMenuA(menuHandle, 
					MF_STRING | MF_POPUP, 
					(UINT)menuHandle, 
					(LPCSTR)caption);
	}

    void MenuItemImpl::setChecked(bool checked) {

	}

    void MenuItemImpl::setEnabled(bool flag) {

	}

    void MenuItemImpl::setIndex(int index) {

	}

    void MenuItemImpl::setGroup(int group) {

	}

    void MenuItemImpl::setRadio(bool flag) {

	}

    void MenuItemImpl::setShortcut(const char* shortcut) {

	}

    void MenuItemImpl::setVisible(bool flag) {

	}

    void MenuItemImpl::setParent(MenuItem* item) {
		mItem = item;
	}
    
    void MenuItemImpl::onClick() {

	}

TVP_NS_VISUAL_END
