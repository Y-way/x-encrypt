
#include "Allocator.h"
#include <cstdlib>
#include <stdint.h>

#if defined(__ANDROID_API__) && (__ANDROID_API__ < 16)
static void* aligned_alloc(size_t alignment, size_t size)
{
    // alignment must be >= sizeof(void*)
    if(alignment < sizeof(void*))
    {
        alignment = sizeof(void*);
    }

    return memalign(alignment, size);
}

#elif defined(__APPLE__) || defined(__ANDROID__) || (defined(__linux__) && defined(__GLIBCXX__) && !defined(_GLIBCXX_HAVE_ALIGNED_ALLOC))

#if defined(__APPLE__)
#include <AvailabilityMacros.h>
#endif

static void* aligned_alloc(size_t alignment, size_t size)
{
    // Unfortunately, aligned_alloc causes VMA to crash due to it returning null pointers. (At least under 11.4)
    // Therefore, for now disable this specific exception until a proper solution is found.
    //#if defined(__APPLE__) && (defined(MAC_OS_X_VERSION_10_16) || defined(__IPHONE_14_0))
    //#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_16 || __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_14_0
    //    // For C++14, usr/include/malloc/_malloc.h declares aligned_alloc()) only
    //    // with the MacOSX11.0 SDK in Xcode 12 (which is what adds
    //    // MAC_OS_X_VERSION_10_16), even though the function is marked
    //    // availabe for 10.15. That is why the preprocessor checks for 10.16 but
    //    // the __builtin_available checks for 10.15.
    //    // People who use C++17 could call aligned_alloc with the 10.15 SDK already.
    //    if (__builtin_available(macOS 10.15, iOS 13, *))
    //        return aligned_alloc(alignment, size);
    //#endif
    //#endif

    // alignment must be >= sizeof(void*)
    if(alignment < sizeof(void*))
    {
        alignment = sizeof(void*);
    }

    void *pointer;
    if(posix_memalign(&pointer, alignment, size) == 0)
        return pointer;
    return NULL;
}
#endif

#if defined(_WIN32) || defined(WINDOWS)

static void* native_aligned_alloc(size_t alignment, size_t size)
{
    return _aligned_malloc(size, alignment);
}

static void* native_aligned_realloc(void* memory, size_t newSize, size_t alignment)
{
    return _aligned_realloc(memory, newSize, alignment);
}

static void native_aligned_free(void* ptr)
{
    _aligned_free(ptr);
}

#else

static void* native_aligned_alloc(size_t alignment, size_t size)
{
    return aligned_alloc(alignment, size);
}

static void* native_aligned_realloc(void* memory, size_t newSize, size_t alignment)
{
    void* newMemory = realloc(memory, newSize);

    // Fast path: realloc returned aligned memory
    if ((reinterpret_cast<uintptr_t>(newMemory) & (alignment - 1)) == 0)
        return newMemory;

    // Slow path: realloc returned non-aligned memory
    void* alignedMemory = native_aligned_alloc(newSize, alignment);
    memcpy(alignedMemory, newMemory, newSize);
    free(newMemory);
    return alignedMemory;
}

static void native_aligned_free(void* ptr)
{
    free(ptr);
}
#endif

namespace XMemory
{
    void* AlignedAlloc(size_t size, size_t alignment)
    {
        return native_aligned_alloc(size, alignment);
    }

    void* AlignedReAlloc(void* memory, size_t newSize, size_t alignment)
    {
        return native_aligned_realloc(memory, newSize, alignment);
    }

    void AlignedFree(void* memory)
    {
        return native_aligned_free(memory);
    }

    XMemoryCallbacks DefaultCallbacks =
    {
        malloc,
        XMemory::AlignedAlloc,
        free,
        XMemory::AlignedFree,
        calloc,
        realloc,
        XMemory::AlignedReAlloc
    };
}

namespace encrypt
{
    static XMemory::XMemoryCallbacks s_Callbacks = XMemory::DefaultCallbacks;

    void MemoryAllocator::SetMemoryCallbacks(XMemory::XMemoryCallbacks* callbacks)
    {
        memcpy(&s_Callbacks, callbacks, sizeof(XMemory::XMemoryCallbacks));
    }

    void* MemoryAllocator::Malloc(size_t size)
    {
        return s_Callbacks.malloc_func(size);
    }

    void* MemoryAllocator::AlignedMalloc(size_t size, size_t alignment)
    {
        return s_Callbacks.aligned_malloc_func(size, alignment);
    }

    void MemoryAllocator::Free(void* memory)
    {
        return s_Callbacks.free_func(memory);
    }

    void MemoryAllocator::AlignedFree(void* memory)
    {
        return s_Callbacks.aligned_free_func(memory);
    }

    void* MemoryAllocator::Calloc(size_t count, size_t size)
    {
        return s_Callbacks.calloc_func(count, size);
    }

    void* MemoryAllocator::Realloc(void* memory, size_t newSize)
    {
        return s_Callbacks.realloc_func(memory, newSize);
    }

    void* MemoryAllocator::AlignedRealloc(void* memory, size_t newSize, size_t alignment)
    {
        return s_Callbacks.aligned_realloc_func(memory, newSize, alignment);
    }
} /* namespace encrypt */
