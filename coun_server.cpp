#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define mx 1024
int a[mx];
int si, len, q;

void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void pus(int cli_sock, char oper, int a[]) {
    int sum = a[0];
    if (oper == '+') {
        for (int i = 1; i < si; ++i) {
            printf("%d\n", a[i]);
            sum += a[i];
        }
    }
    if (oper == '-')
        for (int i = 1; i < si; ++i)
            sum -= a[i];
    if (oper == '*')
        for (int i = 1; i < si; ++i)
            sum *= a[i];
    if (si == 0) sum = 0;
    write(cli_sock, (char*)&sum, sizeof(sum));
    return;
}
char ch[mx];
int main(int argc, char *argv[]) {
    int ser_sock, cli_sock;
    sockaddr_in ser_addr, cli_addr;
    socklen_t cli_adr_si;
    char oper;
    if (argc != 2) {
        printf("Usage :%s <port>\n", argv[0]);
        exit(1);
    }

    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (ser_sock == -1)
        error("socket() error");
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(atoi(argv[1]));
    if (bind(ser_sock, (sockaddr*)&ser_addr, sizeof(ser_addr)) == -1)
        error("bind() error");

    if (listen(ser_sock, 5) == -1)
        error("listen() error");
    cli_adr_si = sizeof(cli_addr);
    cli_sock = accept(ser_sock, (sockaddr*)&cli_addr, &cli_adr_si);
    if (cli_sock == -1)
        error("accept() error");
    if (read(cli_sock, &si, 1) == -1)
        error("read() error");
    int revlen = 0;
    printf("%d\n", si);
    while (si*4+1 > revlen) {
        int ss;
        ss = read(cli_sock, ch+revlen, mx);
        revlen += ss;
    }

    pus(cli_sock, ch[revlen-1], (int *)ch);
    close(cli_sock);
    close(ser_sock);
    return 0;
}
