//
//  Memfun.h
//  zeromq_client
//
//  Created by Robert Bu on 8/15/11.
//  Copyright 2011 heizi. All rights reserved.
//

#ifndef ukn_Memfun_h
#define ukn_Memfun_h

namespace ukn {
    
    template<typename sig, typename obj>
    class Memfun;
    
    template<typename R, typename obj>
    class Memfun<R(void), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*mem_fun)(void);
        
        explicit Memfun(mem_fun func):
        mFunc(func) { }
        
        inline R invoke(obj* left) {
            return (left->*mFunc)();
        }
        
        inline R operator()(obj* left) {
            return (left->*mFunc)();
        }
        
        operator bool() {
            return this->mFunc != NULL;
        }
        
    private:
        mem_fun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename obj>
    class Memfun<R(T0), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*mem_fun)(T0);
        
        explicit Memfun(mem_fun func):
        mFunc(func) { }
        
        inline R invoke(obj* left, T0 a0) {
            return (left->*mFunc)(a0);
        }

        inline R operator()(obj* left, T0 a0) {
            return (left->*mFunc)(a0);
        }
        
        operator bool() {
            return this->mFunc != NULL;
        }
        
    private:
        mem_fun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename obj>
    class Memfun<R(T0, T1), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*mem_fun)(T0, T1);
        
        explicit Memfun(mem_fun func):
        mFunc(func) { }
        
        inline R invoke(obj* left, T0 a0, T1 a1) {
            return (left->*mFunc)(a0, a1);
        }
        
        inline R operator()(obj* left, T0 a0, T1 a1) {
            return (left->*mFunc)(a0, a1);
        }
        
        operator bool() {
            return this->mFunc != NULL;
        }
        
    private:
        mem_fun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename obj>
    class Memfun<R(T0, T1, T2), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*mem_fun)(T0, T1, T2);
        
        explicit Memfun(mem_fun func):
        mFunc(func) { }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2) {
            return (left->*mFunc)(a0, a1, a2);
        }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2) {
            return (left->*mFunc)(a0, a1, a2);
        }
        
        operator bool() {
            return this->mFunc != NULL;
        }
        
    private:
        mem_fun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1,
             typename T2, 
             typename T3, 
             typename obj>
    class Memfun<R(T0, T1, T2, T3), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*mem_fun)(T0, T1, T2, T3);
        
        explicit Memfun(mem_fun func):
        mFunc(func) { }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3) {
            return (left->*mFunc)(a0, a1, a2, a3);
        }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3) {
            return (left->*mFunc)(a0, a1, a2, a3);
        }
        
        operator bool() {
            return this->mFunc != NULL;
        }
        
    private:
        mem_fun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename T3, 
             typename T4, 
             typename obj>
    class Memfun<R(T0, T1, T2, T3, T4), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*mem_fun)(T0, T1, T2, T3, T4);
        
        explicit Memfun(mem_fun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) {
            return (left->*mFunc)(a0, a1, a2, a3, a4);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) {
            return (left->*mFunc)(a0, a1, a2, a3, a4);
        }
        
        operator bool() {
            return this->mFunc != NULL;
        }
        
    private:
        mem_fun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename T3, 
             typename T4, 
             typename T5, 
             typename obj>
    class Memfun<R(T0, T1, T2, T3, T4, T5), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*mem_fun)(T0, T1, T2, T3, T4, T5);
        
        explicit Memfun(mem_fun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5);
        }
        
        operator bool() {
            return this->mFunc != NULL;
        }
        
    private:
        mem_fun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename T3, 
             typename T4, 
             typename T5, 
             typename T6, 
             typename obj>
    class Memfun<R(T0, T1, T2, T3, T4, T5, T6), obj> {
    public:
        typedef R (obj::*mem_fun)(T0, T1, T2, T3, T4, T5, T6);
        
        explicit Memfun(mem_fun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6);
        }
        
        operator bool() {
            return this->mFunc != NULL;
        }
        
    private:
        mem_fun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename T3, 
             typename T4, 
             typename T5, 
             typename T6, 
             typename T7,
             typename obj>
    class Memfun<R(T0, T1, T2, T3, T4, T5, T6, T7), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*mem_fun)(T0, T1, T2, T3, T4, T5, T6, T7);
        
        explicit Memfun(mem_fun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7);
        }
        
        operator bool() {
            return this->mFunc != NULL;
        }
        
    private:
        mem_fun mFunc;
    };
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename T3, 
             typename T4, 
             typename T5, 
             typename T6, 
             typename T7,
             typename T8,
             typename obj>
    class Memfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*mem_fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
        
        explicit Memfun(mem_fun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7, a8);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7, a8);
        }
        
        operator bool() {
            return this->mFunc != NULL;
        }
        
    private:
        mem_fun mFunc;
    };
        
    template<typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4, 
            typename T5, 
            typename T6, 
            typename T7,
            typename T8,
            typename T9,
            typename obj>
    class Memfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*mem_fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
        
        explicit Memfun(mem_fun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
        }
        
        operator bool() {
            return this->mFunc != NULL;
        }
        
    private:
        mem_fun mFunc;
    };
    
    template<typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4, 
            typename T5, 
            typename T6, 
            typename T7,
            typename T8,
            typename T9,
            typename T10,
            typename obj>
    class Memfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10), obj> {
    public:
        typedef obj ObjType;
        typedef R ReturnType;
        
        typedef R (obj::*mem_fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
        
        explicit Memfun(mem_fun func):
        mFunc(func) { }
        
        inline R operator()(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
        }
        
        inline R invoke(obj* left, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10) {
            return (left->*mFunc)(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
        }
        
        operator bool() {
            return this->mFunc != NULL;
        }
        
    private:
        mem_fun mFunc;
    };
    
    template<typename R, typename obj>
    inline Memfun<R(), obj> MakeMemFun(R (obj::*mem_fun)()) {
        return Memfun<R(), obj>(mem_fun);
    }
    
    template<typename R, typename T0, typename obj>
    inline Memfun<R(T0), obj> MakeMemFun(R (obj::*mem_fun)(T0)) {
        return Memfun<R(T0), obj>(mem_fun);
    }
    
    template<typename R, typename T0, typename T1, typename obj>
    inline Memfun<R(T0, T1), obj> MakeMemFun(R (obj::*mem_fun)(T0, T1)) {
        return Memfun<R(T0, T1), obj>(mem_fun);
    }
    
    template<typename R, typename T0, typename T1, typename T2, typename obj>
    inline Memfun<R(T0, T1, T2), obj> MakeMemFun(R (obj::*mem_fun)(T0, T1, T2)) {
        return Memfun<R(T0, T1, T2), obj>(mem_fun);
    }
    
    template<typename R, typename T0, typename T1, typename T2, typename T3, typename obj>
    inline Memfun<R(T0, T1, T2, T3), obj> MakeMemFun(R (obj::*mem_fun)(T0, T1, T2, T3)) {
        return Memfun<R(T0, T1, T2, T3), obj>(mem_fun);
    }
    
    template<typename R, 
             typename T0, 
             typename T1, 
             typename T2, 
             typename T3, 
             typename T4,
             typename obj>
    inline Memfun<R(T0, T1, T2, T3, T4), obj> MakeMemFun(R (obj::*mem_fun)(T0, T1, T2, T3, T4)) {
        return Memfun<R(T0, T1, T2, T3, T4), obj>(mem_fun);
    }
    
    template<typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename obj>
    inline Memfun<R(T0, T1, T2, T3, T4, T5), obj> MakeMemFun(R (obj::*mem_fun)(T0, T1, T2, T3, T4, T5)) {
        return Memfun<R(T0, T1, T2, T3, T4, T5), obj>(mem_fun);
    }
    
    template<typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename obj>
    inline Memfun<R(T0, T1, T2, T3, T4, T5, T6), obj> MakeMemFun(R (obj::*mem_fun)(T0, T1, T2, T3, T4, T5, T6)) {
        return Memfun<R(T0, T1, T2, T3, T4, T5, T6), obj>(mem_fun);
    }
    
    template<typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename obj>
    inline Memfun<R(T0, T1, T2, T3, T4, T5, T6, T7), obj> MakeMemFun(R (obj::*mem_fun)(T0, T1, T2, T3, T4, T5, T6, T7)) {
        return Memfun<R(T0, T1, T2, T3, T4, T5, T6, T7), obj>(mem_fun);
    }
    
    template<typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename T8,
            typename obj>
    inline Memfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8), obj> MakeMemFun(R (obj::*mem_fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8)) {
        return Memfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8), obj>(mem_fun);
    }
    
    template<typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename T8,
            typename T9,
            typename obj>
    inline Memfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9), obj> MakeMemFun(R (obj::*mem_fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)) {
        return Memfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9), obj>(mem_fun);
    }
    
    template<typename R, 
            typename T0, 
            typename T1, 
            typename T2, 
            typename T3, 
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename T8,
            typename T9,
            typename T10,
            typename obj>
    inline Memfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10), obj> MakeMemFun(R (obj::*mem_fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)) {
        return Memfun<R(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10), obj>(mem_fun);
    }
} // namespace ukn

#endif
