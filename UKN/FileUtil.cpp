//
//  FileUtil.cpp
//  Project Unknown
//
//  Created by Robert Bu on 11/30/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#include "UKN/Platform.h"
#include "UKN/FileUtil.h"

#ifdef UKN_OS_WINDOWS
#include <Windows.h>
#include "Shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

#elif defined(UKN_OS_FAMILY_APPLE)
#include "AppleStuff.h"
#include <sys/stat.h>
#else
#include <sys/stat.h>
#endif // UKN_OS_FAMILTY_APPLe

#ifdef UKN_OS_FAMILY_UNIX   
//#include <pwd.h>
#endif // UKN_OS_FAMILTY_UNIX

#include <cstdio>
#include <fstream>

#include "UKN/StringUtil.h"

namespace ukn {
    
    bool File::FileExists(const String& filepath) {
#ifdef UKN_OS_WINDOWS
        return PathFileExistsW(filepath.c_str())?true:false;
        
#elif defined(UKN_OS_FAMILY_APPLE)
        return ukn_apple_file_exists(filepath);
#else
        
        struct stat sb;
        
        if (stat(String::WStringToString(filepath).c_str(), &sb) == 0 && S_ISREG(sb.st_mode)) {
            return true;
        }
#endif
        return false;
    }
    
    bool File::PathExists(const String& path) {
#ifdef UKN_OS_WINDOWS
        return PathFileExistsW(path.c_str())?true:false;

#else
        struct stat sb;
        
        if (stat(String::WStringToString(path).c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
            return true;
        }
#endif
        
        return false;
    }
    
    String Path::GetApplicationPath() {
#ifdef UKN_OS_WINDOWS
        wchar_t buffer[MAX_PATH];
        GetCurrentDirectoryW(MAX_PATH, buffer);
        
        return String(buffer)+L"/";
        
#elif defined(UKN_OS_FAMILY_APPLE)
        return ukn_apple_application_path() + L"/";
        
#endif
        return L"./";
    }
    
    String Path::CheckAndGetFontPath(const String& name) {
        
        if(File::FileExists(L"./"+name))
            return L"./"+name;
        
        // resource path
        {
            String fullpath(Path::GetResource() + name);
            
            if(File::FileExists(fullpath)) {
                return fullpath;
            }
        }
        
        // system font path
        {
            String fullpath(Path::GetFont() + name);
        
            if(File::FileExists(fullpath)) {
                return fullpath;
            }
        }
        
        // document path for ios and osx
        {
#if defined(UKN_OS_FAMILY_APPLE)
            String fullpath(ukn_apple_documents_path());
            
            if(File::FileExists(fullpath)) {
                return fullpath;
            }
#endif
        }
        
        return String();
    }
    
    String Path::GetEnv(const ukn_string& env) {
#if defined(UKN_OS_WINDOWS)
        DWORD len = GetEnvironmentVariableA(env.c_str(), 0, 0);
        if (len != 0) {
            char* buffer = new char[len];
            GetEnvironmentVariableA(env.c_str(), buffer, len);
            ukn_string result(buffer);
            delete [] buffer;
            return result;
        }
        
#elif defined(UKN_OS_FAMILY_UNIX)
        return String::StringToWStringFast(getenv(env.c_str()));
#endif
        
        return String();
    }
    
    String Path::GetCurrent() {
#if defined(UKN_OS_WINDOWS)
        wchar_t buffer[_MAX_PATH];
        DWORD n = GetCurrentDirectoryW(sizeof(buffer), buffer);
        if(n > 0 && n < sizeof(buffer)) {
            ukn_wstring result(buffer, n);
            if(result[n-1] != L'\\')
                result.append(L"\\");
            return result;
        }
        
#elif defined(UKN_OS_FAMILY_UNIX)
        ukn_string path;
        char cwd[PATH_MAX];
        if(getcwd(cwd, sizeof(cwd)))
            path = cwd;
        
        size_t n = path.size();
        if(n > 0 && path[n-1] != '/')
            path.append("/");
        return String::StringToWStringFast(path);
#endif      
    }
    
    String Path::GetHome() {
#if defined(UKN_OS_WINDOWS)
        String result = Path::GetEnv("HOMEDRIVE");
        result.append(Path::GetEnv("HOMEPATH"));
       
        size_t n = result.size();
        if(n > 0 && result[n-1] != L'\\')
            result.append(L"\\");
        return result;
        
#elif defined(UKN_OS_FAMILY_UNIX)
        ukn_string path;
     /*   struct passwd* pwd = getpwuid(getuid());
        if(pwd)
            path = pwd->pw_dir;
        else {
            pwd = getpwuid(getuid());
            if(pwd) 
                path = pwd->pw_dir;
            else */
                return Path::GetEnv("HOME") + L"/";
       // }
        
        size_t n = path.size();
        if(n > 0 && path[n-1] != '/')
            path.append("/");
        return String::StringToWStringFast(path);
#endif           
    }
    
    String Path::GetRoot() {
#if defined(UKN_OS_WINDOWS)
        
#elif defined(UKN_OS_FAMILY_UNIX)
        ukn_string path;
        char* tmp = getenv("TMPDIR");
        if(tmp) {
            path = tmp;
            size_t n = path.size();
            if(n > 0 && path[n-1] != '/')
                path.append("/");
            else 
                path = "/tmp/";
        }
        return String::StringToWStringFast(path);
#endif       
        return String();
    }
    
    String Path::GetTemp() {
#if defined(UKN_OS_WINDOWS)
        wchar_t buffer[_MAX_PATH];
        DWORD n = GetTempPathW(sizeof(buffer), buffer);
        if(n > 0 && n < sizeof(buffer)) {
            ukn_wstring result(buffer, n);
            if(result[n-1] != L'\\')
                result.append(L"\\");
            return result;
        } 
        
#elif defined(UKN_OS_FAMILY_UNIX)
        std::string path;
        char* tmp = getenv("TMPDIR");
        if(tmp) {
            path = tmp;
            std::string::size_type n = path.size();
            if(n > 0 && path[n-1] != '/')
                path.append("/");
            else 
                path = "/tmp/";
        }
        return String::StringToWStringFast(path);
        
#endif    
        return String();
    }
    
    String Path::GetWrittable() {
#if defined(UKN_OS_WINDOWS)
        return L"./";
        
#elif defined(UKN_OS_FAMILY_APPLE)
        return ukn_apple_documents_path();
        
#elif defined(UKN_OS_FAMILY_UNIX)
        return L"./";
#endif
    }
    
    String Path::GetResource() {
#if defined(UKN_OS_WINDOWS)
        return L"./";
        
#elif defined(UKN_OS_FAMILY_APPLE)
        
#ifdef UKN_OS_OSX
        return GetApplicationPath() + L"/Contents/Resources/";
#else
        return GetApplicationPath() + L"./";
#endif
        
#elif defined(UKN_OS_FAMILY_UNIX)
        return L"./";
#endif  
    }
    
    String Path::GetFont() {
#if defined(UKN_OS_WINDOWS)
        wchar_t buffer[_MAX_PATH];
        GetWindowsDirectoryW(buffer, _MAX_PATH-1);
        ukn_wstring path(buffer);
        return path + L"\\Fonts\\";
        
#elif defined(UKN_OS_FAMILY_APPLE)
        return L"/Library/Fonts/";
        
#elif defined(UKN_OS_LINUX)
        return L"/usr/share/fonts/";
        
#endif
        return L"./";
    }
    
    void Path::ListRoots(std::vector<ukn_string>& arr) {
        arr.clear();
        
#if defined(UKN_OS_WINDOWS)
        char buffer[128];
        DWORD n = GetLogicalDriveStringsA(sizeof(buffer)-1, buffer);
        char* it = buffer;
        char* end = buffer + (n > sizeof(buffer) ? sizeof(buffer): n);
        while(it < end) {
            std::string dev;
            while(it < end && *it)
                dev += *it++;
            arr.push_back(dev);
            ++it;
        }
        
#elif defined(UKN_OS_FAMILY_UNIX)
        arr.push_back("/");
#endif    
    }
    
    String Path::ExpandPath(const String& path) {
#if defined(UKN_OS_WINDOWS)
        wchar_t buffer[_MAX_PATH];
        DWORD n = ExpandEnvironmentStringsW(path.c_str(), buffer, sizeof(buffer));
        if(n > 0 && n < sizeof(buffer)) {
            return ukn_wstring(buffer, n-1);
        } else
            return path;
        
#elif defined(UKN_OS_FAMILY_UNIX)
        String result;
        String::const_iterator it  = path.begin();
        String::const_iterator end = path.end();
        if (it != end && *it == L'~') {
            ++it;
            if (it != end && *it == L'/') {
                result += Path::GetHome(); ++it;
            }
            else result += '~';
        }
        while (it != end) {
            if (*it == L'$') {
                std::string var;
                ++it;
                if (it != end && *it == L'{') {
                    ++it;
                    while (it != end && *it != L'}') var += *it++;
                    if (it != end) ++it;
                }
                else {
                    while (it != end && (((*it >= L'a' && *it <= L'z') || (*it >= L'A' && *it <= L'Z') || (*it >= L'0' && *it <= L'9')) || *it == L'_')) var += *it++;
                }
                result += Path::GetEnv(var);
            }
            else result += *it++;
        }
        return result;
#endif    
    }
    
} // namespace ukn


#ifndef UKN_OS_WINDOWS

#include <dirent.h>

namespace ukn {
    
    class DirectoryIterator::DirectoryIteratorImpl {
    public:
        DirectoryIteratorImpl(const ukn_string& path) {
            mDir = opendir((path).c_str());
            
            if(mDir)
                next();
        }
        
        ~DirectoryIteratorImpl() {
            if(mDir)
                closedir(mDir);
        }
        
        void duplicate();
        void release();
        
        const ukn_string& get() const;
        const ukn_string& next() {
            if(!mDir)
                return mCurrent;
            
            do {
                struct dirent* entry = readdir(mDir);
                if(entry)
                    mCurrent = entry->d_name;
                else
                    mCurrent.clear();
            }
            while(mCurrent == "." || mCurrent == ".." || (mCurrent.size() > 0 && mCurrent[0] == '.'));
            
            return mCurrent;
        }
        
    private:
        DIR* mDir;
        ukn_string mCurrent;
        int mRC;
    };
    
    const ukn_string& DirectoryIterator::DirectoryIteratorImpl::get() const {
        return mCurrent;
    }
    
    inline void DirectoryIterator::DirectoryIteratorImpl::duplicate() {
        ++mRC;
    }
    
    inline void DirectoryIterator::DirectoryIteratorImpl::release() {
        if (--mRC) {
            delete this;
        }
    }    
} 

#else

#include <Windows.h>

namespace ukn {
    
    class DirectoryIterator::DirectoryIteratorImpl {
    public:
        DirectoryIteratorImpl(const ukn_string& path) {
            std::string findPath = path + "/*";
            
            mFH = FindFirstFileA(findPath.c_str(), &mFD);
            if(mFH == INVALID_HANDLE_VALUE) {
				DWORD error = GetLastError();
                if(error != ERROR_NO_MORE_FILES) {
                    
                }
            } else {
                mCurrent = mFD.cFileName;
                if(mCurrent == "." || mCurrent == "..")
                    next();
            }
        }
        
        ~DirectoryIteratorImpl() {
            if(mFH != INVALID_HANDLE_VALUE)
                FindClose(mFH);
        }
        
        void duplicate();
        void release();
        
        const ukn_string& get() const;
        const ukn_string& next() {
            do {
                if(FindNextFileA(mFH, &mFD) != 0) {
                    mCurrent = mFD.cFileName;
                } else 
                    mCurrent.clear();
            }
            while(mCurrent == "." || mCurrent == "..");
            
            return mCurrent;
        }
        
    private:
        HANDLE mFH;
        WIN32_FIND_DATAA mFD;
        ukn_string mCurrent;
        int mRC;
    };
    
    const std::string& DirectoryIterator::DirectoryIteratorImpl::get() const {
        return mCurrent;
    }
    
    inline void DirectoryIterator::DirectoryIteratorImpl::duplicate() {
        ++mRC;
    }
    
    inline void DirectoryIterator::DirectoryIteratorImpl::release() {
        if (--mRC) {
            delete this;
        }
    }
    
} // namespace ukn

#endif // UKN_OS_WINDOWS

namespace ukn {
    
    DirectoryIterator::DirectoryIterator():
    mImpl(0) {
        mIsEnd = false;
    }
    
    DirectoryIterator::DirectoryIterator(const DirectoryIterator& iterator):
    mPath(iterator.mPath),
    mFile(iterator.mFile),
    mImpl(iterator.mImpl) {
        mIsEnd = false;
        if(mImpl) {
            mImpl->duplicate();
        }
    }
    
    DirectoryIterator::DirectoryIterator(const ukn_string& path):
    mPath(path+"/"),
    mImpl(new DirectoryIteratorImpl(path)) {
        mIsEnd = false;
        
        mFile = mPath + mImpl->get();
    }
    
    DirectoryIterator::DirectoryIterator(const ukn_wstring& path):
    mPath(String::WStringToString(path+L"/")) {
        mIsEnd = false;
        mImpl = new DirectoryIteratorImpl(mPath.c_str());
        
        mFile = mPath + mImpl->get();
    }
    
    DirectoryIterator::~DirectoryIterator() {
        if(mImpl)
            mImpl->release();
    }
    
    DirectoryIterator& DirectoryIterator::operator =(const DirectoryIterator& rhs) {
        if(this != &rhs) {
            if(mImpl)
                mImpl->duplicate();
            mImpl = rhs.mImpl;
            if(mImpl) {
                mImpl->duplicate();
                mPath = rhs.mPath;
                mIsEnd = rhs.mIsEnd;
                mFile = rhs.mFile;
            }
            return *this;
        }
        return *this;
    }
    
    
    DirectoryIterator& DirectoryIterator ::operator++() {
        if(mImpl) {
            std::string n = mImpl->next();
            if(n.empty())
                mIsEnd = true;
            else {
                mFile = mPath + mImpl->get();
            }
        }
        return *this;
    }
    
}