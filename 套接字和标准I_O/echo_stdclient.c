#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

#define mx 30

void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in ser_adr;
    char buf[mx];
    FILE *sock_w, *sock_r;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    inet_aton(argv[1], &ser_adr.sin_addr);
    ser_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*) &ser_adr, sizeof(ser_adr)) == -1)
        error("connect error");
    else
        puts("connect ....");
    sock_r = fdopen(sock, "r");
    sock_w = fdopen(sock, "w");
    while (1) {
        fputs("INPUT: (q->end) ", stdout);
        fgets(buf, mx, stdin);
        if (strcmp(buf, "q\n") == 0)
            break;
        fputs(buf, sock_w);
        fflush(sock_w);
        fgets(buf, mx, sock_r);
        puts(buf);
    }
    fclose(sock_w);
    fclose(sock_r);
    return 0;
}
