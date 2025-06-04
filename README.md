# SymbolLocator

A Swift package for locating and dynamically linking to symbols in MachO frameworks at runtime.

## Overview

SymbolLocator provides a mechanism to access Darwin OS's non-public framework APIs through dynamic symbol resolution. It works by:

1. Creating symbol stubs that map to their corresponding implementations in frameworks
2. Dynamically loading the framework symbols at runtime using `dlopen` and `dlsym`
3. Supporting both x86_64 and ARM64 architectures

## Usage

### Basic Usage

```c
// Define a stub for a SwiftUI framework function
DEFINE_SL_STUB_SLF(CGSizeHasZero, SwiftUI, _$sSo6CGSizeV7SwiftUIE7hasZeroSbvg);

// The stub can now be called like a regular function
bool hasZero = CGSizeHasZero(size);
```

### Defining Stubs

SymbolLocator provides several macros for defining stubs:

- `DEFINE_SL_STUB_SLF(symbolName, framework, mangledName)` - For System Library Frameworks
- `DEFINE_SL_STUB_SLPF(symbolName, framework, mangledName)` - For Private System Library Frameworks
- `DEFINE_SL_STUB(symbolName, "path", mangledName)` - For any framework with custom path

### Safe Stubs

SymbolLocator offers "safe" versions of stub macros that handle symbol resolution failures gracefully:

- `DEFINE_SL_SAFE_STUB_SLF(symbolName, framework, mangledName)`
- `DEFINE_SL_SAFE_STUB_SLPF(symbolName, framework, mangledName)`
- `DEFINE_SL_SAFE_STUB(symbolName, "path", mangledName)`

When a symbol cannot be resolved at runtime, these safe stubs:
1. Log an error message with information about the missing symbol
2. Return without crashing instead of attempting to call a null function pointer
3. Provide runtime resilience against API changes or missing symbols

**Important Note**: Safe stubs only prevent crashes for functions that return `void`. For functions that return values, you must still check if the symbol exists before calling, as a missing implementation will cause undefined behavior when the caller attempts to use the return value.

### Manual Symbol Checking

For more robust safety that works with both void and value-returning functions, you can manually check if a symbol exists before calling it. Each stub automatically provides a corresponding pointer variable that you can check:

```swift
// Define stubs (safe or regular)
DEFINE_SL_SAFE_STUB_SLF(TestStub_UNKNOWN_SYMBOL_VOID, UIKit, _$s5UIKit14UpdateUIElementyyF);
DEFINE_SL_SAFE_STUB_SLF(TestStub_UNKNOWN_SYMBOL_CGCOLOR, UIKit, _$s5UIKit15GetBackgroundColorSo7CGColoraF);

// Import the function and its pointer
@_silgen_name("TestStub_UNKNOWN_SYMBOL_VOID")
func unknown_symbol_void()

@_silgen_name("TestStub_UNKNOWN_SYMBOL_VOID_ptr")
let unknown_symbol_void_ptr: UnsafeRawPointer?

@_silgen_name("TestStub_UNKNOWN_SYMBOL_CGCOLOR")
func unknown_symbol_cgColor() -> CGColor

@_silgen_name("TestStub_UNKNOWN_SYMBOL_CGCOLOR_ptr")
let unknown_symbol_cgColor_ptr: UnsafeRawPointer?

// Safe usage - works for both void and value-returning functions
if unknown_symbol_void_ptr != nil {
    unknown_symbol_void()
}

if unknown_symbol_cgColor_ptr != nil {
    let color = unknown_symbol_cgColor()
    // Use color safely
}
```

This approach provides complete safety for both void and value-returning functions by allowing you to verify symbol availability before attempting to call them.

### Finding Mangled Swift Symbols

To find the mangled name of a Swift symbol:

1. Use `nm -g /System/Library/Frameworks/Framework.framework/Framework | grep functionName`
2. Or examine framework symbols with Hopper Disassembler

## Architecture Support

SymbolLocator supports both: x86_64 and arm64 architectures.

## License

See [LICENSE](LICENSE) for details.