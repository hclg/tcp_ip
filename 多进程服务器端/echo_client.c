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
int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in ser_adr;
    char message[mx];
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    inet_aton(argv[1], &ser_adr.sin_addr);
    ser_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&ser_adr, sizeof(ser_adr)) == -1)
        error("connect() error");
    while (1) {
        printf("INPUT: (q->end)");
        scanf("%s", message);
        if (strcmp(message, "q") == 0)
            break;
        write(sock, message, sizeof(message));
        read(sock, message, mx);
        printf("server OUT: %s\n", message);
    }
    close(sock);
    return 0;
}
