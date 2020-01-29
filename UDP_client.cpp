#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define mx 30
void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char * argv[]) {
    int sock;
    sockaddr_in ser_addr, cli_addr;
    socklen_t cli_adr_sz;
    int str_len;
    char ch[mx];
    sock  = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error("socket() error");
    memset(&ser_addr, 0, sizeof (ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr(argv[1]);
    ser_addr.sin_port = htons(atoi(argv[2]));
//    if (bind(sock, (sockaddr*) &ser_addr, sizeof(ser_addr)) == 0)
//        error("bind() error");
    connect(sock, (sockaddr*) &ser_addr, sizeof(ser_addr));
    while(1) {
        fputs("INPUT:", stdout);
        scanf("%s", ch);
        if (strcmp(ch, "q") == 0) break;
//        sendto(sock, ch, sizeof(ch), 0, (sockaddr*) &ser_addr, sizeof(ser_addr));
        write(sock, ch, sizeof(ch));
        printf("OUT:");
        str_len = read(sock, ch, mx);
//        cli_adr_sz = sizeof(ser_addr);
//        str_len = recvfrom(sock, ch, mx, 0, (sockaddr*) &ser_addr, &cli_adr_sz);
        ch[str_len] = 0;
        puts(ch);
    }
    close(sock);
}

