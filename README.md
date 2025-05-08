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

```c
// Define a safe stub for a void-returning function
DEFINE_SL_SAFE_STUB_SLF(TestStub_UNKNOWN_SYMBOL_VOID, UIKit, _$s5UIKit14UpdateUIElementyyF);

@_silgen_name("TestStub_UNKNOWN_SYMBOL_VOID")
func unknown_symbol_void()

unknown_symbol_void()
// This will not crash even if _$s5UIKit14UpdateUIElementyyF cannot be resolved in UIKit at runtime
```

### Finding Mangled Swift Symbols

To find the mangled name of a Swift symbol:

1. Use `nm -g /System/Library/Frameworks/Framework.framework/Framework | grep functionName`
2. Or examine framework symbols with Hopper Disassembler

## Architecture Support

SymbolLocator supports both: x86_64 and arm64 architectures.

## License

See [LICENSE](LICENSE) for details.