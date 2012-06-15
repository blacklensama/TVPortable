//
//  FunctionImpl.h
//  zeromq_client
//
//  Created by Robert Bu on 8/15/11.
//  Copyright 2011 heizi. All rights reserved.
//

#ifndef ukn_FunctionImpl_h
#define ukn_FunctionImpl_h

#include "UKN/detail/TypeTraits.h"

namespace ukn {
    
    namespace detail {
        
        union any_ptr {
            void* obj_ptr;
            const void* const_obj_ptr;
            
            void (*func_ptr)();
            
            char data;
        };
        
        struct function_ptr_tag {};
        struct function_obj_tag {};
        struct member_ptr_tag {};
        struct function_obj_ref_tag {};
        
        template<typename F>
        class get_function_tag {
            typedef typename traits::if_c<(traits::is_pointer<F>::value),
                                            function_ptr_tag,
                                            function_obj_tag>::type ptr_or_obj_tag;
            
            typedef typename traits::if_c<(traits::is_member_pointer<F>::value),
                                            member_ptr_tag,
                                            ptr_or_obj_tag>::type ptr_or_obj_or_mem_tag;
            
            typedef typename traits::if_c<(traits::is_reference_wrapper<F>::value),
                                            function_obj_ref_tag,
                                            ptr_or_obj_or_mem_tag>::type or_ref_tag;
            
        public:
            typedef or_ref_tag type;
        };  
        
        inline any_ptr make_any_pointer(void* o) {
            any_ptr ptr;
            ptr.obj_ptr = o;
            return ptr;
        }
        
        inline any_ptr make_any_pointer(const void* o) {
            any_ptr ptr;
            ptr.const_obj_ptr = o;
            return ptr;
        }
        
        inline any_ptr make_any_pointer(void (*fn)()) {
            any_ptr ptr;
            ptr.func_ptr = fn;
            return ptr;
        }
        
    } // namespace ukn
    
} // namespace ukn


#endif
