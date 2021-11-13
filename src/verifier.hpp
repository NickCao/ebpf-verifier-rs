#pragma once

#include <iostream>
#include <vector>
#include "ebpf_verifier.hpp"
#include "main/memsize_linux.hpp"
#include "main/linux_verifier.hpp"
#include "main/utils.hpp"

using std::string;
using std::vector;

int add(int a, int b) {
    return a + b;
}

int verify(std::string filename, std::string section) {
    ebpf_verifier_options_t ebpf_verifier_options = ebpf_verifier_default_options;
    ebpf_verifier_options.check_termination = true;
    ebpf_verifier_options.assume_assertions = false;
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
    ebpf_verifier_stats_t verifier_stats;
    const auto [res, seconds] = timed_execution([&] {
        return ebpf_verify_program(std::cout, prog, raw_prog.info, &ebpf_verifier_options, &verifier_stats);
    });
    if (ebpf_verifier_options.check_termination && (ebpf_verifier_options.print_failures || ebpf_verifier_options.print_invariants)) {
        std::cout << "Program terminates within " << verifier_stats.max_instruction_count << " instructions\n";
    }
    std::cout << res << "," << seconds << "," << resident_set_size_kb() << "\n";
    return !res;
}
