import Testing
import SymbolLocator
import SymbolLocatorTestsSupport

#if canImport(SwiftUI, _underlyingVersion: 6.0.87)
import SwiftUI

extension CGSize {
    var hasZero: Bool {
        @_silgen_name("SymbolLocatorTestsSupportTestStub_CGSizeHasZero")
        get
    }
}

// Example for simple getter method
@Test
func sizeExample() async throws {
    let size1 = CGSize(width: 0, height: 0)
    let size2 = CGSize(width: 1, height: 0)
    let size3 = CGSize(width: 0, height: 1)
    let size4 = CGSize(width: 1, height: 1)
    #expect(size1.hasZero)
    #expect(size2.hasZero)
    #expect(size3.hasZero)
    #expect(!size4.hasZero)
}

enum Update {}

extension Update {
    @_silgen_name("SymbolLocatorTestsSupportTestStub_UpdateLocked")
    static func locked<T>(_ body: () throws -> T) rethrows -> T
}

// Example for generic and throwing function
@Test
func updateLockedExample() async throws {
    let result = Update.locked { 3 }
    #expect(result == 3)
    enum E: Error {
        case a, b
    }
    try await confirmation { confirm in
        do {
            let _ = try Update.locked { throw E.b }
        } catch E.b {
            confirm()
        }
    }
}

@_silgen_name("SymbolLocatorTestsSupportTestStub_UNKNOWN_SYMBOL_VOID")
func unknown_symbol_void()

@_silgen_name("SymbolLocatorTestsSupportTestStub_UNKNOWN_SYMBOL_CGCOLOR")
func unknown_symbol_cgColor() -> CGColor

@Test
func nonExistSymbol() {
    unknown_symbol_void()

    #if compiler(>=6.2) && os(macOS)
    await #expect(processExitsWith: .failure) {
        let color = unknown_symbol_cgColor()
        color.alpha
    }
    #endif
}


#endif
