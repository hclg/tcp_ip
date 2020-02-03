#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define mx 30
int main(int argc, char *argv[]) {
    int ser, cli;
    sockaddr_in ser_addr, cli_addr;
    socklen_t cli_si;
    FILE *fp = fopen("text.txt", "rb");
    ser = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(atoi(argv[1]));
    bind(ser, (sockaddr*) &ser_addr, sizeof(ser_addr));
    listen(ser, 5);

    connect(ser, (sockaddr*) &ser_addr, sizeof(ser_addr));
    cli_si = sizeof(cli_addr);
    cli = accept(ser, (sockaddr*) &cli_addr, &cli_si);
    char a[mx];
    int alen = 0, ra;
    while (1) {
        ra = fread((void*)a, 1, mx, fp);
        if (ra < mx) {
            write(cli, a, ra);
            break;
        }
        write(cli, a, mx);
    }
    shutdown(cli, SHUT_WR);
    read(cli, a, mx);
    printf("ser :%s", a);
    fclose(fp);
    close(ser), close(cli);
    return 0;
}
