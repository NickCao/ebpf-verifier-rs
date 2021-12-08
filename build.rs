fn main() {
    cxx_build::bridge("src/lib.rs")
        .file("src/verifier.cpp")
        .flag_if_supported("-std=c++17")
        .flag_if_supported("-O2")
        .compile("cxxbridge");

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=src/verifier.hpp");
    println!("cargo:rerun-if-changed=src/verifier.cpp");
    println!("cargo:rustc-link-lib=ebpfverifier");
}
