#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define mx 30
void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in adr;
    FILE *fp;
    char buf[mx];
    int str_len;
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error("socket() error");
    memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(atoi(argv[1]));
    if (bind(sock, (struct sockaddr*)&adr, sizeof(adr)) == -1)
        error("bind error");
    while (1) {
        str_len = recvfrom(sock, buf, mx, 0, NULL, 0);
        if (str_len < 0)
            break;
        buf[str_len] = 0;
        fputs(buf, stdout);
    }
    close(sock);
    return 0;
}
