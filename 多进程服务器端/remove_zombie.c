#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void read_childproc(int sig) {
    int status;
    pid_t pid;
    pid = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Remove process id: %d\n", pid);
        printf("child sen: %d\n", WEXITSTATUS(status));
    }
}

int main() {
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if (pid == 0) {
        puts("Hi, I am child process~");
        sleep(10);
        return 7;
    }
    else {
        printf("child process id: %d\n", pid);
        pid = fork();
        if (pid == 0) {
            puts("Hi, I am child process~");
            sleep(10);
            exit(24);
        }
        else {
            printf("child process id: %d\n", pid);
            for (int i = 0; i < 5; ++i) {
                puts("wait ...");
                sleep(5);
            }
        }
    }
    return 0;
}
