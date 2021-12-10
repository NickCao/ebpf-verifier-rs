fn main() {
    println!(
        "verification result is: {}",
        ebpf_verifier_rs::ffi::verify(&std::env::args().collect::<Vec<String>>()[1], ".text")
    );
}
