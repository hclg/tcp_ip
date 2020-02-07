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
    FILE *cli_w, *cli_r;
    struct sockaddr_in ser_adr, cli_adr;
    socklen_t cli_sz;
    char buf[mx];
    int str_len;
    struct timeval timeout;
    fd_set reads, cp_reads;

    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(& ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_adr.sin_port = htons(atoi(argv[1]));

    if(bind(ser_sock, (struct sockaddr*) &ser_adr, sizeof(ser_adr)) == -1)
        error("bind error");
    listen(ser_sock, 5);
    FD_ZERO(&reads);
    FD_SET(ser_sock, &reads);
    int fd_max = ser_sock, fd_num;
    while (1) {
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        cp_reads = reads;
        if ((fd_num = select(fd_max+1, &cp_reads, 0, 0, &timeout)) == -1)
            error("select error");
        if (fd_num == 0)
            continue;
        for (int i = 0; i <= fd_max; ++i) {
           if (FD_ISSET(i, &cp_reads)) {
                if (i == ser_sock) {
                    cli_sz = sizeof(cli_adr);
                    cli_sock = accept(ser_sock, (struct sockaddr*) &cli_adr, &cli_sz);
                    if (cli_sock > fd_max)
                        fd_max = cli_sock;
                    FD_SET(cli_sock, &reads);
                    puts("new connected.....");
                }
                else {
                    cli_r = fdopen(i, "r");
                    cli_w = fdopen(i, "w");
                    fgets(buf, mx, cli_r);
                    if (feof(cli_r)) {
                        FD_CLR(i, &reads);
                        fclose(cli_r);
                        fclose(cli_w);
                        puts("disconnect ....");
                        continue;
                    }
                    fputs(buf, cli_w);
                    fflush(cli_w);
                }
           }
        }
    }
    close(ser_sock);
    return 0;
}
