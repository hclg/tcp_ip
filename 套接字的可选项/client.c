#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define mx 1024
void error_handling(char *message);


int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in ser_adr;
    int str_len;
    char message[mx];
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = inet_addr(argv[1]);
    ser_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&ser_adr, sizeof(ser_adr)) == -1)
        error_handling("connect() error");

    while (1) {
        printf("INPUT (q->end()):");
        scanf("%s", message);
        if (strcmp(message, "q") == 0) break;
        write(sock, message, sizeof(message));
        str_len = read(sock, message, sizeof(message));
        printf("server : %s\n", message);
    }
//    shutdown(sock, SHUT_WR);
    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
