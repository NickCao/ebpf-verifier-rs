#![no_std]

#[cxx::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!("ebpf-verifier-rs/src/verifier.hpp");
        fn verify(filename: &str, section: &str) -> bool;
    }
}

#[cfg(test)]
mod test {
    #[test]
    fn test() {
        assert!(crate::ffi::verify("tests/demo.o", ".text"));
        assert!(!crate::ffi::verify("tests/loop.o", ".text"));
    }
}
