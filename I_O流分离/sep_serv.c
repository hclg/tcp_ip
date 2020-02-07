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
    FILE *writefd, *readfp;
    struct sockaddr_in ser_adr, cli_adr;
    char buf[mx];
    socklen_t cli_sz;

    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_adr.sin_port = htons(atoi(argv[1]));
    bind(ser_sock, (struct sockaddr*) &ser_adr, sizeof(ser_adr));
    listen(ser_sock, 5);
    cli_sock = accept(ser_sock, (struct sockaddr*) &cli_adr, &cli_sz);

    writefd = fdopen(cli_sock, "w");
    writefd = fdopen(cli_sock, "w");
    readfp = fdopen(cli_sock, "r");
    fputs("Hi ~client \n", writefd);
    fputs("yes \n", writefd);
    fputs("你好\n", writefd);
    fclose(writefd);
    fflush(writefd);

    fgets(buf, mx, readfp);
    fputs(buf,stdout);
//    fflush(stdout);
    fclose(readfp);
    close(ser_sock);
    return 0;
}
