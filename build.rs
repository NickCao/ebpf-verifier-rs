fn main() {
    autocxx_build::Builder::new("src/lib.rs", &["src/"])
        .extra_clang_args(&["-std=c++17"])
        .expect_build()
        .compile("verifier");
    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=src/verifier.hpp");
    println!("cargo:rustc-link-lib=ebpfverifier");
}
