//
//  Font.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/17/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_Font_h
#define TVPortable_Font_h

#include "Prerequisites.h"

TVP_NS_VISUAL_BEGIN

    class Font {
    public:
        Font();
        ~Font();
        
        bool doUserSelect(int flags, const char* caption, const char* prompt, const char* sample);
        
        float getEscHeightX(const char* text);
        float getEscHeightY(const char* text);
        float getEscWidthX(const char* text);
        float getEscWidthY(const char* text);
        
        std::string getList(int flags);
        
        float getTextHeight(const char* text);
        float getTextWidth(const char* text);
        
        void mapPrerenderedFont(/* fontstorage? */);
        void unmapPrerenderedFont();
        
    public:
        TVP_PROPERTY(Angle, float);
        TVP_PROPERTY(Bold, bool);
        TVP_PROPERTY(Face, std::string);
        TVP_PROPERTY(Height, float);
        TVP_PROPERTY(Italic, bool);
        TVP_PROPERTY(Strikeout, bool);
        TVP_PROPERTY(Underline, bool);
    };
        
TVP_NS_VISUAL_END


#endif
