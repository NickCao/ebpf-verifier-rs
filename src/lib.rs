autocxx::include_cpp!(
   #include "verifier.hpp"
   generate!("verify")
   safety!(unsafe)
);

#[cfg(test)]
mod test {
    #[test]
    fn test() {
        println!("{:?}", crate::ffi::verify("tests/demo.elf", ".text"));
    }
}
