//
//  CCLocalizedStringTable.cpp
//  StringEditor
//
//  Created by Ruiwei Bu on 7/19/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "CCLocalizedStringTable.h"
#include <cassert>

namespace cocos2d {
    
    CCLocalizedStringTable& CCLocalizedStringTable::Instance() {
        static CCLocalizedStringTable* stringtable = new CCLocalizedStringTable();
        return *stringtable;
    }
    
    std::string CCStringMap::stringAtIndex(size_t index) const {
        if(index < mStringMap.size()) {
            CCStringMap::StringMapCont::const_iterator it = mStringMap.begin();
            std::advance(it, index);
            return it->second;
        }
        return "";
    }
    
    std::string CCStringMap::stringAtId(const std::string& sid) const {
        StringMapCont::const_iterator it = mStringMap.find(sid);
        return it != mStringMap.end() ? it->second : "";
    }
    
    void CCStringMap::setStringAtId(const std::string& sid, const std::string& string) {
        StringMapCont::iterator it = mStringMap.find(sid);
        if(it != mStringMap.end()) {
            it->second = string;
        }
    }
    
    void CCStringMap::addString(const std::string& sid, const std::string& string) {
        mStringMap.insert(std::make_pair(sid, string));
    }
    
    void CCStringMap::removeStringAtId(const std::string& sid) {
        StringMapCont::iterator it = mStringMap.find(sid);
        if(it != mStringMap.end()) {
            mStringMap.erase(it);
        }
    }
    
    size_t CCStringMap::size() const {
        return mStringMap.size();
    }
    
    void CCStringMap::setStringIdAtId(const std::string& sid, const std::string& after) {
        StringMapCont::iterator it = mStringMap.find(sid);
        if(it != mStringMap.end()) {
            std::string value = it->second;
            mStringMap.erase(it);
            mStringMap.insert(std::make_pair(after, value));
        }
    }
    
    const CCStringMap& CCLocalizedStringTable::languageStringMapAtId(const std::string& lanName) const {
        LanguageMap::const_iterator it = mLocalizedString.find(lanName);
        if(it != mLocalizedString.end()) {
            return it->second;
        } else {
            assert(0);
        }
    }
    
    CCStringMap& CCLocalizedStringTable::languageStringMapAtId(const std::string& lanName) {
        LanguageMap::iterator it = mLocalizedString.find(lanName);
        if(it != mLocalizedString.end()) {
            return it->second;
        } else {
            assert(0);
        }
    }
    
    void CCLocalizedStringTable::addLanguage(const std::string& lanName) {
        mLocalizedString.insert(std::make_pair(lanName, CCStringMap()));
    }
    
    void CCLocalizedStringTable::removeLanguage(const std::string& lanName) {
        CCLocalizedStringTable::iterator it = mLocalizedString.find(lanName);
        if(it != mLocalizedString.end()) {
            mLocalizedString.erase(it);
        }
    }
    
    void CCLocalizedStringTable::setCurrentLanguage(const std::string& language) {
        mCurrentLanguage = language;
    }
    
    const std::string& CCLocalizedStringTable::getCurrentLanguage() const {
        return mCurrentLanguage;
    }
    
    std::string CCLocalizedStringTable::stringById(const std::string& sid) const {
        if(mCurrentLanguage.empty())
            return "";
        
        const CCStringMap& map = languageStringMapAtId(mCurrentLanguage);
        return map.stringAtId(sid);
    }
    
    void CCLocalizedStringTable::setStringById(const std::string& sid, const std::string& string) {
        if(mCurrentLanguage.empty())
            return;
        
        CCStringMap& map = languageStringMapAtId(mCurrentLanguage);
        map.setStringAtId(sid, string);
    }
    
    void CCLocalizedStringTable::addString(const std::string& sid, const std::string& string) {
        IdVector::const_iterator idIt = std::find(mIdVector.begin(),
                                                  mIdVector.end(),
                                                  sid);
        if(idIt != mIdVector.end())
            return;
        
        for(LanguageMap::iterator it = mLocalizedString.begin(),
            end = mLocalizedString.end();
            it != end;
            ++it) {
            it->second.addString(sid, string);
        }
        mIdVector.push_back(sid);
    }
    
    void CCLocalizedStringTable::removeStringById(const std::string& sid) {
        IdVector::iterator idIt = std::find(mIdVector.begin(),
                                                  mIdVector.end(),
                                                  sid);
        if(idIt != mIdVector.end()) {
            for(LanguageMap::iterator it = mLocalizedString.begin(),
                end = mLocalizedString.end();
                it != end;
                ++it) {
                it->second.removeStringAtId(sid);
            }
            
            mIdVector.erase(idIt);
        }
    }
    
    std::string CCLocalizedStringTable::getString(const std::string& lan, const std::string& sid) {
        CCStringMap& map = languageStringMapAtId(lan);
        return map.stringAtId(sid);
    }
    
    void CCLocalizedStringTable::setString(const std::string& lan, const std::string& sid, const std::string& string) {
        CCStringMap& map = languageStringMapAtId(lan);
        map.setStringAtId(sid, string);
    }
    
    size_t CCLocalizedStringTable::sizeOfLanguages() const {
        return mLocalizedString.size();
    }
    
    size_t CCLocalizedStringTable::sizeOfStrings() const {
        if(mLocalizedString.empty())
            return 0;
        return mLocalizedString.begin()->second.size();
    }
    
    void CCLocalizedStringTable::setStringIdAtId(const std::string& before, const std::string& after) {
        for(LanguageMap::iterator it = mLocalizedString.begin(),
            end = mLocalizedString.end();
            it != end;
            ++it)  {
            it->second.setStringIdAtId(before, after);
        }
        updateStringId(before, after);
    }
    
    std::string CCLocalizedStringTable::getIdByIndex(size_t index) const {
        return mIdVector[index];
    }

    void CCLocalizedStringTable::clear() {
        mLocalizedString.clear();
    }
    
    bool CCLocalizedStringTable::checkLanguage(const std::string& lanName) const {
        return mLocalizedString.find(lanName) == mLocalizedString.end();
    }
    bool CCLocalizedStringTable::checkStringId(const std::string& stringId) const {
        return std::find(mIdVector.begin(),
                         mIdVector.end(),
                         stringId) == mIdVector.end();
    }
    
    void CCLocalizedStringTable::updateStringId(const std::string& before, const std::string& after) {
        for(IdVector::iterator it = mIdVector.begin(),
            end = mIdVector.end();
            it != end;
            ++it) {
            if(*it == before) {
                *it = after;
                break;
            }
        }
    }
    
    void CCLocalizedStringTable::addString(const std::string& lan, const std::string& sid, const std::string& string) {
        LanguageMap::iterator it = mLocalizedString.find(lan);
        if(it != mLocalizedString.end()) {
            it->second.addString(sid, string);
        } else {
            addLanguage(lan);
            LanguageMap::iterator it = mLocalizedString.find(lan);
            it->second.addString(sid, string);
        }
    }
}
