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
    int ser_sock, cli_sock;
    FILE *writefp, *readfp;
    struct sockaddr_in ser_adr, cli_adr;
    socklen_t cli_sz;
    char buf[mx];

    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_adr.sin_port = htons(atoi(argv[1]));
    bind(ser_sock, (struct sockaddr*) &ser_adr, sizeof(ser_adr));
    listen(ser_sock, 5);
    cli_sz = sizeof(cli_adr);
    cli_sock = accept(ser_sock, (struct sockaddr*) &cli_adr, &cli_sz);

    readfp = fdopen(cli_sock, "r");
    writefp = fdopen(dup(cli_sock), "w");

    fputs("FROM ser: Hi~ client\n", writefp);
    fputs("yes--------------", writefp);
    fflush(writefp);

    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);

    fgets(buf, sizeof(buf), readfp);
    fputs(buf, stdout);
    fclose(readfp);
    return 0;

}
