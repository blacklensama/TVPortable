//
//  FunctionTemplate.h
//  zeromq_client
//
//  Created by Robert Bu on 8/16/11.
//  Copyright 2011 heizi. All rights reserved.
//

// this is a template header replys on macro definitions
// must not have multiple-inclusion protection

#include "UKN/Preprocessor.h"
#include "FunctionBase.h"
#include "FunctionFoward.h"

#include <cassert>

namespace ukn {

#ifdef UKN_FUNCTION_NUM_ARGS

    namespace detail {

#define UKN_FUNCTION_TEMPLATE_PARAM UKN_ENUM_PARAMS(UKN_FUNCTION_NUM_ARGS, typename T)

#define UKN_FUNCTION_TEMPLATE_ARGS UKN_ENUM_PARAMS(UKN_FUNCTION_NUM_ARGS, T)

#define UKN_FUNCTION_PARAM(n, d) UKN_CAT(T, n) UKN_CAT(a, n)
#define UKN_FUNCTION_PARAMS UKN_ENUM_N(UKN_FUNCTION_NUM_ARGS, UKN_FUNCTION_PARAM, UKN_EMPTY)

#define UKN_FUNCTION_ARGS UKN_ENUM_PARAMS(UKN_FUNCTION_NUM_ARGS, a)

#define UKN_INVOKER UKN_JOIN(invoker, UKN_FUNCTION_NUM_ARGS)
#define UKN_OBJ_INVOKER UKN_JOIN(obj_invoker, UKN_FUNCTION_NUM_ARGS)

#define UKN_GET_INVOKER UKN_JOIN(get_function_invoker, UKN_FUNCTION_NUM_ARGS)
#define UKN_GET_OBJ_INVOKER UKN_JOIN(get_function_obj_invoker, UKN_FUNCTION_NUM_ARGS)

#define UKN_FUNCTION UKN_JOIN(function, UKN_FUNCTION_NUM_ARGS)

#define UKN_FUNCTOR_RELEASE UKN_JOIN(functor_release, UKN_FUNCTION_NUM_ARGS)
#define UKN_GET_FUNCTOR_RELEASE UKN_JOIN(get_functor_release, UKN_FUNCTION_NUM_ARGS)

#if UKN_FUNCTION_NUM_ARGS == 0
#define UKN_FUNCTION_COMMA
#else
#define UKN_FUNCTION_COMMA ,
#endif

        template<typename FunctionPtr, typename R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_PARAM>
        struct UKN_INVOKER {
            static R invoke(any_ptr ptr UKN_FUNCTION_COMMA UKN_FUNCTION_PARAMS) {
                FunctionPtr f = reinterpret_cast<FunctionPtr>(ptr.func_ptr);
                return f(UKN_FUNCTION_ARGS);
            }
        };

        template<typename FunctionObj, typename R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_PARAM>
        struct UKN_FUNCTOR_RELEASE {
            static void release(any_ptr ptr) {
                FunctionObj* f = reinterpret_cast<FunctionObj*>(ptr.obj_ptr);
                delete f;
            }
        };

        template<typename FunctionObj, typename R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_PARAM>
        struct UKN_OBJ_INVOKER {
            static R invoke(any_ptr ptr UKN_FUNCTION_COMMA UKN_FUNCTION_PARAMS) {
                FunctionObj* f = reinterpret_cast<FunctionObj*>(ptr.obj_ptr);
                return (*f)(UKN_FUNCTION_ARGS);
            }
        };

        template<typename FunctionPtr UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_PARAM>
        struct UKN_INVOKER<FunctionPtr, void UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS> {
            static void invoke(any_ptr ptr UKN_FUNCTION_COMMA UKN_FUNCTION_PARAMS) {
                FunctionPtr f = reinterpret_cast<FunctionPtr>(ptr.func_ptr);
                f(UKN_FUNCTION_ARGS);
            }
        };

        template<typename FunctionObj UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_PARAM>
        struct UKN_OBJ_INVOKER<FunctionObj, void UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS> {
            static void invoke(any_ptr ptr UKN_FUNCTION_COMMA UKN_FUNCTION_PARAMS) {
                FunctionObj* f = reinterpret_cast<FunctionObj*>(ptr.obj_ptr);
                (*f)(UKN_FUNCTION_ARGS);
            }
        };

        template<typename FunctionPtr, typename R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_PARAM>
        struct UKN_GET_INVOKER {
            typedef UKN_INVOKER<FunctionPtr, R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS> type;
        };

        template<typename FunctionPtr, typename R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_PARAM>
        struct UKN_GET_OBJ_INVOKER {
            typedef UKN_OBJ_INVOKER<FunctionPtr, R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS> type;
        };

        template<typename FunctionObj, typename R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_PARAM>
        struct UKN_GET_FUNCTOR_RELEASE {
            typedef UKN_FUNCTOR_RELEASE<FunctionObj, R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS> type;
        };

        template<typename R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_PARAM>
        class UKN_FUNCTION {
        public:
            typedef UKN_FUNCTION<R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS> self_type;

            template<typename Functor>
            UKN_FUNCTION(const Functor& functor):
            mInvoker(0),
            mReleaser(0),
            mRefCount(NULL) {
                this->assign_to(functor);
            }

            UKN_FUNCTION():
            mInvoker(0),
            mReleaser(0),
            mRefCount(NULL) {

            }

            virtual ~UKN_FUNCTION() {
                clear();
            }

            void clear() {
                if(mInvoker && mRefCount) {
                    --*mRefCount;
                    if(*mRefCount <= 0) {
                        delete mRefCount;

                        if(mReleaser)
                            mReleaser(mPtr);
                    }
                    mRefCount = NULL;
                    mReleaser = NULL;
                    mInvoker = NULL;
                }
            }

            template<typename Functor>
            void assign_to(Functor f) {
                clear();

                typedef typename get_function_tag<Functor>::type tag;
                this->assign_to(f, tag());
            }

            inline R operator()(UKN_FUNCTION_PARAMS) const {
                assert(mInvoker);

                return mInvoker(mPtr UKN_FUNCTION_COMMA UKN_FUNCTION_ARGS);
            }

            operator bool() const {
                return mInvoker != NULL;
            }
            
            template<typename F>
            bool operator== (F f) {
                typedef typename get_function_tag<F>::type tag;
                return this->equalTo(tag());
            }
            
            template<typename F>
            bool equalTo(F f, function_ptr_tag tag) {
                return this->mPtr.func_ptr == f;
            }
            
            template<typename F>
            bool equalTo(F f, member_ptr_tag tag) {
                return this->equalTo(MemFun(f), function_obj_tag());
            }
            
            template<typename F>
            bool equalTo(F f, function_obj_tag tag) {
                return *(F*)this->mPtr.obj_ptr == f;
            }

            typedef R (*invoker_type)(any_ptr UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS);
            typedef void (*releaser_type)(any_ptr);

        private:

            template<typename Functor>
            void assign_to(Functor f, function_ptr_tag tag) {
                if(f) {
                    typedef typename UKN_GET_INVOKER<Functor, R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS>::type invoker_type;

                    mInvoker = &invoker_type::invoke;
                    mPtr = make_any_pointer((void (*)())f);
                }
            }

            template<typename Functor>
            void assign_to(Functor f, function_obj_tag tag) {
                typedef typename UKN_GET_OBJ_INVOKER<Functor, R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS>::type invoker_type;
                typedef typename UKN_GET_FUNCTOR_RELEASE<Functor, R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS>::type releaser_type;

                mInvoker = &invoker_type::invoke;
                mReleaser = &releaser_type::release;

                mPtr = make_any_pointer(new Functor(f));
                mRefCount = new int;
                *mRefCount = 1;
            }

            template<typename Functor>
            void assign_to(Functor f, member_ptr_tag tag) {
                this->assign_to(MemFun(f));
            }

            template<typename Functor>
            void assign_to(Functor f, function_obj_ref_tag tag) {
                // ?
            }

        protected:
            any_ptr mPtr;
            invoker_type mInvoker;
            releaser_type mReleaser;
            int* mRefCount;
        };

        template<UKN_FUNCTION_TEMPLATE_PARAM>
        class UKN_FUNCTION<void UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS> {
        public:
            typedef UKN_FUNCTION<void UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS> self_type;

            template<typename Functor>
            UKN_FUNCTION(const Functor& functor):
            mInvoker(0),
            mReleaser(0),
            mRefCount(NULL) {
                this->assign_to(functor);
            }

            UKN_FUNCTION():
            mInvoker(0),
            mReleaser(0),
            mRefCount(NULL) {

            }

            virtual ~UKN_FUNCTION() {
                clear();
            }

            void clear() {
                if(mInvoker && mRefCount) {
                    --*mRefCount;
                    if(*mRefCount <= 0) {
                        delete mRefCount;

                        if(mReleaser)
                            mReleaser(mPtr);
                    }
                    mRefCount = NULL;
                    mReleaser = NULL;
                    mInvoker = NULL;
                }
            }

            template<typename Functor>
            void assign_to(Functor f) {
                clear();

                typedef typename get_function_tag<Functor>::type tag;
                this->assign_to(f, tag());
            }

            inline void operator()(UKN_FUNCTION_PARAMS) const {
                assert(mInvoker);

                mInvoker(mPtr UKN_FUNCTION_COMMA UKN_FUNCTION_ARGS);
            }

            operator bool() const {
                return mInvoker != NULL;
            }
            
            template<typename F>
            bool operator== (F f) {
                typedef typename get_function_tag<F>::type tag;
                return this->equalTo(f, tag());
            }
            
            template<typename F>
            bool equalTo(F f, function_ptr_tag tag) {
                return this->mPtr.func_ptr == f;
            }
            
            template<typename F>
            bool equalTo(F f, member_ptr_tag tag) {
                return this->equalTo(MakeMemFun(f), function_obj_tag());
            }
            
            template<typename F>
            bool equalTo(F f, function_obj_tag tag) {
                return *(F*)this->mPtr.obj_ptr == f;
            }

            typedef void (*invoker_type)(any_ptr UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS);
            typedef void (*releaser_type)(any_ptr);

        private:

            template<typename Functor>
            void assign_to(Functor f, function_ptr_tag tag) {
                if(f) {
                    typedef typename UKN_GET_INVOKER<Functor, void UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS>::type invoker_type;

                    mInvoker = &invoker_type::invoke;
                    mPtr = make_any_pointer((void (*)())f);
                }
            }

            template<typename Functor>
            void assign_to(Functor f, function_obj_tag tag) {
                typedef typename UKN_GET_OBJ_INVOKER<Functor, void UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS>::type invoker_type;
                typedef typename UKN_GET_FUNCTOR_RELEASE<Functor, void UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS>::type releaser_type;


                mInvoker = &invoker_type::invoke;
                mReleaser = &releaser_type::release;

                mPtr = make_any_pointer(new Functor(f));
                mRefCount = new int(1);
            }

            template<typename Functor>
            void assign_to(Functor f, member_ptr_tag tag) {
                this->assign_to(MakeMemFun(f));
            }

            template<typename Functor>
            void assign_to(Functor f, function_obj_ref_tag tag) {
                // ?
            }

        protected:
            any_ptr mPtr;
            invoker_type mInvoker;
            releaser_type mReleaser;
            int* mRefCount;
        };


    } // namespace detail


    template<typename R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_PARAM>
    class Function<R(UKN_FUNCTION_TEMPLATE_ARGS)>: public detail::UKN_FUNCTION<R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS>
#if UKN_FUNCTION_NUM_ARGS == 1
    , public std::unary_function<T0, R>
#elif UKN_FUNCTOR_NUM_ARGS == 2
    , public std::binary_function<T0, T1, R>
#endif
    {
    public:
        typedef detail::UKN_FUNCTION<R UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS> base_type;
        typedef Function<R(UKN_FUNCTION_TEMPLATE_ARGS)> self_type;
#if UKN_FUNCTION_NUM_ARGS == 1
        typedef T0 argument_type;
#elif UKN_FUNCTOR_NUM_ARGS == 2
        typedef T0 first_argument_type;
        typedef T1 second_argument_type;
#endif

        template<typename Functor>
        Function(Functor f):
        base_type(f) {

        }

        Function():
        base_type() {

        }

        template<typename Functor>
        Function<R(UKN_FUNCTION_TEMPLATE_ARGS)>& operator=(Functor f) {
            base_type::assign_to(f);
            return *this;
        }


        self_type& operator=(const self_type& rhs) {
            if(this != &rhs) {
                base_type::clear();

                this->mPtr = rhs.mPtr;
                this->mInvoker = rhs.mInvoker;
                this->mReleaser = rhs.mReleaser;
                this->mRefCount = rhs.mRefCount;

                if(this->mRefCount != NULL) {
                    ++*this->mRefCount;
                }
            }
            return *this;
        }

        Function(const self_type& rhs) {
            if(this != &rhs) {
                base_type::clear();

                this->mPtr = rhs.mPtr;
                this->mInvoker = rhs.mInvoker;
                this->mReleaser = rhs.mReleaser;
                this->mRefCount = rhs.mRefCount;

                if(this->mRefCount != NULL) {
                    ++*this->mRefCount;
                }
            }
        }
    };

    template<UKN_FUNCTION_TEMPLATE_PARAM>
    class Function<void(UKN_FUNCTION_TEMPLATE_ARGS)>: public detail::UKN_FUNCTION<void UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS>
#if UKN_FUNCTION_NUM_ARGS == 1
    , public std::unary_function<T0, void>
#elif UKN_FUNCTOR_NUM_ARGS == 2
    , public std::binary_function<T0, T1, void>
#endif
    {
    public:
        typedef detail::UKN_FUNCTION<void UKN_FUNCTION_COMMA UKN_FUNCTION_TEMPLATE_ARGS> base_type;
        typedef Function<void(UKN_FUNCTION_TEMPLATE_ARGS)> self_type;
#if UKN_FUNCTION_NUM_ARGS == 1
        typedef T0 argument_type;
#elif UKN_FUNCTOR_NUM_ARGS == 2
        typedef T0 first_argument_type;
        typedef T1 second_argument_type;
#endif

        template<typename Functor>
        Function(Functor f):
        base_type(f) {

        }

        Function():
        base_type() {

        }

        template<typename Functor>
        Function<void(UKN_FUNCTION_TEMPLATE_ARGS)>& operator=(Functor f) {
            base_type::assign_to(f);
            return *this;
        }


        self_type& operator=(const self_type& rhs) {
            if(this != &rhs) {
                base_type::clear();

                this->mPtr = rhs.mPtr;
                this->mInvoker = rhs.mInvoker;
                this->mReleaser = rhs.mReleaser;
                this->mRefCount = rhs.mRefCount;

                if(this->mRefCount != NULL) {
                    ++*this->mRefCount;
                }
            }
            return *this;
        }

        Function(const self_type& rhs) {
            if(this != &rhs) {
                base_type::clear();

                this->mPtr = rhs.mPtr;
                this->mInvoker = rhs.mInvoker;
                this->mReleaser = rhs.mReleaser;
                this->mRefCount = rhs.mRefCount;

                if(this->mRefCount != NULL) {
                    ++*this->mRefCount;
                }
            }
        }
    };

#undef UKN_FUNCTION_TEMPLATE_PARAM
#undef UKN_FUNCTION_TEMPLATE_ARGS
#undef UKN_FUNCTION_PARAMS
#undef UKN_FUNCTION_PARAM
#undef UKN_FUNCTION_ARGS
#undef UKN_INVOKER
#undef UKN_OBJ_INVOKER
#undef UKN_GET_INVOKER
#undef UKN_GET_OBJ_INVOKER
#undef UKN_FUNCTION_COMMA
#undef UKN_FUNCTION
#undef UKN_GET_FUNCTOR_RELEASE
#undef UKN_FUNCTOR_RELEASE

#endif // UKN_FUNCTION_NUM_ARGS


} // namespace sora
