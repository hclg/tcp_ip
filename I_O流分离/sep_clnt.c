#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/select.h>

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
    FILE *writefp, *readfp;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = inet_addr(argv[1]);
    ser_adr.sin_port = htons(atoi(argv[2]));

    connect(sock, (struct sockaddr*) &ser_adr, sizeof(ser_adr));

    writefp = fdopen(sock, "w");
    readfp = fdopen(sock, "r");
    while (1) {
        if (fgets(buf, mx, readfp) == NULL)
            break;
        fputs(buf, stdout);
        fflush(stdout);
    }
    fputs("THANK YOU! \n", writefp);
    fflush(writefp);
    fclose(writefp);
    fclose(readfp);
    return 0;
}
