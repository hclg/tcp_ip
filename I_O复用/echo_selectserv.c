#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>
#define mx 1024

void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int ser_sock, cli_sock;
    struct sockaddr_in ser_adr, cli_adr;
    socklen_t cli_sz;
    char buf[mx];
    struct timeval timeout;
    int fd_max, fd_num, str_len;

    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_adr.sin_port = htons(atoi(argv[1]));
    if (bind(ser_sock, (struct sockaddr*) &ser_adr, sizeof(ser_adr)) == -1)
        error("bind() error");
    if (listen(ser_sock, 5) == -1)
        error("listen() error");
    fd_set reads, readcp;
    FD_ZERO(&reads);
    FD_SET(ser_sock, &reads);
    fd_max = ser_sock;

    while (1) {
        readcp = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        fd_num = select(fd_max+1, &readcp, 0, 0, &timeout);
        if (fd_num == -1)
            break;
        if (fd_num == 0)
            continue;
        for (int i = 0; i < fd_max+1; ++i) {
            if (FD_ISSET(i, &readcp)) {
                if (i == ser_sock) {
                    printf("server socket :%d\n", i);
                    cli_sz = sizeof(cli_adr);
                    cli_sock = accept(ser_sock, (struct sockaddr*) &cli_adr, &cli_sz);
                    FD_SET(cli_sock, &reads);
                    if (cli_sock > fd_max)
                        fd_max = cli_sock;
                    printf("client socket : %d\n", cli_sock);
                }
                else {
                    str_len = read(i, buf, mx);
                    if (str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("disconnect client : %d\n", i);
                    }
                    else
                        write(i, buf, str_len);
                }
            }
        }
    }
    close(ser_sock);
    return 0;
}
