//
//  LocalizedStringTable.h
//  StringEditor
//
//  Created by Ruiwei Bu on 7/19/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef StringEditor_LocalizedStringTable_h
#define StringEditor_LocalizedStringTable_h

#include <map>
#include <vector>
#include <string>
#include <exception>

#include "ukn/Platform.h"

namespace ukn {

    class StringMap {
    public:
        typedef std::map<std::string, std::string> StringMapCont;
        
        std::string stringAtIndex(size_t index) const;
        std::string stringAtId(const std::string& sid) const;
        void setStringAtId(const std::string& sid, const std::string& string);
        void addString(const std::string& sid, const std::string& string);
        void removeStringAtId(const std::string& sid);
        size_t size() const;
        void setStringIdAtId(const std::string& sid, const std::string& after);
        
        typedef StringMapCont::iterator iterator;
        typedef StringMapCont::const_iterator const_iterator;
        
        iterator begin() { return mStringMap.begin(); }
        iterator end() { return mStringMap.end(); }
        
        const_iterator begin() const { return mStringMap.begin(); }
        const_iterator end() const { return mStringMap.end(); }
        
        
    private:
        StringMapCont mStringMap;
    };
    
    class LocalizedStringTable {
    public:
        typedef std::map<std::string /* lan */, StringMap /* StringMap */> LanguageMap;
        
        const StringMap& languageStringMapAtId(const std::string& lanName) const;
        StringMap& languageStringMapAtId(const std::string& lanName);
        
        void addLanguage(const std::string& lanName);

        void removeLanguage(const std::string& lanName);
        void setCurrentLanguage(const std::string& language);
        
        const std::string& getCurrentLanguage() const;
        
        std::string stringById(const std::string& sid) const;
        
        void setStringById(const std::string& sid, const std::string& string);
        void addString(const std::string& sid, const std::string& string);
        void addString(const std::string& lan, const std::string& sid, const std::string& string);

        void removeStringById(const std::string& sid);
        
        std::string getString(const std::string& lan, const std::string& sid);
        void setString(const std::string& lan, const std::string& sid, const std::string& string);
        void setStringIdAtId(const std::string& before, const std::string& after);
        
        std::string getIdByIndex(size_t index) const;
        
        size_t sizeOfLanguages() const;
        size_t sizeOfStrings() const;
        
        void clear();
    
        bool checkLanguage(const std::string& lanName) const;
        bool checkStringId(const std::string& stringId) const;
        
        typedef LanguageMap::iterator iterator;
        typedef LanguageMap::const_iterator const_iterator;
        
        iterator begin() { return mLocalizedString.begin(); }
        iterator end()   { return mLocalizedString.end(); }
        
        const_iterator begin() const { return mLocalizedString.begin(); }
        const_iterator end()   const { return mLocalizedString.end(); }
        
    private:
        friend class LocalizedStringTableLoader;
        void updateStringId(const std::string& before, const std::string& after);
        
        LanguageMap mLocalizedString;
        
        typedef std::vector<std::string> IdVector;
        IdVector mIdVector;
        
        std::string mCurrentLanguage;
    };

} // namespace ukn

#endif
