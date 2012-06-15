//
//  Uncopyable.h
//  Project Unknown
//
//  Created by Robert Bu on 11/24/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_Uncopyable_h
#define Project_Unknown_Uncopyable_h

namespace ukn {
    
    class Uncopyable {
    public:
        Uncopyable() { }
        
    protected:
        Uncopyable(const Uncopyable&);
        Uncopyable& operator=(const Uncopyable&);
    };
    
} // namespace ukn

#endif
