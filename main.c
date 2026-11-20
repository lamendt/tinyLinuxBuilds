//Author: Levi Amendt levi.amendt@gmail.com
#define BUF_SIZE 512
#define TOK_NUM 64
#define CMD_NUM 2

static long write(char* str, long len);
static long read(char* buf, long len);
static long fork();
static long exec(char* path, char* argv[], char* envp[]);
static void exit();
static long wait(long pid, int options);
static long chDir(char* path);

static void tok(char *buf);
static void prompt();
static void run(char* name, char* args[]);
static void prints(char* str);
static char strcmp(char* s1, char* s2);
static char* strcat(char* start, char* end);
static void cd();

char* toks[TOK_NUM];
char binpath[BUF_SIZE] = {"/bin/"};
char inputBuf[BUF_SIZE];
char** envp;

    __attribute__((section(".text.startup"), noreturn))
void _start(void) {
    asm("lea 8(%%rsp), %0" : "=r"(envp));
    while (*envp++);

    //First line of code executed goes here
    while(1) {
        prompt();
    }
    __builtin_unreachable();
}

void tok(char* buf) {
    short n = 0;
    char whitespace = 1;
    while (*buf != '\0') {
        if (*buf <= ' ') {
            *buf = '\0'; 
            whitespace = 1;
        } else {
            if (whitespace == 1) {
                toks[n++] = buf;
            }
            whitespace = 0;
        }
        buf++;
    }
    toks[n] = 0;
    return;
}

void prompt() {
    prints("$ ");
    short n = read(inputBuf, BUF_SIZE);
    if (n==1) return;
    tok(inputBuf);
    if (strcmp("exit", toks[0]))
        exit();
    if (strcmp("cd", toks[0])) {
        cd();
        return;
    }
    run(toks[0], &toks[0]);
    return;
}

void run(char* name, char* args[]) {
    short i;
    long pid = fork();
    if (pid == 0) {
        exec(name, args, envp);
        exec(strcat(binpath,name), args, envp);
        prints("Error\n");
        exit();
    }
    else {
        wait(pid, 0);
    }
    return;
}

void prints(char* str) {
    char len = 0;
    for (; str[len] != '\0'; len++);
    write(str, len);
    return;
}

char strcmp(char* str1, char* str2) {
    short i = 0;
    while (str1[i] == str2[i] && str1[i] != '\0') {
        i++;
    }
    if (str1[i] == '\0') return 1;
    return 0;
}

char* strcat(char* start, char* end) {
    char* s = start;
    while (*start != '\0') {
        start++;
    }
    while (*end != '\0') {
        *start = *end;
        start++; end++;
    }
    return s;
}

void cd() {
    chDir(toks[1]);
}

long chDir(char* path) {
    long n;
    asm volatile("syscall": "=a"(n): "a"(80), "D"(path): "rcx", "r11", "memory");
    return n;
}

long write(char* str, long len) {
    long n;
    asm volatile("syscall": "=a"(n): "a"(1), "D"(1), "S"(str), "d"(len): "rcx", "r11", "memory");
    return n;
}

long read(char* buf, long len) {
    long n;
    asm volatile("syscall": "=a"(n): "a"(0), "D"(0), "S"(buf), "d"(len): "rcx", "r11", "memory");
    buf[n - 1] = '\0';
    return n;
}

long fork() {
    long n;
    asm volatile("syscall": "=a"(n): "a"(57): "rcx", "r11", "memory");
    return n;
}

long exec(char* path, char* argv[], char* envp[]) {
    long n;
    asm volatile("syscall": "=a"(n): "a"(59), "D"(path), "S"(argv), "d"(envp): "rcx", "r11", "memory");
    return n;
}

void exit() {
    asm volatile("syscall": : "a"(60), "D"(0): "rcx", "r11");
    return;
}

long wait(long pid, int options) {
    long n;
    register long r10 asm("r10") = 0;
    asm volatile("syscall": "=a"(n): "a"(61), "D"(pid), "S"(0), "d"(options), "r"(r10): "rcx", "r11", "memory");
    return n;
}