autocxx::include_cpp!(
   #include "verifier.hpp"
   generate!("verify")
   safety!(unsafe)
);

#[cfg(test)]
mod test {
    #[test]
    fn test() {
        assert!(crate::ffi::verify("tests/demo.o", ".text"));
        assert!(!crate::ffi::verify("tests/loop.o", ".text"));
    }
}
