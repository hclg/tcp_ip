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
    int ser_sock, cli_sock;
    FILE *cli_w, *cli_r;
    struct sockaddr_in ser_adr, cli_adr;
    socklen_t cli_sz;
    char buf[mx];
    int str_len;

    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(& ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_adr.sin_port = htons(atoi(argv[1]));

    if(bind(ser_sock, (struct sockaddr*) &ser_adr, sizeof(ser_adr)) == -1)
        error("bind error");
    listen(ser_sock, 5);
    while (1) {
        cli_sz = sizeof(cli_adr);
        cli_sock = accept(ser_sock, (struct sockaddr*) &cli_adr, &cli_sz);
        cli_r = fdopen(cli_sock, "r");
        cli_w = fdopen(cli_sock, "w");
        while(!feof(cli_r)) {
            fgets(buf, mx, cli_r);
            fputs(buf, cli_w);
            fflush(cli_w);
        }
        fclose(cli_r);
        fclose(cli_w);
    }
    close(ser_sock);
    return 0;
}
