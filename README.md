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

### Finding Mangled Swift Symbols

To find the mangled name of a Swift symbol:

1. Use `nm -g /System/Library/Frameworks/Framework.framework/Framework | grep functionName`
2. Or examine framework symbols with Hopper Disassembler

## Architecture Support

SymbolLocator supports both: x86_64 and arm64 architectures.

## License

See [LICENSE](LICENSE) for details.