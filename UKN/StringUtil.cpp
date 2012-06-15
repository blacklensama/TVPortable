//
//  StringUtil.cpp
//  Project Unknown
//
//  Created by Robert Bu on 11/23/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#include "UKN/StringUtil.h"
#include "UKN/MemoryUtil.h"
#include "UKN/MathUtil.h"

namespace ukn {
    
    static size_t utf8_strlen(const char* s) {
        size_t len = 0;
        for(; *s; ) {
            if(*s <= 0x7f)
                s += 1;
            else if(*s <= 0xc0)
                s += 2;
            else 
                s += 3;
            len ++;
        }
        return len;
    }
    
    String::String():
    mStrBuff(0),
    mStrBuffSize(0) {
        
    }
    
    String::String(const char* str, StringFormat format):
    mStrBuff(0),
    mStrBuffSize(0) {
        convert(str, format);
    }
    
    String::String(const wchar_t* str):
    mStrBuff(0),
    mStrBuffSize(0) {
        assign(str);
    }
    
    String::String(const std::string& str, StringFormat format):
    mStrBuff(0),
    mStrBuffSize(0) {
        convert(str.c_str(), format);
    }
    
    String::String(const std::wstring& str):
    mStrBuff(0),
    mStrBuffSize(0) {
        assign(str);
    }
    
    bool String::convert(const char* str, StringFormat format) {
        mFormat = format;
        this->clear();
        switch(format) {
            case SF_ANSI:
                this->assign(String::StringToWString(str));
                break;
                
            case SF_UTF8: {
                wchar_t temp;
                unsigned char* ptemp = (unsigned char*)&temp;
                for(; *str; ) {
                    // 0xxxxxxx
                    if(((*str) & 0x80) == 0) {
                        ptemp[0] = *str;
                        ptemp[1] = 0;
                        str++;
                    // 110xxxxx 10xxxxxx
                    } else if(((*str) & 0xE0) == 0xC0) {
                        ptemp[0] = ((*str) << 6) | ((*(str+1)) & 0x3F);
                        ptemp[1] = ((*str) >> 2) & 0x07;
                        str += 2;
                    // 1110xxxx 10xxxxxx 10xxxxxx
                    } else {
                        ptemp[0] = ((*(str+1)) << 6) | ((*(str + 2)) & 0x3F);
                        ptemp[1] = ((*str) << 4) | (((*(str + 1)) >> 2) & 0x0F);
                        str += 3;
                    }
                    this->push_back(temp);
                }
                break;
            }
                
            case SF_Unicode: {
                wchar_t temp;
                unsigned char* ptemp = (unsigned char*)&temp;
                
                for(; *str; str += 2) {
                    ptemp[0] = *(str + 1);
                    ptemp[1] = *str;
                    this->push_back(temp);
                }
                break;
            }
        }
        return true;
    }
    
    StringFormat String::format() const {
        return mFormat;
    }
    
    const char* String::ansi_str() {
        return String::WStringToString(*this).c_str();
    }
    
    const char* String::utf8_str() {
        if(mStrBuffSize < size() * 3 + 1) {
            if(mStrBuff != 0)
                ukn_free(mStrBuff);
            mStrBuff = (char*)malloc(size() * 3 + 1);
            mStrBuffSize = size() * 3 + 1;
        }
        char* pstr = mStrBuff;
        size_t i = 0, len = size();
        for(const wchar_t* s = c_str(); i < len; s++, i++) {
            if(*s < 0x80) {
                *pstr = 0 | (char)(*s);
                pstr += 1;
            } else if(*s < 0x800) {
                pstr[0] = 0xE0 | (char)((*s) >> 6);
                pstr[1] = 0x80 | ((*s) & 0x3F);
                pstr += 2;
            } else {
                pstr[0] = 0xE0 | ((*s) >> 12);
                pstr[1] = 0x80 | (((*s) >> 6) & 0x3F);
                pstr[2] = 0x80 | ((*s) & 0x3F);
                pstr += 3;
            }
        }
        *pstr = '\0';
        return mStrBuff;
    }
    
    std::ostream& operator<<(std::ostream& os, const String& val) {
        os << String::WStringToString(val);
        return os;
    }
    
    StringTokenlizer::StringTokenlizer() {
        
    }
    
    StringTokenlizer::StringTokenlizer(const ukn_string& str) {
        parse(str, ukn_string());
    }
    
    StringTokenlizer::StringTokenlizer(const ukn_string& str, const ukn_string& deli) {
        parse(str, deli);
    }
    
    ukn_string StringTokenlizer::first() {
        if(mTokens.size() != 0)
            return mTokens.front();
        return ukn_string();
    }
    
    ukn_string StringTokenlizer::last() {
        if(mTokens.size() != 0)
            return mTokens.back();
        return ukn_string();
    }
    
    ukn_string StringTokenlizer::front() {
        if(mTokens.size() != 0)
            return mTokens.front();
        return ukn_string();
    }
    
    ukn_string StringTokenlizer::back() {
        if(mTokens.size() != 0)
            return mTokens.back();
        return ukn_string();
        
    }
    
    StringTokenlizer::iterator StringTokenlizer::begin() {
        return mTokens.begin();
    }
    
    StringTokenlizer::iterator StringTokenlizer::end() {
        return mTokens.end();
    }
    
    const StringTokenlizer::TokenContainer& StringTokenlizer::getTokens() const {
        return mTokens;
    }
    
    bool StringTokenlizer::isSpace(char c) {
        return (c == '\n' || c == ' ' || c == 'r' || c == '\t');
    }
    
    bool StringTokenlizer::isAlpha(char c) {
        return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'));
    }
    
    void StringTokenlizer::parse(const ukn_string& str) {
        parse(str, ukn_string());
    }
    
    ukn_string StringTokenlizer::operator[](size_t index) {
        ukn_assert(index < mTokens.size());
        
        return mTokens[index];
    }
    
    void StringTokenlizer::parse(const ukn_string& str, const ukn_string& deli) {
        size_t start = 0, end = 0;
        if(deli.size() != 0) {
            while(end < str.size()) {
                while((deli.find(str[end]) == ukn_string::npos) && end < str.size()) { ++end; }
                if(end == str.size()) {
                    while(deli.find(str[end]) != ukn_string::npos)
                        --end;
                    mTokens.push_back( str.substr(start, end-start) );
                    break;
                }
                if(end != start)
                    mTokens.push_back( str.substr(start, end-start) );
                
                ++end; 
                while((deli.find(str[end]) != ukn_string::npos || isSpace(str[end])) && end < str.size()) ++end;
                
                start = end;
            }
        } else {
            while(end < str.size()) {
                while(isAlpha(str[end]) && end < str.size()) { ++end; }
                if(end == str.size()) {
                    mTokens.push_back( str.substr(start) );
                    break;
                }
                mTokens.push_back( str.substr(start, end-start) );
                
                ++end; 
                while(!isAlpha(str[end]) && end < str.size()) {
                    if(str[end] == '"') {
                        ++end;
                        start = end;
                        while(str[end] != '"' && end < str.size())
                            ++end;
                        mTokens.push_back( str.substr(start, end-start) );
                    }
                    ++end;
                    
                }
                start = end;
            }
        }
    }
    
    void StringTokenlizer::operator=(const ukn_string& str) {
        parse(str, ukn_string());
    }
    
    void StringTokenlizer::operator=(const char* str) {
        parse(str, ukn_string());
    }
    
    size_t StringTokenlizer::size() const {
        return mTokens.size();
    }
    
} // namespace ukn

#ifdef UKN_OS_WINDOWS
#include <windows.h>
#elif defined(UKN_OS_IOS) || defined(UKN_OS_OSX)
#include "AppleStuff.h"
#endif

#include <cstring>
#include <locale.h>

namespace ukn {
    
#ifdef UKN_OS_WINDOWS
	static ukn_string ukn_win_wstring_to_string(const String& pwszSrc) {
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc.c_str(), -1, NULL, 0, NULL, NULL);
		if (nLen<= 0) return std::string("");
		char* pszDst = new char[nLen];
		if (NULL == pszDst) return std::string("");
		WideCharToMultiByte(CP_ACP, 0, pwszSrc.c_str(), -1, pszDst, nLen, NULL, NULL);
		pszDst[nLen -1] = 0;
		std::string strTemp(pszDst);
		delete [] pszDst;
		return strTemp;
	}
    
	static ukn_wstring ukn_win_string_to_wstring(const ukn_string& pszSrc) {
		if(pszSrc.size() == 0)
			return L"\0";
		int nSize = MultiByteToWideChar(CP_ACP, 0, pszSrc.c_str(), pszSrc.size(), 0, 0);
		if(nSize <= 0) return NULL;
		WCHAR *pwszDst = new WCHAR[nSize+1];
		if( NULL == pwszDst) return NULL;
		MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc.c_str(), pszSrc.size(), pwszDst, nSize);
		pwszDst[nSize] = 0;
		if( pwszDst[0] == 0xFEFF)                    // skip Oxfeff
			for(int i = 0; i < nSize; i ++)
				pwszDst[i] = pwszDst[i+1];
		std::wstring wcharString(pwszDst);
		delete pwszDst;
		return wcharString;
	}
#endif
    
    static ukn_string ukn_normal_wstring_to_string(const String& ws) {
        std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
        setlocale(LC_ALL, "chs");
        const wchar_t* _Source = ws.c_str();
        size_t _Dsize = 2 * ws.size() + 1;
        char *_Dest = new char[_Dsize];
        memset(_Dest,0,_Dsize);
        wcstombs(_Dest,_Source,_Dsize);
        std::string result = _Dest;
        delete []_Dest;
        setlocale(LC_ALL, curLocale.c_str());
        return result;
    }
    
    static ukn_wstring ukn_normal_string_to_wstring(const ukn_string& s) {
        setlocale(LC_ALL, "chs"); 
        const char* _Source = s.c_str();
        size_t _Dsize = s.size() + 1;
        wchar_t *_Dest = new wchar_t[_Dsize];
        wmemset(_Dest, 0, _Dsize);
        mbstowcs(_Dest,_Source,_Dsize);
        std::wstring result = _Dest;
        delete []_Dest;
        setlocale(LC_ALL, "C");
        return result;
    }
    
    ukn_string String::WStringToString(const ukn_wstring& str) {
#if defined(UKN_OS_WINDOWS)
        return ukn_win_wstring_to_string(str);
#elif defined(UKN_OS_IOS)
        return ukn_apple_wstring_to_string(str);
#else
        return ukn_normal_wstring_to_string(str);
#endif
    }
    
    ukn_wstring String::StringToWString(const ukn_string& str) {
#if defined(UKN_OS_WINDOWS)
        return ukn_win_string_to_wstring(str);
#elif defined(UKN_OS_IOS)
        return ukn_apple_string_to_wstring(str);
#else
        return ukn_normal_string_to_wstring(str);
#endif  
    }
    
    ukn_string String::WStringToStringFast(const ukn_wstring& str) {
        ukn_string buffer(str.length(), ' ');
		std::copy(str.begin(), str.end(), buffer.begin());
		return buffer;
    }
    
    ukn_wstring String::StringToWStringFast(const ukn_string& str) {
        ukn_wstring buffer(str.length(), L' ');
		std::copy(str.begin(), str.end(), buffer.begin());
		return buffer; 
    }
    
    ukn_string String::GetFileName(const ukn_string& str) {
        ukn_string::const_iterator it = str.end();
        it--;
        
        while(it != str.begin() && *it != '/' && *it != '\\') {
            --it;
        }
        return ukn_string(it, str.end());
    }
    
    ukn_wstring String::GetFileName(const ukn_wstring& str) {
        ukn_wstring::const_iterator it = str.end();
        it--;
        
        while(it != str.begin() && *it != L'/' && *it != L'\\') {
            --it;
        }
        return ukn_wstring(it, str.end());
    }
    
    ukn_string String::GetFilePath(const ukn_string& str) {
        if(str.empty())
            return str;
        
        ukn_string::const_iterator it = str.end();
        it--;
        
        while(it != str.begin() && *it != '/' && *it != '\\') {
            --it;
        }
        if(it != str.begin())
            return ukn_string(str.begin(), it+1);
        return ukn_string();
    }

    ukn_wstring String::GetFilePath(const ukn_wstring& str) {
        if(str.empty())
            return str;
        
        ukn_wstring::const_iterator it = str.end();
        it--;
        
        while(it != str.begin() && *it != L'/' && *it != L'\\') {
            --it;
        }
        if(it != str.begin())
            return ukn_wstring(str.begin(), it+1);
        return ukn_wstring();
    }
    
    uint16 *utf8_to_unicode(uint16 *unicode, const char *utf8, size_t len) {
        int i, j;
        uint16 ch;
        
        for ( i=0, j=0; i < len; ++i, ++j ) {
            ch = ((const unsigned char *)utf8)[i];

            if ( ch >= 0xF0 ) {
                ch  =  (uint16)(utf8[i]&0x07) << 18;
                ch |=  (uint16)(utf8[++i]&0x3F) << 12;
                ch |=  (uint16)(utf8[++i]&0x3F) << 6;
                ch |=  (uint16)(utf8[++i]&0x3F);
            } else
                if ( ch >= 0xE0 ) {
                    ch  =  (uint16)(utf8[i]&0x0F) << 12;
                    ch |=  (uint16)(utf8[++i]&0x3F) << 6;
                    ch |=  (uint16)(utf8[++i]&0x3F);
                } else
                    if ( ch >= 0xC0 ) {
                        ch  =  (uint16)(utf8[i]&0x1F) << 6;
                        ch |=  (uint16)(utf8[++i]&0x3F);
                    }
            unicode[j] = ch;
        }
        unicode[j] = 0;
        
        return unicode;
    }
    
    int unicode_strlen(const uint16 *text) {
        int size = 0;
        while ( *text++ ) {
            ++size;
        }
        return size;
    }
    
    void unicode_strcpy(uint16 *dst, const uint16 *src, size_t swap) {
        if ( swap ) {
            while ( *src ) {
                *dst = flip_bytes(*src);
                ++src;
                ++dst;
            }
            *dst = '\0';
        } else {
            while ( *src ) {
                *dst = *src;
                ++src;
                ++dst;
            }
            *dst = '\0';
        }
    }

    
} // namespace ukn
