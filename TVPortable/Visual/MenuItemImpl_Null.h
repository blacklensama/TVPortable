//
//  MenuItemImpl_Null.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/18/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_MenuItemImpl_Null_h
#define TVPortable_MenuItemImpl_Null_h

#include "Prerequisites.h"

TVP_NS_VISUAL_BEGIN

class MenuItemImpl {
public:
    void initWithNativeMenu(void* menu) { }
    void initWithWindow(Window* window, const char*title) { }
    void add(MenuItem* item) { }
    void insert(MenuItem* item, int index) { }
    void popup(int flags, int32 x, int32 y) { }
    void remove(MenuItem* item) { }
    void addToWindow(Window* window) { }
    
public:
    void setCaption(const char* caption) { }
    void setChecked(bool checked) { }
    void setEnabled(bool flag) { }
    void setIndex(int index) { }
    void setGroup(int group) { }
    void setRadio(bool flag) { }
    void setShortcut(const char* shortcut) { }
    void setVisible(bool flag) { }
    void setParent(MenuItem* item) { }
    
public:
    void onClick(void* sender) { }
    
private:
    MenuItem* mItem;
    
    void* mCocoaItemHandle;
    void* mActionReceiver;
};

TVP_NS_VISUAL_END

#endif
