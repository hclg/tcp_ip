#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <signal.h>

#define mx 100
void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
void read_childproc(int sig)  {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    printf("child id: %d\n", pid);
}
void read_routine(int sock, char *buf) {
    int len;
    while ((len = read(sock, buf, sizeof(buf))) != 0) {
        printf("SERVER OUT: %s", buf);
    }
}

void write_routine(int sock, char *buf) {
    while (1) {
        fgets(buf, mx, stdin);
        if (strcmp(buf, "q\n") == 0) {
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buf, sizeof(buf));
    }
}

int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in ser_adr;
    char buf[mx];

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    inet_aton(argv[1], &ser_adr.sin_addr);
    ser_adr.sin_port = htons(atoi(argv[2]));
    if (connect(sock, (struct sockaddr*) &ser_adr, sizeof(ser_adr)) == -1)
        error("connect() error");
    pid_t pid = fork();
    if (pid)
        read_routine(sock, buf);
    else
        write_routine(sock, buf);
    close(sock);
    return 0;

}
