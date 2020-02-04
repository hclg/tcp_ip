#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
void timeout(int sig) {
    if (sig == SIGALRM)
        puts("time out!");
    alarm(2);
}

void keycontrol(int sig) {
    if (sig == SIGINT)
        puts("CTRL C pressed");
}

void child(int sig) {
    if (sig == SIGCHLD)
        puts("child ----");
}

int main() {
    int i, status;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    signal(SIGCHLD, child);
    alarm(2);
    for (int i = 0; i < 3; ++i) {
        puts("wait...");
        sleep(100);
    }
    pid_t pid = fork();
    if (pid == 0)
        return 3;
    else {

        while (!waitpid(pid, &status, WNOHANG)) {
            puts("wait!!!");
            sleep(2);
        }
        printf("child %d\n", WEXITSTATUS(status));
        sleep(100);
    }
    return 0;
}
