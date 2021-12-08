#include <iostream>
#include <vector>
#include "verifier.hpp"
#include "ebpf_verifier.hpp"
#include "main/utils.hpp"
#include "ebpf-verifier-rs/src/lib.rs.h"

using std::string;
using std::vector;

bool verify(rust::Str filename_r, rust::Str section_r) {
    string filename = string(filename_r);
    string section = string(section_r);
    ebpf_verifier_options_t ebpf_verifier_options = ebpf_verifier_default_options;
    ebpf_verifier_options.check_termination = true;
    ebpf_verifier_options.print_failures = true;
    ebpf_verifier_options.strict = true;

    const ebpf_platform_t* platform = &g_ebpf_platform_linux;

    vector<raw_program> raw_progs;
    try {
        raw_progs = read_elf(filename, section, &ebpf_verifier_options, platform);
    } catch (std::runtime_error& e) {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }

    raw_program raw_prog = raw_progs.back();
    std::variant<InstructionSeq, std::string> prog_or_error = unmarshal(raw_prog);
    if (std::holds_alternative<string>(prog_or_error)) {
        std::cout << "unmarshaling error at " << std::get<string>(prog_or_error) << "\n";
        return 1;
    }

    auto& prog = std::get<InstructionSeq>(prog_or_error);
    auto res = ebpf_verify_program(std::cout, prog, raw_prog.info, &ebpf_verifier_options, nullptr);
    return res;
}
