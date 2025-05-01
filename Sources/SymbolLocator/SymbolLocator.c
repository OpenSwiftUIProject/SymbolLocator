//
//  SymbolLocator.c
//  SymbolLocator

#include "SymbolLocator.h"
#include <stdio.h>
#include <dlfcn.h>

void* findSymbolInFramework(void *handle, const char* frameworkPath, const char* mangledName) {
    if (!handle) {
        fprintf(stderr, "Error opening framework %s: %s\n", frameworkPath, dlerror());
        return NULL;
    }

    void* symbol = dlsym(handle, mangledName);
    if (!symbol) {
        fprintf(stderr, "Symbol %s not found in %s\n", mangledName, frameworkPath);
    }
    return symbol;
}
