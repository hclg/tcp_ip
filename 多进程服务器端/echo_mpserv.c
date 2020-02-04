#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <signal.h>

#define mx 100

void read_childproc(int sig);
void error(char *message);

int main(int argc, char* argv[]) {
    int ser_sock, cli_sock;
    struct sockaddr_in ser_adr, cli_adr;
    int cli_sz, str_len, status;
    struct sigaction act;
    char message[mx];
    pid_t pid;

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);
    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_adr.sin_port = htons(atoi(argv[1]));

    if (bind(ser_sock, (struct sockaddr*) &ser_adr, sizeof(ser_adr)) == -1)
        error("bind() error");
    if (listen(ser_sock, 5) == -1)
        error("listen() error");
    while (1) {
        cli_sz = sizeof(cli_adr);
        cli_sock = accept(ser_sock, (struct sockaddr*) &cli_adr, &cli_sz);
        if (cli_sock == -1)
            continue;
        else
            puts("new client connected ...");
        pid = fork();
        if (pid == -1) {
            close(cli_sock);
            continue;
        }
        if (pid == 0){
            close(ser_sock);
            while ((str_len = read(cli_sock, message, sizeof(message))) != 0)
                write(cli_sock, message, str_len);
            close(cli_sock);
            puts("client disconnect ....");
            return 0;
        }
        else
            close(cli_sock);
    }
    close(ser_sock);
    return 0;
}

void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_childproc(int sig) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    printf("remove process id: %d", pid);
}
