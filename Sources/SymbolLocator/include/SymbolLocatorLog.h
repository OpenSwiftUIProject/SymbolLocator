//
//  SymbolLocator.h
//  SymbolLocator

#ifndef SYMBOL_LOCATOR_LOG_H
#define SYMBOL_LOCATOR_LOG_H

#include <CoreFoundation/CoreFoundation.h>
#include <os/log.h>

CF_ASSUME_NONNULL_BEGIN

CF_EXPORT
os_log_t symbol_locator_log(void);

CF_EXPORT
void symbol_locator_log_error(const char *format, ...) CF_FORMAT_FUNCTION(1, 2);

CF_ASSUME_NONNULL_END

#endif /* SYMBOL_LOCATOR_LOG_H */
