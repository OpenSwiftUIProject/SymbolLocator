//
//  Log.m
//  SymbolLocator

#import "SymbolLocatorLog.h"
#import <Foundation/Foundation.h>

os_log_t symbol_locator_log(void) {
    static dispatch_once_t onceToken;
    static os_log_t log = NULL;
    dispatch_once(&onceToken, ^{
        log = os_log_create("org.OpenSwiftUIProject.SymbolLocator", "SymbolLocator");
    });
    return log;
}

void symbol_locator_log_error(const char * _Nonnull format, ...) {
    va_list args;
    va_start(args, format);
    os_log_with_type(symbol_locator_log(), OS_LOG_TYPE_ERROR, "%{public}@", [[NSString alloc] initWithFormat:[NSString stringWithUTF8String:format] arguments:args]);
    va_end(args);
}
