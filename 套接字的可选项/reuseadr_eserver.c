#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define mx 1024
void error_handling(char *message);


int main(int argc, char* argv[]) {
    int ser_sock, cli_sock;
    struct sockaddr_in ser_adr, cli_adr;
    socklen_t cli_adr_sz;
    int state, str_len;
    char message[mx];
    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (ser_sock == -1)
        error_handling("socket() error");
    int option;
    int optlen = sizeof(option);
    option = 1;
    state = setsockopt(ser_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);
    if (state)
        error_handling("setsockopt() error");
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_adr.sin_port = htons(atoi(argv[1]));
    if (bind(ser_sock, (struct sockaddr*)&ser_adr, sizeof(ser_adr)) == -1)
        error_handling("bind() error");
    if (listen(ser_sock, 5) == -1)
        error_handling("listen() error");
    cli_adr_sz = sizeof(cli_adr);
    cli_sock = accept(ser_sock, (struct sockaddr*)&cli_adr, &cli_adr_sz);
    if (cli_sock == -1)
        error_handling("socket() error_cli");
    while ((str_len = read(cli_sock, message, sizeof(message))) != 0) {
        write(cli_sock, message, str_len);
//        write(1, message, str_len);
    }
//    shutdown(ser_sock, SHUT_WR);
    close(cli_sock);
    close(ser_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
