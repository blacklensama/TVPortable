//
//  MenuItem.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/17/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_MenuItem_h
#define TVPortable_MenuItem_h

#include "Prerequisites.h"

#include <list>

TVP_NS_VISUAL_BEGIN

    class MenuItemImpl;

    class MenuItemListener {
    public:
        virtual void onItemClicked(const char* caption) { }
    };

    class MenuItem {
        typedef std::list<MenuItem*> ChildList;

    public:
        /* HMENU on Windows; NSMenu* on OSX; currently not availabe on other platforms */
        TVP_PROPERTY_READ_ONLY(Menu, Handle);
        TVP_PROPERTY_READ_ONLY(Parent, MenuItem*);
        TVP_PROPERTY_READ_ONLY(Window, Window*);
        
        TVP_PROPERTY(Listener, MenuItemListener*);
        
    private:
        /* used by Window */
        friend class Window;
        MenuItem(Window* window, void* native_mainmenu);
        
    public:
        MenuItem(Window* window, const char* caption = "");
        
        void add(MenuItem* item);
        void insert(MenuItem* item, int index);
        void popup(int flags, int32 x, int32 y);
        void remove(MenuItem* item);
        MenuItem* getRoot() const;
          
    public:
        void setCaption(const char* caption);
        const char* getCaption() const;
        
        void setChecked(bool checked);
        bool getChecked() const;
        
        void setEnabled(bool flag);
        bool getEnabled() const;
        
        void setIndex(int index);
        int getIndex() const;
        
        /* radio menu item */
        void setGroup(int group);
        int getGroup() const;
        
        void setRadio(bool flag);
        bool getRadio() const;
        
        void setShortcut(const char* shortcut);
        const char* getShortcut() const;
        
        void setVisible(bool flag);
        bool getVisible() const;
        
        const ChildList& getChildren() const;
        
        virtual void onClick();
        
    private:
        friend class MenuItemImpl;
        
        MenuItemImpl* mImpl;
        
        std::string mCaption;
        bool mChecked;
        bool mEnabled;
        bool mRadio;
        bool mVisible;
        
        int mGroup;
        int mIndex;
        std::string mShortcut;
        
        ChildList mChildren;
    };

TVP_NS_VISUAL_END

#endif
