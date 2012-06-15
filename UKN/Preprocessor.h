//
//  Preprocessor.h
//  Project Unknown
//
//  Created by Robert Bu on 11/21/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_Preprocessor_h
#define Project_Unknown_Preprocessor_h

namespace ukn {
    template<bool x> struct STATIC_ASSERTION_FAILURE;
    template<> struct STATIC_ASSERTION_FAILURE<true> { enum {Value = 1}; };
    template<int x> struct static_assert_test{};
}

#define UKN_JOIN(x, y) UKN_DO_JOIN(x, y)
#define UKN_DO_JOIN(x ,y) UKN_DO_JOIN_2(x, y)
#define UKN_DO_JOIN_2(x, y) x##y

#define ukn_static_assert(B) \
typedef ::ukn::static_assert_test<\
    sizeof(::ukn::STATIC_ASSERTION_FAILURE<(bool)(B)>)>\
        UKN_JOIN(__ukn_static_assert_typedef_, __LINE__)

#define UKN_UNIQUE_NAME(name) \
    UKN_JOIN(name, __LINE__)

#define UKN_STATIC_RUN_CODE_I(name, code) \
    namespace { \
        static struct name { \
            name() { \
                code; \
            } \
        } UKN_JOIN(g_, name); \
    }

#define UKN_STATIC_RUN_CODE(code) \
    UKN_STATIC_RUN_CODE_I(UKN_UNIQUE_NAME(UKN_static_), code) 

#define UKN_STATIC_CALL_FUNC_I(name, FN) \
    namespace { \
        static struct name { \
            name() { \
            static int counter = 0; \
            if(counter++ > 0) return; \
                FN(); \
            } \
        } g_##name; \
    }

#define UKN_STATIC_CALL_FUNC(FN) \
    UKN_STATIC_CALL_FUNC_I(UKN_static_init_##FN, FN)

#define UKN_REPEAT_1_0(m, d)
#define UKN_REPEAT_1_1(m, d) m(0, d)
#define UKN_REPEAT_1_2(m, d) UKN_REPEAT_1_1(m, d) m(1, d)
#define UKN_REPEAT_1_3(m, d) UKN_REPEAT_1_2(m, d) m(2, d)
#define UKN_REPEAT_1_4(m, d) UKN_REPEAT_1_3(m, d) m(3, d)
#define UKN_REPEAT_1_5(m, d) UKN_REPEAT_1_4(m, d) m(4, d)
#define UKN_REPEAT_1_6(m, d) UKN_REPEAT_1_5(m, d) m(5, d)
#define UKN_REPEAT_1_7(m, d) UKN_REPEAT_1_6(m, d) m(6, d)
#define UKN_REPEAT_1_8(m, d) UKN_REPEAT_1_7(m, d) m(7, d)
#define UKN_REPEAT_1_9(m, d) UKN_REPEAT_1_8(m, d) m(8, d)
#define UKN_REPEAT_1_10(m, d) UKN_REPEAT_1_9(m, d) m(9, d)
#define UKN_REPEAT_1_11(m, d) UKN_REPEAT_1_10(m, d) m(10, d)
#define UKN_REPEAT_1_12(m, d) UKN_REPEAT_1_11(m, d) m(11, d)

#define UKN_REPEAT_2_0(m, d, d2) 
#define UKN_REPEAT_2_1(m, d, d2) m(0, d, d2)
#define UKN_REPEAT_2_2(m, d, d2) UKN_REPEAT_2_1(m, d, d2) m(1, d, d2)
#define UKN_REPEAT_2_3(m, d, d2) UKN_REPEAT_2_2(m, d, d2) m(2, d, d2)
#define UKN_REPEAT_2_4(m, d, d2) UKN_REPEAT_2_3(m, d, d2) m(3, d, d2)
#define UKN_REPEAT_2_5(m, d, d2) UKN_REPEAT_2_4(m, d, d2) m(4, d, d2)
#define UKN_REPEAT_2_6(m, d, d2) UKN_REPEAT_2_5(m, d, d2) m(5, d, d2)
#define UKN_REPEAT_2_7(m, d, d2) UKN_REPEAT_2_6(m, d, d2) m(6, d, d2)
#define UKN_REPEAT_2_8(m, d, d2) UKN_REPEAT_2_7(m, d, d2) m(7, d, d2)
#define UKN_REPEAT_2_9(m, d, d2) UKN_REPEAT_2_8(m, d, d2) m(8, d, d2)
#define UKN_REPEAT_2_10(m, d, d2) UKN_REPEAT_2_9(m, d, d2) m(9, d, d2)
#define UKN_REPEAT_2_11(m, d, d2) UKN_REPEAT_2_10(m, d, d2) m(10, d, d2)
#define UKN_REPEAT_2_12(m, d, d2) UKN_REPEAT_2_11(m, d, d2) m(11, d, d2)

// would be expanded to m([0-(n-1)], d)
// m should be a macro provided by user, receiving two params
// for example
//      #define expand(n, d) d##n = n;
//      UKN_REPEAT_N(4, expand, myVar)
// would be expanded to
// myVar0 = 0; myVar1 = 1; myVar2 = 2; myVar3 = 3;
// n max = 12
#define UKN_REPEAT_N(n, m, d) UKN_REPEAT_1_##n(m, d)
#define UKN_REPEAT_N_2(n, m, d, d2) UKN_REPEAT_2_##n(m, d, d2)

#define UKN_ENUM_N(n, m, d) UKN_REPEAT_N_2(n, UKN_ENUM_M, m, d)

#define UKN_ENUM_M(n, m, d) UKN_COMMA_IF(n) m(n, d)

// function param repeat definition macro
// would be expanded to d##0, d##1, d##2, d##n-1
// n max = 12
#define UKN_ENUM_PARAMS(n, d) UKN_REPEAT_N(n, UKN_FUNCTION_M, d)

#define UKN_PARAM_REPEAT(n, m, d)

#define UKN_BOOL_0 0
#define UKN_BOOL_1 1
#define UKN_BOOL_2 1
#define UKN_BOOL_3 1
#define UKN_BOOL_4 1
#define UKN_BOOL_5 1
#define UKN_BOOL_6 1
#define UKN_BOOL_7 1
#define UKN_BOOL_8 1
#define UKN_BOOL_9 1
#define UKN_BOOL_10 1
#define UKN_BOOL_11 1
#define UKN_BOOL_12 1
#define UKN_BOOL_13 1
#define UKN_BOOL_14 1
#define UKN_BOOL_15 1
#define UKN_BOOL_16 1
#define UKN_BOOL_17 1
#define UKN_BOOL_18 1
#define UKN_BOOL_19 1
#define UKN_BOOL_20 1

#define UKN_COMMA() ,
#define UKN_EMPTY() 

#define UKN_IF_IIF_0(t, f) t
#define UKN_IF_IIF_1(t, f) f

#define UKN_BOOL_N(n) UKN_BOOL_##n

#define UKN_IIF_I(cond, t, f) UKN_IF_IIF_##cond(t, f)
#define UKN_IIF(cond, t, f) UKN_IIF_I(cond, t, f)

// condition select macro
// cond must be a number >= 0
// if cond = 0, then t is used
// otherwise f is used
#define UKN_IF(cond, t, f) UKN_IIF(UKN_BOOL_N(cond), t, f)

#define UKN_COMMA_IF(n) UKN_IF(n, UKN_EMPTY, UKN_COMMA)()

#define UKN_FUNCTION_M(n, d) UKN_COMMA_IF(n) d ## n

// cat
#define UKN_CAT(a, b) UKN_CAT_I(a, b)
#define UKN_CAT_I(a, b) a##b

namespace ukn {
    
    namespace detail {
        
        // any type holder
        struct auto_any_base { 
            operator bool() const {
                return false;
            }
        };
        
        template<typename T>
        struct auto_any: public auto_any_base {
            auto_any(const T& any): item(any) { }
            
            mutable T item;
        };
        
        
        // detect and hold type of an expr without eval it
        template<typename T> struct type2type {
            typedef typename T::iterator iterator;
            typedef typename T::const_iterator const_iterator;
            
            typedef typename T::reference reference;
            typedef typename T::const_reference const_reference;
        };
        
        template<typename T>
        type2type<T> encode_type(const T& t) {
            return type2type<T>();
        }         
        struct any_type {
            template<typename T>
            operator type2type<T>() const {
                return type2type<T>();
            }
        };
        
        // ? operator type trick
        // the macro actually returns type2type<TYPE_OF_CONTAINER>
        // since any_type can be cast to type2type
#define _for_each_EncodeTypeOf(container) (true ? ukn::detail::any_type(): ukn::detail::encode_type(container))
        
        // simple value holder for holding a container
        // with the _for_each_EvalRValue macro, the struct evaluates t is a rvalue or not
        // 
        //      (true ? sora::for_each::make_holder(expr, is_rvalue) : expr)
        // if T is a rvalue, then the type of expr is T, operator T() would be called and is_rvalue would be set to true
        // otherwise the type of expr is T&, operator T& is called
        template<typename T>
        struct value_holder {
            value_holder(T&t, bool& b):
            value(t),
            is_rvalue(b) { }
            
            T& value;
            bool& is_rvalue;
            
            operator T() {
                this->is_rvalue = true;
                return this->value;
            }
            
            operator T&() const {
                return this->value;
            }
        };
        
        // simple ref counted any holder
        struct simple_any {
            simple_any() {
                parent = 0;
                refCount = new int(1);
            }
            template<typename T>
            simple_any(const T& t) {
                parent = new holder<T>(t);
                refCount = new int(1);
            }
            
            simple_any(const simple_any& rhs) {
                parent = 0;
                refCount = new int(1);
                if(this != &rhs) {
                    release();
                    
                    refCount = rhs.refCount;
                    ++*refCount;
                    this->parent = rhs.parent;
                }
            }
            
            void release() {
                (*refCount)--;
                if(*refCount == 0) {
                    if(parent)
                        delete parent; 
                    delete refCount;
                    
                    parent = 0;
                    refCount = 0;
                }
            }
            
            ~simple_any() {
                release();
            }
            
            template<typename T>
            T& get() const {
                holder<T>* child = static_cast<holder<T>* >(parent);
                return child->value;
            }
            
            struct holder_parent {};
            
            template<typename T>
            struct holder: holder_parent {
                holder(const T& t):
                value(t) {
                    
                }
                
                T value;
            };
            
            holder_parent* parent;
            int* refCount;
        };
        
        
        template<typename Container>
        value_holder<Container> make_holder(Container& t, bool& is_rvalue) {
            return value_holder<Container>(t, is_rvalue);
        }
        
        template<typename Container>
        value_holder<Container> make_holder(const Container& t, bool& is_rvalue) {
            return value_holder<Container>(const_cast<Container&>(t), is_rvalue);
        }
        
        // ? operator trick 
        // determine the expr returns a rvalue or not without actually evaluate it
#define _for_each_EvalRValue(expr, is_rvalue) (true ? ukn::detail::make_holder(expr, is_rvalue) : expr)
        
        
        template<typename Container>
        auto_any<simple_any> make_container(const Container& t, bool& is_rvalue) {
            // if container expr is a rvalue (evaluated by _for_each_EvalRValue macro)
            // copy and store it in simple_any
            // otherwise store it as a pointer in simple_any
            return is_rvalue ? simple_any(t) : simple_any(&t);
        }
        
        template<typename Container>
        auto_any<simple_any> make_container(Container& t, bool& is_rvalue) {
            return is_rvalue ? simple_any(t) : simple_any(&t);
        }
        
        
        // cast auto_any_baes to auto_any<TYPE>
        template<typename T>
        T& auto_any_cast(const auto_any_base& base) {
            const auto_any<T>& value = static_cast<const auto_any<T>&>(base);
            return value.item;
        }
        
        // get begin iterator for container
        template<typename Container>
        auto_any<typename type2type<Container>::iterator> any_cont_begin(const auto_any_base& holder, bool is_rvalue, type2type<Container>) {
            const simple_any& val = auto_any_cast<simple_any>(holder);
            
            // if the container expr returns a rvalue, what we stored is a copy of it
            // otherwise we stored a pointer to it
            Container& cont = is_rvalue ? val.get<Container>() : *val.get<Container*>();
            return cont.begin();
        }
        
        // get end iterator for container
        template<typename Container>
        auto_any<typename type2type<Container>::iterator> any_cont_end(const auto_any_base& holder, bool is_rvalue, type2type<Container>) {
            const simple_any& val = auto_any_cast<simple_any>(holder);
            
            // if the container expr returns a rvalue, what we stored is a copy of it
            // otherwise we stored a pointer to it
            Container& cont = is_rvalue ? val.get<Container>() : *val.get<Container*>();
            return cont.end();
        }
        
        // increment the iterator
        template<typename Container>
        void any_cont_next(const auto_any_base& iter, type2type<Container>) {
            typedef typename type2type<Container>::iterator iter_type;
            ++auto_any_cast<iter_type>(iter);
        }
        
        // see if the iteration ends
        template<typename Container>
        bool any_cont_done(const auto_any_base& iter, const auto_any_base& end, type2type<Container>) {
            typedef typename type2type<Container>::iterator iter_type;
            return auto_any_cast<iter_type>(iter) == auto_any_cast<iter_type>(end);
        }
        
        // dereference the iterator to get the real value
        template<typename Container>
        typename type2type<Container>::reference auto_any_deref(const auto_any_base& iter, type2type<Container>) {
            typedef typename type2type<Container>::iterator iter_type;
            return *auto_any_cast<iter_type>(iter);
        }
        
        
#define UKN_FOR_EACH(item, container) \
    if(bool is_rvalue = false) {} else \
    if(const ukn::detail::auto_any_base& c = ukn::detail::make_container(_for_each_EvalRValue(container, is_rvalue), is_rvalue)) {} else \
    if(const ukn::detail::auto_any_base& b = ukn::detail::any_cont_begin(c, is_rvalue, _for_each_EncodeTypeOf(container))) {} else \
    if(const ukn::detail::auto_any_base& e = ukn::detail::any_cont_end(c, is_rvalue, _for_each_EncodeTypeOf(container))) {} else \
        for(bool more=true; more && !ukn::detail::any_cont_done(b, e, _for_each_EncodeTypeOf(container)); ukn::detail::any_cont_next(b, _for_each_EncodeTypeOf(container))) \
        if((more = false)) {} else \
        for(item=ukn::detail::auto_any_deref(b, _for_each_EncodeTypeOf(container)); !more; more=true)

        
    } // namespace detail

} // namespace ukn

#include "Platform.h"

namespace ukn {

    
}


#endif // Project_Unknown_Preprocessor_h
