#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define mx 40
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
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(atoi(argv[1]));
    if (bind(sock, (sockaddr*) &ser_addr, sizeof(ser_addr)) == -1)
        error("bind error");
    while(1) {
        cli_adr_sz = sizeof(cli_addr);
        str_len = recvfrom(sock, ch, mx, 0, (sockaddr*) &cli_addr, &cli_adr_sz);
        sendto(sock, ch, str_len, 0, (sockaddr*) &cli_addr, cli_adr_sz);
    }
    close(sock);
}
