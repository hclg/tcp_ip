#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <signal.h>

#define mx 100

void read_childproc(int sig) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    printf("REMOVE process : %d\n", pid);
}
void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void child_record(int fds, char *message) {
    FILE *fp = fopen("record.txt", "wt");
    int i = 0;
    fprintf(fp, "1号 玩家：\n");
    while (1) {
        int len = read(fds, message, mx);
        if (strcmp(message, "*") == 0) {
            i++;
            if (i == 3) break;
            fprintf(fp, "%d号 玩家：\n",i+1);
        }
        else
            fwrite((void*)message, 1, len, fp);
    }
    fclose(fp);
    return;
}

int main(int argc, char* argv[]) {
    int ser_sock, cli_sock;
    struct sockaddr_in ser_adr, cli_adr;
    char message[mx];
    socklen_t cli_sz;
    int fds[2], str_len, state;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0); ///信号处理
    pipe(fds);

    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_adr.sin_port = htons(atoi(argv[1]));

    if (bind(ser_sock, (struct sockaddr*) &ser_adr, sizeof(ser_adr)) == -1)
        error("bind() error");
    if(listen(ser_sock, 5) == -1)
        error("listen() error");
    pid_t pid = fork();
    if (pid == 0) {
        child_record(fds[0], message);
        return 0;
    }
    while (1) {
        cli_sz = sizeof(cli_adr);
        cli_sock = accept(ser_sock, (struct sockaddr*) &cli_adr, &cli_sz);
        if (cli_sock == -1)
            continue;
        else
            printf("new client connected ...\n");
        pid = fork();

        if (pid == 0) {
            close(ser_sock);
            while ((str_len = read(cli_sock, message, mx)) != 0) {
                write(cli_sock, message, str_len);
                write(fds[1], message, str_len);
            }
            write(fds[1], "*", 2);
            close(cli_sock);
            puts("client disconnected...");
            return 0;
        }
        else
            close(cli_sock);
    }
    close(ser_sock);
    return 0;
}
