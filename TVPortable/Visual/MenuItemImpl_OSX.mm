//
//  MenuItemImpl_OSX.mm
//  TVPortable
//
//  Created by Ruiwei Bu on 6/18/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "MenuItemImpl_OSX.h"
#include "MenuItem.h"
#include "Window.h"
#include "ObjcWrappers.h"
#include "cocos2d.h"

#import <Cocoa/Cocoa.h>

@interface ActionReceiver: NSObject {
    TVPortable::Visual::MenuItemImpl* itemImpl;
}
- (id)initWithItemImpl:(TVPortable::Visual::MenuItemImpl*)item;
- (void)onClick;

@end

@implementation ActionReceiver

- (id)initWithItemImpl:(TVPortable::Visual::MenuItemImpl*)item {
    [super init];
    
    itemImpl = item;
    return self;
}

- (void)onClick {
    itemImpl->onClick();
}

@end


TVP_NS_VISUAL_BEGIN

#ifdef UKN_OS_OSX 

    void MenuItemImpl::initWithWindow(Window* window, const char* title) {
        mActionReceiver = new ActionReceiver;
        
        NSMenuItem* nsitem = [[NSMenuItem alloc] 
                              initWithTitle:[[[NSString alloc] 
                                              initWithUTF8String:title] autorelease]
                              action: nil 
                              keyEquivalent:@""];
        [nsitem setEnabled:YES];
        
        [nsitem setTarget:[[ActionReceiver alloc] initWithItemImpl:this]];
        [nsitem setAction:@selector(onClick)];
        
        mCocoaItemHandle = nsitem;
        
        mItem->mMenu = 0;
    }

    void MenuItemImpl::initWithNativeMenu(void* menu) {
        NSMenu* mm = (NSMenu*)mm;
        mItem->mMenu = (uintPtr)menu;
        
        mActionReceiver = NULL;
        mCocoaItemHandle = NULL;
    }

    void MenuItemImpl::add(MenuItem* item) {
        if(mItem->mMenu == 0) {
            NSMenu* subMenu = [[NSMenu alloc] initWithTitle:[[[NSString alloc] 
                                                              initWithUTF8String:mItem->getCaption()] autorelease]];
            [(NSMenuItem*)mCocoaItemHandle setSubmenu:subMenu];
            
            mItem->mMenu = (uintPtr)subMenu;
        }
        NSMenu* menu = (NSMenu*)mItem->getMenu();
        [menu setAutoenablesItems:YES];
        
        NSMenuItem* nsItem = (NSMenuItem*)item->mImpl->mCocoaItemHandle;
        [menu addItem:nsItem];
        [nsItem setEnabled:YES];
        
        mItem->mIndex = [menu indexOfItem:nsItem];
    }

    void MenuItemImpl::insert(MenuItem* item, int index) {
        if(mItem->mMenu == 0) {
            NSMenu* subMenu = [[NSMenu alloc] initWithTitle:[[[NSString alloc] 
                                                              initWithUTF8String:mItem->getCaption()] autorelease]];
            [(NSMenuItem*)mCocoaItemHandle setSubmenu:subMenu];
            
            mItem->mMenu = (uintPtr)subMenu;
        }
        NSMenu* menu = (NSMenu*)mItem->getMenu();
        
        NSMenuItem* nsItem = (NSMenuItem*)item->mImpl->mCocoaItemHandle;
        [menu addItem:nsItem];
        [nsItem setEnabled:YES];
        
        mItem->mIndex = [menu indexOfItem:nsItem];
    }

    void MenuItemImpl::popup(int flags, int32 x, int32 y) {
        // to do,
        // instead of get window directly
        // convert this to something like mItme->getWindow().getHandle()
        int windowNumber = [[[NSApplication sharedApplication] keyWindow] windowNumber];
        
        NSPoint wp = {x, y}; 
        
        NSEvent* event = [NSEvent otherEventWithType:NSApplicationDefined
                                            location:wp
                                       modifierFlags:NSApplicationDefined 
                                           timestamp: (NSTimeInterval) 0
                                        windowNumber: windowNumber
                                             context: [NSGraphicsContext currentContext]
                                             subtype: 0
                                               data1: 0
                                               data2: 0]; 
        cocos2d::CCDirector::sharedDirector()->pause();
        
        [NSMenu popUpContextMenu:(NSMenu*)mItem->getMenu() 
                       withEvent:event 
                         forView:[[NSApplication sharedApplication] keyWindow].contentView];
        
        cocos2d::CCDirector::sharedDirector()->resume();
        
    }

    void MenuItemImpl::remove(MenuItem* item) {
        NSMenu* menu = (NSMenu*)mItem->getMenu();
        
        if(menu)
            [menu removeItem:(NSMenuItem*)item->mImpl->mCocoaItemHandle];
    }

    void MenuItemImpl::setCaption(const char* caption) {
        if(!mCocoaItemHandle)
            return;
        
        NSMenuItem* item = (NSMenuItem*)mCocoaItemHandle;
        [item setTitle:[[[NSString alloc] initWithUTF8String:caption] autorelease]];
    }

    void MenuItemImpl::setChecked(bool checked) {
        if(!mCocoaItemHandle)
            return;
        
        NSMenuItem* item = (NSMenuItem*)mCocoaItemHandle;

        /**
         osx also need "on/off" images to show to check?
         **/
        if(checked) {
            [item setState:NSOnState];
        }
        else {
            [item setState:NSOffState];
        }
    }

    void MenuItemImpl::setEnabled(bool flag) {
        if(!mCocoaItemHandle)
            return;
        
        NSMenuItem* item = (NSMenuItem*)mCocoaItemHandle;

        if(flag)
            [item setEnabled:YES];
        else 
            [item setEnabled:NO];
    }

    void MenuItemImpl::setIndex(int index) {
        if(!mCocoaItemHandle)
            return;
        
        NSMenuItem* item = (NSMenuItem*)mCocoaItemHandle;

        NSMenu* parentMenu = (NSMenu*)mItem->getParent()->mMenu;
        if(parentMenu) {
            [item retain];
            
            [parentMenu removeItem:item];
            [parentMenu insertItem:item atIndex:index];
            
            mItem->mIndex = [parentMenu indexOfItem:item];
            
            [item release];
        }
    }

    void MenuItemImpl::setGroup(int group) {
        
    }

    void MenuItemImpl::setRadio(bool flag) {
        if(!mCocoaItemHandle)
            return;
    }

    void MenuItemImpl::setShortcut(const char* shortcut) {

    }

    void MenuItemImpl::setVisible(bool flag) {
        if(!mCocoaItemHandle)
            return;
        
        NSMenuItem* item = (NSMenuItem*)mCocoaItemHandle;

        if(flag)
            [item setHidden:NO];
        else {
            [item setHidden:YES];
        }
    }

    void MenuItemImpl::doRadioChecked() {
        if(mItem->mRadio) {
            setChecked(true);
            
            MenuItem::ChildList& friends = mItem->getParent()->mChildren;
            MenuItem::ChildList::iterator it = friends.begin();
            
            for(; it != friends.end(); ++it) {
                MenuItem* friend_item = (*it);
                if(friend_item->getRadio() && 
                   friend_item->getGroup() == mItem->mGroup &&
                   friend_item != mItem) {
                    friend_item->mImpl->setChecked(false);
                }
            }
        }
    }

    void MenuItemImpl::setParent(MenuItem* item) {
        mItem = item;
    }

    void MenuItemImpl::onClick() {
        mItem->onClick();
        
        if(mItem->mRadio)
            doRadioChecked();
    }

#endif


TVP_NS_VISUAL_END
