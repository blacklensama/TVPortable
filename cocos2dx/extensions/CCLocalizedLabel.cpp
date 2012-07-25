//
//  CCLocalizedLabelTTF.cpp
//  TVPortable
//
//  Created by Robert Bu on 7/25/12.
//
//

#include "CCLocalizedLabel.h"

#include "CCApplication.h"
#include "CCLocalizedStringTable.h"

namespace cocos2d {
    
    inline const char* _current_language_to_language_name() {
        ccLanguageType lan = CCApplication::getCurrentLanguage();
        switch(lan) {
            case kLanguageChinese: return "zh";
            case kLanguageEnglish: return "en";
            case kLanguageFrench:  return "fr";
            case kLanguageGerman:  return "de";
            case kLanguageItalian: return "it";
            case kLanguageRussian: return "ru";
            case kLanguageSpanish: return "es";
        }
    }
    
    CCLabelTTF* CCLocalizedLabel::LocalizedLabelTTFFromStringId(const std::string& sid, const char* fnt, size_t fontSize) {
        std::string currentLan = _current_language_to_language_name();
        
        std::string realString = sid;
        if(CCLocalizedStringTable::Instance().checkLanguage(currentLan)) {
            realString = CCLocalizedStringTable::Instance().getString(currentLan, sid);
        }
        return CCLabelTTF::labelWithString(realString.c_str(),
                                           fnt,
                                           fontSize);
    }
    
    static CCLabelTTF* LocalizedLabelTTFFromStringId(const std::string& sid, const CCSize& dimensions, CCTextAlignment alignment, const char* fnt, size_t fontSize) {
        std::string currentLan = _current_language_to_language_name();
        
        std::string realString = sid;
        if(CCLocalizedStringTable::Instance().checkLanguage(currentLan)) {
            realString = CCLocalizedStringTable::Instance().getString(currentLan, sid);
        }
        return CCLabelTTF::labelWithString(realString.c_str(),
                                           dimensions,
                                           alignment,
                                           fnt,
                                           fontSize);
    }
    
    CCLabelBMFont* CCLocalizedLabel::LocalizedLabelBMFontFromStringId(const std::string& sid, const char* fnt) {
        std::string currentLan = _current_language_to_language_name();
        
        std::string realString = sid;
        if(CCLocalizedStringTable::Instance().checkLanguage(currentLan)) {
            realString = CCLocalizedStringTable::Instance().getString(currentLan, sid);
        }
        return CCLabelBMFont::labelWithString(realString.c_str(),
                                              fnt);
    }
    
}