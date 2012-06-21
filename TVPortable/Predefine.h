//
//  Predefine.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/14/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_Predefine_h
#define TVPortable_Predefine_h

#include "Type.h"

#define TVP_PROPERTY(prop, type) \
    private: \
        type m##prop; \
    public: \
        type get##prop() const { return m##prop; } \
        void set##prop(type val) { m##prop = val; } 

#define TVP_PROPERTY_READ_ONLY(prop, type) \
    private: \
        type m##prop; \
    public: \
        type get##prop() const { return m##prop; } \

#define TVP_PROPERTY_GETTER_SETTER(prop, type, accessor) \
    type get##prop() const { return accessor; } \
    void set##prop(type val) { accessor = val; }

#define TVP_DEFAULT_FONT "Courier"

#endif
