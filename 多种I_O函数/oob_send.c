#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define mx 30
void error(char *message) {
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}
int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in ser_adr;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family =AF_INET;
    inet_aton(argv[1], &ser_adr.sin_addr);
    ser_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr* ) &ser_adr, sizeof(ser_adr)) == -1)
        error("connect() error");

        write(sock, "123", strlen("123"));
        send(sock, "4", 1, MSG_OOB);
        write(sock, "567", strlen("567"));
        send(sock, "890", strlen("890"), MSG_OOB);
        close(sock);
        return 0;
}
