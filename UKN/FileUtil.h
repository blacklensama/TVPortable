//
//  FileUtil.h
//  Project Unknown
//
//  Created by Robert Bu on 11/30/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_FileUtil_h
#define Project_Unknown_FileUtil_h

#include "UKN/Platform.h"
#include "UKN/StringUtil.h"

#include <vector>

namespace ukn {
    
    class File {
    public:
        static bool FileExists(const String& filepath);
        static bool PathExists(const String& path);
    };

    
    class UKN_API Path {
    public:
        static String GetEnv(const ukn_string& env);
        static String GetCurrent();
        static String GetHome();
        static String GetRoot();
        static String GetTemp();
        static String GetFont();
        static String GetWrittable();
        static String GetResource();
        static String GetApplicationPath();
        
        // check and get path of a font
        // priority follows current path, resource path, system font folder, asset resource name
        static String CheckAndGetFontPath(const String& name);
        
        static void ListRoots(std::vector<ukn_string>& arr);
        
        static String ExpandPath(const String& path);
    };
            
    class UKN_API DirectoryIterator {
    public:
        DirectoryIterator();
        DirectoryIterator(const DirectoryIterator& iterator);
        explicit DirectoryIterator(const ukn_string& path);
        explicit DirectoryIterator(const ukn_wstring& path);
        ~DirectoryIterator();
        
        const ukn_string& file() const;
        const ukn_string& path() const;
        
        bool isEnd() const;
        
        DirectoryIterator& operator=(const DirectoryIterator& it);
        
        DirectoryIterator& operator++();
        
        bool operator == (const DirectoryIterator& iterator) const;
        bool operator != (const DirectoryIterator& iterator) const;
        
    private:
        ukn_string mPath;
        ukn_string mFile;
        bool mIsEnd;
        
        class DirectoryIteratorImpl;

        DirectoryIteratorImpl* mImpl;        
    };
    
    inline const ukn_string& DirectoryIterator::path() const {
        return mPath;
    }
    
    inline bool DirectoryIterator::isEnd() const {
        return mIsEnd;
    }
    
    inline const ukn_string& DirectoryIterator::file() const {
        return mFile;
    }
    
    inline bool DirectoryIterator::operator ==(const DirectoryIterator& rhs) const {
        return file() == rhs.file();
    }
    
    inline bool DirectoryIterator::operator !=(const DirectoryIterator& rhs) const {
        return file() != rhs.file();
    }
    
    
} // namespace ukn

#endif
