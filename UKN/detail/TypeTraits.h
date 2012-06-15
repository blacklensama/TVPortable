//
//  TypeTraits.h
//  zeromq_client
//
//  Created by Robert Bu on 8/15/11.
//  Copyright 2011 heizi. All rights reserved.
//

#ifndef ukn_TypeTraits_h
#define ukn_TypeTraits_h

namespace ukn {
    
    namespace traits {
        template<bool cont, typename T1, typename T2>
        struct selector {
            typedef T1 type;
        };
        
        template<typename T1, typename T2>
        struct selector<false, T1, T2> {
            typedef T2 type;
        };
        
        template<typename T>
        struct is_void {
            static const bool value = false;
        };
        
        template<>
        struct is_void<void> {
            static const bool value = true;
        };
        
        template<typename T>
        struct is_pointer {
            static const bool value = false;
        };
        
        template<typename T>
        struct is_pointer<T*> {
            static const bool value = true;
        };
        
        template<typename T>
        struct strip_pointer {
            typedef T type;
        };
        
        template<typename T>
        struct strip_pointer<T*> {
            typedef T type;
        };
        
        template<typename T>
        struct strip_qualifier {
            typedef T type;
        };
        
        template<typename T>
        struct strip_qualifier<T*> {
            typedef T type;
        };
        
        template<typename T>
        struct strip_qualifier<T&> {
            typedef T type;
        };
        
        template<typename T>
        struct strip_qualifier<const T> {
            typedef T type;
        };
        
        template<typename T>
        struct strip_qualifier<volatile T> {
            typedef T type;
        };
        
        // helper template to seperate array type and size
        template<typename T>
        struct strip_array;
        
        template<typename T, size_t LENGTH>
        struct strip_array<T[LENGTH]> {
            typedef T type;
            
            enum { Length = LENGTH };
        };
        
        template<typename T>
        struct is_member_pointer {
            static const bool value = false;
        };
        
        template<typename T, typename R>
        struct is_member_pointer<R T::*> {
            static const bool value = true;
        };
        
        template<typename T, typename R>
        struct is_member_pointer<R T::* const> {
            static const bool value = true;
        };
        
        template<typename T>
        struct is_reference_wrapper {
            static const bool value = false;
        };
        
        template<typename T>
        struct is_reference_wrapper<T&> {
            static const bool value = true;
        };
        
        template<bool C, typename T1, typename T2>
        struct if_c {
            typedef T1 type;
        };
        
        template<typename T1, typename T2>
        struct if_c<false,T1,T2> {
            typedef T2 type;
        };  
        
        
        template<typename T>
        struct is_pod {
            enum { value = false };
        };
        
        template<> struct is_pod<int32>         { enum { value = true }; };
        template<> struct is_pod<uint32>        { enum { value = true }; };
        template<> struct is_pod<intPtr>        { enum { value = true }; };
        template<> struct is_pod<uintPtr>       { enum { value = true }; };
        template<> struct is_pod<uint8>         { enum { value = true }; };
        template<> struct is_pod<uint16>        { enum { value = true }; };
        template<> struct is_pod<int8>          { enum { value = true }; };
        template<> struct is_pod<int16>         { enum { value = true }; };
        template<> struct is_pod<uint64>        { enum { value = true }; };
        template<> struct is_pod<int64>         { enum { value = true }; };
        template<> struct is_pod<char>          { enum { value = true }; };
        template<> struct is_pod<wchar_t>       { enum { value = true }; };
        template<typename T> struct is_pod<T*>  { enum { value = true }; };
        template<typename T> struct is_pod<T&>  { enum { value = true }; };
        
        template<typename T, intPtr size> struct is_pod<T[size]>    { enum { value = is_pod<T>::value }; };
        template<typename T, typename C> struct is_pod<T C::*>      { enum { value = true }; };
        template<typename T> struct is_pod<const T>                 { enum { value = is_pod<T>::value }; };
        template<typename T> struct is_pod<volatile T>              { enum { value = is_pod<T>::value }; };
    } // namespace traits
    
} // namespace ukn

#endif
