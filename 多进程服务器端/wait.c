#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int status;
    pid_t pid = fork();
    if (pid == 0)
        return 3;
    else {
        printf("child process one ID: %d\n", pid);
        pid = fork();
        if (pid == 0)
            exit(7);
        else {
            printf("child process two1 ID: %d\n", pid);
            wait(&status);
            if (WIFEXITED(status)) {
                printf("child sen one: %d\n", WEXITSTATUS(status));
            }
            printf("child process two2 ID: %d\n", wait(&status));
            if (WIFEXITED(status)) {
                printf("child sen two: %d\n", WEXITSTATUS(status));
            }

            sleep(30);
        }
    }
    return 0;
}
