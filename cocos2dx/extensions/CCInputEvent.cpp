//
//  CCInputEvent.cpp
//  TVPortable
//
//  Created by Ruiwei Bu on 6/15/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "CCInputEvent.h"

namespace cocos2d {
    
    bool CCKeyEvent::isKeyDown(CCKey::KeyCode k) const {
        return (k == this->key && state == CCKey::Press);
    }
    
    bool CCKeyEvent::isKeyUp(CCKey::KeyCode k) const {
        return (k == this->key && state == CCKey::Release);
    }
    
    bool CCKeyEvent::isDigit() const {
        return this->key >= 48 && this->key <= 57;
    }
    
    bool CCKeyEvent::isLetter() const {
        return (((this->key >= 65 && this->key <= 90)
                 || (this->key >= 97 && this->key <= 122)
                 || (this->key >= 192 && this->key <= 255))
                && (this->key != 215) && (this->key != 247));
    }
    
    char CCKeyEvent::toCharCode() const {
        if(isPrintable()) {
            return (char)key;
        }
        return '?';
    }
    
    bool CCKeyEvent::isPrintable() const {
        if((this->key >= CCKey::Num0 && key <= CCKey::Num9) ||
           (this->key >= CCKey::A && key <= CCKey::Z) ||
           (this->key == CCKey::Grave) ||
           (this->key == CCKey::Minus) ||
           (this->key == CCKey::Equals) ||
           (this->key == CCKey::Backslash) ||
           (this->key == CCKey::LeftBracket) ||
           (this->key == CCKey::RightBracket) ||
           (this->key == CCKey::Semicolon) ||
           (this->key == CCKey::Apostrophe) ||
           (this->key == CCKey::Comma) ||
           (this->key == CCKey::Period) ||
           (this->key == CCKey::Slash) ||
           (this->key == CCKey::Space)
           )
            return true;
        return false;
    }
    
    bool CCKeyEvent::isShiftFlag() const {
        return this->flag & CCKey::FlagShift;
    }
    
    bool CCKeyEvent::isCtrlFlag() const {
        return this->flag & CCKey::FlagCtrl;
    }
    
    bool CCKeyEvent::isAltFlag() const {
        return this->flag & CCKey::FlagAlt;
    }
    
}