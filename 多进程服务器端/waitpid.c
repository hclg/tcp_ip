#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        sleep(10);
        return 24;
    }
    else {
        while (!waitpid(-1, &status, WNOHANG)){
            sleep(2);
            puts("sleep(2)");
        }
        printf("child send %d\n", WEXITSTATUS(status));
    }
    return 0;
}
