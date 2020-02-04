#include <stdio.h>
#include <unistd.h>
#define mx 1024

int main() {
    char str1[] = "WHO ARE YOU!!";
    char str2[] = "IS YOUR FATHER";
    char buf[mx];
    int fds1[2], fds2[2];
    pipe(fds1);
    pipe(fds2);
    pid_t pid = fork();
    if (pid == 0) {
        write(fds1[1], str1, sizeof(str1));
        read(fds2[0], buf, mx);
        printf("child process output:%s\n", buf);
    }
    else {
        read(fds1[0], buf, mx);
        printf("parent process output:%s\n", buf);
        write(fds2[1], str2, sizeof(str2));
    }
    return 0;
}

