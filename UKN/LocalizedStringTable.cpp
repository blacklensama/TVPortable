//
//  LocalizedStringTable.cpp
//  StringEditor
//
//  Created by Ruiwei Bu on 7/19/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "ukn/LocalizedStringTable.h"
#include "ukn/Exception.h"
#include "ukn/Logger.h"

namespace ukn {
    
    std::string StringMap::stringAtIndex(size_t index) const {
        if(index < mStringMap.size()) {
            StringMap::StringMapCont::const_iterator it = mStringMap.begin();
            std::advance(it, index);
            return it->second;
        }
        return "";
    }
    
    std::string StringMap::stringAtId(const std::string& sid) const {
        StringMapCont::const_iterator it = mStringMap.find(sid);
        return it != mStringMap.end() ? it->second : "";
    }
    
    void StringMap::setStringAtId(const std::string& sid, const std::string& string) {
        StringMapCont::iterator it = mStringMap.find(sid);
        if(it != mStringMap.end()) {
            it->second = string;
        }
    }
    
    void StringMap::addString(const std::string& sid, const std::string& string) {
        mStringMap.insert(std::make_pair(sid, string));
    }
    
    void StringMap::removeStringAtId(const std::string& sid) {
        StringMapCont::iterator it = mStringMap.find(sid);
        if(it != mStringMap.end()) {
            mStringMap.erase(it);
        }
    }
    
    size_t StringMap::size() const {
        return mStringMap.size();
    }
    
    void StringMap::setStringIdAtId(const std::string& sid, const std::string& after) {
        StringMapCont::iterator it = mStringMap.find(sid);
        if(it != mStringMap.end()) {
            std::string value = it->second;
            mStringMap.erase(it);
            mStringMap.insert(std::make_pair(after, value));
        }
    }
    
    const StringMap& LocalizedStringTable::languageStringMapAtId(const std::string& lanName) const {
        LanguageMap::const_iterator it = mLocalizedString.find(lanName);
        if(it != mLocalizedString.end()) {
            return it->second;
        } else {
            throw Exception("cannot find language name");
        }
    }
    
    StringMap& LocalizedStringTable::languageStringMapAtId(const std::string& lanName) {
        LanguageMap::iterator it = mLocalizedString.find(lanName);
        if(it != mLocalizedString.end()) {
            return it->second;
        } else {
            throw Exception("cannot find language name");
        }
    }
    
    void LocalizedStringTable::addLanguage(const std::string& lanName) {
        mLocalizedString.insert(std::make_pair(lanName, StringMap()));
    }
    
    void LocalizedStringTable::removeLanguage(const std::string& lanName) {
        LocalizedStringTable::iterator it = mLocalizedString.find(lanName);
        if(it != mLocalizedString.end()) {
            mLocalizedString.erase(it);
        }
    }
    
    void LocalizedStringTable::setCurrentLanguage(const std::string& language) {
        mCurrentLanguage = language;
    }
    
    const std::string& LocalizedStringTable::getCurrentLanguage() const {
        return mCurrentLanguage;
    }
    
    std::string LocalizedStringTable::stringById(const std::string& sid) const {
        if(mCurrentLanguage.empty())
            return "";
        
        const StringMap& map = languageStringMapAtId(mCurrentLanguage);
        return map.stringAtId(sid);
    }
    
    void LocalizedStringTable::setStringById(const std::string& sid, const std::string& string) {
        if(mCurrentLanguage.empty())
            return;
        
        StringMap& map = languageStringMapAtId(mCurrentLanguage);
        map.setStringAtId(sid, string);
    }
    
    void LocalizedStringTable::addString(const std::string& sid, const std::string& string) {
        
        IdVector::iterator idIt = std::find(mIdVector.begin(),
                                            mIdVector.end(),
                                            sid);
        if(idIt != mIdVector.end()) {
            log_error(format_string("ukn::LocalizedStringTable::addString: string id %s already exist", sid.c_str()));
            return;
        }
        
        for(LanguageMap::iterator it = mLocalizedString.begin(),
            end = mLocalizedString.end();
            it != end;
            ++it) {
            it->second.addString(sid, string);
        }
        mIdVector.push_back(sid);
    }
    
    void LocalizedStringTable::removeStringById(const std::string& sid) {
        
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
        } else {
            log_error(format_string("ukn::LocalizedStringTable::removeStringById: string id %s already exist", sid.c_str()));
        }
        
    }
    
    std::string LocalizedStringTable::getString(const std::string& lan, const std::string& sid) {
        if(checkLanguage(lan)) {
            StringMap& map = languageStringMapAtId(lan);
            return map.stringAtId(sid);
        }
        return "";
    }
    
    void LocalizedStringTable::setString(const std::string& lan, const std::string& sid, const std::string& string) {
        if(checkLanguage(lan)) {
            StringMap& map = languageStringMapAtId(lan);
            map.setStringAtId(sid, string);
        }
    }
    
    size_t LocalizedStringTable::sizeOfLanguages() const {
        return mLocalizedString.size();
    }
    
    size_t LocalizedStringTable::sizeOfStrings() const {
        if(mLocalizedString.empty())
            return 0;
        return mLocalizedString.begin()->second.size();
    }
    
    void LocalizedStringTable::setStringIdAtId(const std::string& before, const std::string& after) {
        IdVector::const_iterator idIt = std::find(mIdVector.begin(),
                                                  mIdVector.end(),
                                                  before);
        if(idIt != mIdVector.end()) {
            for(LanguageMap::iterator it = mLocalizedString.begin(),
                end = mLocalizedString.end();
                it != end;
                ++it)  {
                it->second.setStringIdAtId(before, after);
            }
            updateStringId(before, after);
        } else {
            log_error(format_string("ukn::LocalizedStringTable::setStringIdAtId: string id %s already exist", before.c_str()));
        }
        
    }
    
    std::string LocalizedStringTable::getIdByIndex(size_t index) const {
        return mIdVector[index];
    }

    void LocalizedStringTable::clear() {
        mLocalizedString.clear();
    }
    
    bool LocalizedStringTable::checkLanguage(const std::string& lanName) const {
        return mLocalizedString.find(lanName) == mLocalizedString.end();
    }
    bool LocalizedStringTable::checkStringId(const std::string& stringId) const {
        return std::find(mIdVector.begin(),
                         mIdVector.end(),
                         stringId) == mIdVector.end();
    }
    
    void LocalizedStringTable::updateStringId(const std::string& before, const std::string& after) {
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
    
    void LocalizedStringTable::addString(const std::string& lan, const std::string& sid, const std::string& string) {
        StringMap& map = languageStringMapAtId(lan);
        map.addString(sid, string);
    }
}
