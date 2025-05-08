//
//  SymbolLocator.h
//  SymbolLocator

#ifndef SYMBOL_LOCATOR_H
#define SYMBOL_LOCATOR_H

#include "metamacros.h"
#include <dlfcn.h>
#include <stddef.h>
#include "SymbolLocatorLog.h"

#if defined(__cplusplus)
#define SL_EXPORT extern "C"
#else
#define SL_EXPORT extern
#endif

#define SL_SLF_NAME(X) "/System/Library/Frameworks/" #X ".framework/" #X
#define SL_SLPF_NAME(X) "/System/Library/PrivateFrameworks/" #X ".framework/" #X

#define _SL_STUB_NAME(X) X
#define _SL_STUB_TARGET_NAME(X) metamacro_concat(_SL_STUB_NAME(X), _ptr)

#define _SL_SYMBOL(X) metamacro_concat(_, X)
#define _SL_STUB_SYMBOL(X) _SL_SYMBOL(_SL_STUB_NAME(X))
#define _SL_STUB_TARGET_SYMBOL(X) _SL_SYMBOL(_SL_STUB_TARGET_NAME(X))
#define _SL_STUB_LOADER(X) metamacro_concat(_SL_STUB_NAME(X), Loader)

void* findSymbolInFramework(void *handle, const char* frameworkPath, const char* mangledName);

// Define a symbol stub that maps a function to its implementation

#define DEFINE_SL_STUB_SLF(symbolName, framework, mangledName) \
    DEFINE_SL_STUB(symbolName, \
        SL_SLF_NAME(framework), \
        mangledName \
    )

#define DEFINE_SL_STUB_SLPF(symbolName, framework, mangledName) \
    DEFINE_SL_STUB(symbolName, \
        SL_SLPF_NAME(framework), \
        mangledName \
    )

#define DEFINE_SL_SAFE_STUB_SLF(symbolName, framework, mangledName) \
    DEFINE_SL_SAFE_STUB(symbolName, \
        SL_SLF_NAME(framework), \
        mangledName \
    )

#define DEFINE_SL_SAFE_STUB_SLPF(symbolName, framework, mangledName) \
    DEFINE_SL_SAFE_STUB(symbolName, \
        SL_SLPF_NAME(framework), \
        mangledName \
    )

// Define common stub code for all architectures
#define _SL_STUB_COMMON(symbolName, frameworkPath, mangledName) \
    SL_EXPORT void* _SL_STUB_TARGET_NAME(symbolName); \
    void *_SL_STUB_TARGET_NAME(symbolName) = 0; \
    __attribute__((constructor)) \
    static void _SL_STUB_LOADER(symbolName)(void) { \
        void* handle = dlopen(frameworkPath, RTLD_LAZY); \
        _SL_STUB_TARGET_NAME(symbolName) = findSymbolInFramework(handle, frameworkPath, #mangledName); \
        if (handle != NULL) { \
            dlclose(handle); \
        } \
    }

#if defined(__x86_64__) || defined(__i386__)
#define _SL_STUB_ASM(symbolName) \
    __asm__ (metamacro_stringify(.global _SL_STUB_SYMBOL(symbolName))); \
    __asm__ (metamacro_stringify(_SL_STUB_SYMBOL(symbolName) :)); \
    __asm__ (metamacro_stringify(jmpq    * _SL_STUB_TARGET_SYMBOL(symbolName)(%rip)));

#elif defined(__arm64__) || defined(__aarch64__)
#define _SL_STUB_ASM(symbolName) \
    __attribute__((visibility("default"))) \
    __attribute__((used)) \
    __asm__( \
        ".text\n" \
        ".global _" #symbolName "\n" \
        ".align 2\n" \
        "_" #symbolName ":\n" \
        "adrp x16, _" #symbolName "_ptr@PAGE\n" \
        "ldr x16, [x16, _" #symbolName "_ptr@PAGEOFF]\n" \
        "br x16\n" \
    );
#else
#error "Architecture not supported"
#endif

#if defined(__x86_64__) || defined(__i386__)
#define _SL_SAFE_STUB_ASM(symbolName) \
    __asm__ (metamacro_stringify(.global _SL_STUB_SYMBOL(symbolName))); \
    __asm__ (metamacro_stringify(_SL_STUB_SYMBOL(symbolName) :)); \
    __asm__ (metamacro_stringify(cmpq    $0, _SL_STUB_TARGET_SYMBOL(symbolName)(%rip))); \
    __asm__ (metamacro_stringify(jne     2f)); \
    __asm__ (metamacro_stringify(leaq    L ## symbolName ## _str(%rip), %rdi)); \
    __asm__ (metamacro_stringify(jmp     _symbol_locator_log_error)); \
    __asm__ (metamacro_stringify(2:)); \
    __asm__ (metamacro_stringify(jmpq    *_SL_STUB_TARGET_SYMBOL(symbolName)(%rip))); \
    __asm__ (metamacro_stringify(.data)); \
    __asm__ (metamacro_stringify(L ## symbolName ## _str:)); \
    __asm__ (metamacro_stringify(.asciz "Failed to resolve symbol: ) #symbolName")); \
    __asm__ (metamacro_stringify(.text));

#elif defined(__arm64__) || defined(__aarch64__)
#define _SL_SAFE_STUB_ASM(symbolName) \
    __attribute__((visibility("default"))) \
    __attribute__((used)) \
    __asm__( \
        ".text\n" \
        ".global _" #symbolName "\n" \
        ".align 2\n" \
        "_" #symbolName ":\n" \
        "adrp x16, _" #symbolName "_ptr@PAGE\n" \
        "ldr x16, [x16, _" #symbolName "_ptr@PAGEOFF]\n" \
        "cbnz x16, 1f\n" \
        "adrp x0, L" #symbolName "_str@PAGE\n" \
        "add x0, x0, L" #symbolName "_str@PAGEOFF\n" \
        "b _symbol_locator_log_error\n" \
        "1:\n" \
        "br x16\n" \
        ".data\n" \
        ".align 3\n" \
        "L" #symbolName "_str:\n" \
        ".asciz \"Failed to resolve symbol: " #symbolName "\"\n" \
        ".text\n" \
    );
#else
#error "Architecture not supported"
#endif

// Combined macro that uses the common part and architecture-specific assembly
#define DEFINE_SL_STUB(symbolName, frameworkPath, mangledName) \
    _SL_STUB_COMMON(symbolName, frameworkPath, mangledName) \
    _SL_STUB_ASM(symbolName)
#define DEFINE_SL_SAFE_STUB(symbolName, frameworkPath, mangledName) \
    _SL_STUB_COMMON(symbolName, frameworkPath, mangledName) \
    _SL_SAFE_STUB_ASM(symbolName)

#endif /* SYMBOL_LOCATOR_H */
