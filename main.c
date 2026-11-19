//Author: Levi Amendt levi.amendt@gmail.com

long write(char* str, long len);
long read(char* buf, long len);
void exit();

    __attribute__((section(".text.startup"), noreturn))
void _start(void) {
    //First line of code executed goes here
    exit();
    __builtin_unreachable();
}

long write(char* str, long len) {
    long n;
    asm volatile("syscall": "=a"(n): "a"(1), "D"(1), "S"(str), "d"(len): "rcx", "r11", "memory");
    return n;
}

long read(char* buf, long len) {
    long n;
    asm volatile("syscall": "=a"(n): "a"(0), "D"(0), "S"(buf), "d"(len): "rcx", "r11", "memory");
    buf[n] = '\0';
    return n;
}

void exit() {
    asm volatile("syscall": : "a"(60), "D"(0): "rcx", "r11");
    return;
}