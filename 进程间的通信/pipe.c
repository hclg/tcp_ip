#include <stdio.h>
#include <unistd.h>

#define mx 1024

int main() {
    pid_t pid;
    char str[] = "WHO ARE YOU!!";
    char buf[mx];
    int fds[2];
    pipe(fds);
    pid = fork();
    if (pid == 0)
        write(fds[1], str, sizeof(str));
    else {
        read(fds[0], buf, mx);
        puts(buf);
    }
    return 0;
}
