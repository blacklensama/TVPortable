//
//  Memory.h
//  Project Unknown
//
//  Created by Robert Bu on 11/23/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_Memory_h
#define Project_Unknown_Memory_h

#include "UKN/Platform.h"

#ifndef UKN_OS_OSX
#include <memory.h>
#endif

#include <cstdlib>

namespace ukn {

    // memory allocator for sora core
    struct MemoryAllocFunc {
        typedef void* (*MallocFunc)(size_t);
        typedef void  (*FreeFunc)(void*);
        typedef void* (*ReallocFunc)(void*, size_t);

        MallocFunc  ukn_malloc;
        FreeFunc    ukn_free;
        ReallocFunc ukn_realloc;

        MemoryAllocFunc(MallocFunc ma,
                        FreeFunc fr,
                        ReallocFunc re):
        ukn_malloc(ma),
        ukn_free(fr),
        ukn_realloc(re) {}
    };

    static MemoryAllocFunc g_memory_allocator(::malloc,
                                              ::free,
                                              ::realloc);

    static void set_malloc_func(MemoryAllocFunc::MallocFunc ma) {
        g_memory_allocator.ukn_malloc = ma;
    }

    static void set_free_func(MemoryAllocFunc::FreeFunc fr) {
        g_memory_allocator.ukn_free = fr;
    }

    static void set_realloc_func(MemoryAllocFunc::ReallocFunc re) {
        g_memory_allocator.ukn_realloc = re;
    }

    inline void* ukn_malloc(size_t size) {
        return g_memory_allocator.ukn_malloc(size);
    }

    inline void ukn_free(void* ptr) {
        return g_memory_allocator.ukn_free(ptr);
    }

    inline void* ukn_realloc(void* ptr, size_t size) {
        return g_memory_allocator.ukn_realloc(ptr, size);
    }

#define ukn_malloc_t(t, size) \
    (t*)ukn_malloc(sizeof(t) * size);

} // namespace ukn

#endif
