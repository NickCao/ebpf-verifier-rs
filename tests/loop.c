static long (*bpf_trace_printk)(const char* fmt, int fmt_size, long p1, long p2, long p3) = (void*)6;

long program() {
    char fmt[] = "number is: %d";
    for (int i = 0; 1; i++) {
        bpf_trace_printk(fmt, 13, i, 0, 0);
    }
    return 0;
}
