//
//  RefCounted.h
//  Project Unknown
//
//  Created by Robert Bu on 12/17/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_RefCounted_h
#define Project_Unknown_RefCounted_h

#include "UKN/Platform.h"
#include "UKN/Uncopyable.h"

namespace ukn {
    
    class UKN_API RefCounted: Uncopyable {
    public:
        RefCounted(): mCounter(1) { }
        
        void duplicate();
        void release();
        int32 getRefCount() const;
        
    protected:
        virtual ~RefCounted() { }
        
    private:
        mutable int32 mCounter;
    };
    
    inline int32 RefCounted::getRefCount() const {
        return mCounter;
    }
    
    inline void RefCounted::duplicate() {
        ++mCounter;
    }
    
    inline void RefCounted::release() {
        if(--mCounter == 0)
            delete this;
    }
    
} // namespace ukn


#endif
