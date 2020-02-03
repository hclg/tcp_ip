#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid) {
        printf("child process ID: %d\n", pid);
        sleep(30);
    }
    else
        printf("Hi, I am a child process");
    if (pid)
        printf("END parent process\n");
    else
        printf("END child process\n");
    return 0;
}
