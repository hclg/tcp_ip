#include <stdio.h>
#include <unistd.h>
#define mx 1024

int main() {
    char str1[] = "WHO ARE YOU!!";
    char str2[] = "IS YOUR FATHER";
    char buf[mx];
    int fds[2];
    pipe(fds);
    pid_t pid = fork();
    if (pid == 0) {
        write(fds[1], str1, sizeof(str1));
        sleep(2);
        read(fds[0], buf, mx);
        printf("child process output:%s\n", buf);
    }
    else {
        read(fds[0], buf, mx);
        printf("parent process output:%s\n", buf);
        write(fds[1], str2, sizeof(str2));
        sleep(3);
    }
    return 0;
}
