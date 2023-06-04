#pragma once

#include "config.h"

namespace XMemory
{
    void* AlignedAlloc(size_t size, size_t alignment);
    void* AlignedReAlloc(void* memory, size_t newSize, size_t alignment);
    void AlignedFree(void* memory);

    typedef struct
    {
        void* (*malloc_func)(size_t size);
        void* (*aligned_malloc_func)(size_t size, size_t alignment);
        void (*free_func)(void* ptr);
        void (*aligned_free_func)(void* ptr);
        void* (*calloc_func)(size_t nmemb, size_t size);
        void* (*realloc_func)(void* ptr, size_t size);
        void* (*aligned_realloc_func)(void* ptr, size_t size, size_t alignment);
    } XMemoryCallbacks;

    extern XMemoryCallbacks DefaultCallbacks;
}

namespace encrypt
{
    struct XENCRYPT_API MemoryAllocator
    {
        static void SetMemoryCallbacks(XMemory::XMemoryCallbacks* callbacks);

        static void* Malloc(size_t size);
        static void* AlignedMalloc(size_t size, size_t alignment);
        static void Free(void* memory);
        static void AlignedFree(void* memory);
        static void* Calloc(size_t count, size_t size);
        static void* Realloc(void* memory, size_t newSize);
        static void* AlignedRealloc(void* memory, size_t newSize, size_t alignment);
    };
} /* namespace encrypt */

#define XMEMORY_MALLOC(size) encrypt::MemoryAllocator::Malloc(size)
#define XMEMORY_MALLOC_ALIGNED(size, alignment) encrypt::MemoryAllocator::AlignedMalloc(size, alignment)
#define XMEMORY_MALLOC_ZERO(size) encrypt::MemoryAllocator::Calloc(1,size)
#define XMEMORY_FREE(memory) encrypt::MemoryAllocator::Free(memory)
#define XMEMORY_FREE_ALIGNED(memory) encrypt::MemoryAllocator::AlignedFree(memory)
#define XMEMORY_CALLOC(count, size) encrypt::MemoryAllocator::Calloc(count,size)
#define XMEMORY_REALLOC(memory, newSize) encrypt::MemoryAllocator::Realloc(memory,newSize)
#define XMEMORY_REALLOC_ALIGNED(memory, newSize, alignment) encrypt::MemoryAllocator::AlignedRealloc(memory, newSize, alignment)

#define XMEMORY_SAFE_FREE(memory) \
{\
    void* pMem = (void*)(memory); \
    if(pMem != nullptr) \
        encrypt::MemoryAllocator::Free(pMem); \
    pMem = nullptr; \
}
