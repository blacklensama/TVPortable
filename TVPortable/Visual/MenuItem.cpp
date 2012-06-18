//
//  MenuItem.cpp
//  TVPortable
//
//  Created by Ruiwei Bu on 6/18/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "MenuItem.h"

#include "UKN/Platform.h"

#if defined(UKN_OS_OSX)
#include "MenuItemImpl_OSX.h"
#elif defined(UKN_OS_WINDOWS)
#include "MenuItemImpl_Win.h"
#else
#include "MenuItemImpl_Null.h"
#endif

TVP_NS_VISUAL_BEGIN

    MenuItem::MenuItem(Window* window, const char* caption):
    mWindow(window),
    mCaption(caption),
    mChecked(false),
    mVisible(true),
    mEnabled(true),
    mRadio(false),
    mGroup(0),
    mIndex(0),
    mShortcut(""),
    mParent(0),
    mMenu(0),
    mListener(0)
    {
        mImpl = new MenuItemImpl;
        mImpl->setParent(this);
        mImpl->initWithWindow(window, caption);
    }

    MenuItem::MenuItem(Window* window, void* native_mainmenu):
    mWindow(window),
    mCaption(""),
    mChecked(false),
    mVisible(true),
    mEnabled(true),
    mRadio(false),
    mGroup(0),
    mIndex(0),
    mShortcut(""),
    mParent(0),
    mMenu(0),
    mListener(0)
    {
        mImpl = new MenuItemImpl;
        mImpl->setParent(this);
        mImpl->initWithNativeMenu(native_mainmenu);
    }

    void MenuItem::add(MenuItem* item) {
        item->mParent = this;
        mChildren.push_back(item);
        
        mImpl->add(item);
    }


    void MenuItem::insert(MenuItem* item, int index) {
        item->mParent = this;
        mChildren.push_back(item);
        
        mImpl->insert(item, index);
    }

    void MenuItem::popup(int flags, int32 x, int32 y) {
        mImpl->popup(flags, x, y);
    }

    void MenuItem::remove(MenuItem* item) {
        item->mParent = NULL;
        mChildren.remove(item);
        
        mImpl->remove(item);
    }

    void MenuItem::setCaption(const char* caption) {
        mCaption = caption;
        
        mImpl->setCaption(caption);
    }

    const char* MenuItem::getCaption() const {
        return mCaption.c_str();
    }

    void MenuItem::setChecked(bool checked) {
        mChecked = checked;
        
        mImpl->setChecked(checked);
    }

    bool MenuItem::getChecked() const {
        return mChecked;
    }

    void MenuItem::setEnabled(bool flag) {
        mEnabled = flag;
        
        mImpl->setEnabled(flag);
    }

    bool MenuItem::getEnabled() const {
        return mEnabled;
    }

    void MenuItem::setIndex(int index) {
        mIndex = index;
        
        mImpl->setIndex(index);
    }

    int MenuItem::getIndex() const {
        return mIndex;
    }

    void MenuItem::setGroup(int group) {
        mGroup = group;
        
        mImpl->setGroup(group);
    }

    int MenuItem::getGroup() const {
        return mGroup;
    }

    void MenuItem::setRadio(bool flag) {
        mRadio = flag;
        
        mImpl->setRadio(flag);
    }

    bool MenuItem::getRadio() const {
        return mRadio;
    }

    void MenuItem::setShortcut(const char* shortcut) {
        mShortcut = shortcut;
        
        mImpl->setShortcut(shortcut);
    }

    const char* MenuItem::getShortcut() const {
        return mShortcut.c_str();
    }

    MenuItem* MenuItem::getRoot() const {
        MenuItem* parent = getParent();
        while(parent && parent->getParent() != NULL)
            parent = parent->getParent();
        return parent;
    }   

    void MenuItem::setVisible(bool flag) {
        mVisible = flag;
        
        mImpl->setVisible(flag);
    }

    bool MenuItem::getVisible() const {
        return mVisible;
    }

    void MenuItem::onClick() {
        if(mListener)
            mListener->onItemClicked(this->getCaption());
    }

    const MenuItem::ChildList& MenuItem::getChildren() const {
        return mChildren;
    }

TVP_NS_VISUAL_END
