import Testing
import SymbolLocator
import SymbolLocatorTestsSupport
import Foundation

extension CGSize {
    var hasZero: Bool {
        @_silgen_name("SymbolLocatorTestsSupportTestStub_CGSizeHasZero")
        get
    }
}

@Test func example() async throws {
    let size1 = CGSize(width: 0, height: 0)
    let size2 = CGSize(width: 1, height: 0)
    let size3 = CGSize(width: 0, height: 1)
    let size4 = CGSize(width: 1, height: 1)
    #expect(size1.hasZero)
    #expect(size2.hasZero)
    #expect(size3.hasZero)
    #expect(!size4.hasZero)
}
