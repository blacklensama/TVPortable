//
//  Font.cpp
//  TVPortable
//
//  Created by Ruiwei Bu on 6/17/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "Font.h"

TVP_NS_VISUAL_BEGIN

    Font* Font::DefaultFont() {
        static Font* font = new Font(TVP_DEFAULT_FONT, 20);
        return font;
    }

    Font::Font(const char* name, int size):
    mFace(name),
    mHeight(size),
    mBold(false),
    mItalic(false),
    mUnderline(false),
    mStrikeout(false),
    mAngle(0) {
        
    }

    Font::Font() {
        
    }

    Font::~Font() {
        
    }

    bool Font::doUserSelect(int flags, const char* caption, const char* prompt, const char* sample) {
        return false;
    }

    float Font::getEscHeightX(const char* text) {
        return 0.f;
    }

    float Font::getEscHeightY(const char* text) {
        return 0.f;
    }

    float Font::getEscWidthX(const char* text) {
        return 0.f;
    }

    float Font::getEscWidthY(const char* text) {
        return 0.f;
    }

    std::string Font::getList(int flags) {
        return "";
    }

    float Font::getTextHeight(const char* text) {
        return 0.f;
    }

    float Font::getTextWidth(const char* text) {
        return 0.f;
    }

    void Font::mapPrerenderedFont(/* fontstorage? */) {
        
    }

    void Font::unmapPrerenderedFont() {
        
    }

TVP_NS_VISUAL_END
