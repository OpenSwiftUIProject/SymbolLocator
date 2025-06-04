// swift-tools-version: 6.0
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "SymbolLocator",
    products: [
        .library(
            name: "SymbolLocator",
            targets: ["SymbolLocator"]
        ),
    ],
    targets: [
        .target(
            name: "SymbolLocator",
            publicHeadersPath: "include"
        ),
        .target(
            name: "SymbolLocatorTestsSupport",
            dependencies: ["SymbolLocator"],
            publicHeadersPath: "."
        ),
        .testTarget(
            name: "SymbolLocatorTests",
            dependencies: [
                "SymbolLocator",
                "SymbolLocatorTestsSupport"
            ],
            swiftSettings: [
                .swiftLanguageMode(.v5),
            ]
        ),
    ]
)
