#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main() {
    int TCP_sock, UDP_sock;
    int sock_type;
    socklen_t optlen;
    int state;
    TCP_sock = socket(PF_INET, SOCK_STREAM, 0);
    UDP_sock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("SOCK_STREAM: %d\n SOCK_DGRAM: %d\n", SOCK_STREAM, SOCK_DGRAM);
    printf("PF_INET: %d\n", PF_INET);
    state = getsockopt(TCP_sock, SOL_SOCKET, SO_TYPE, (void*) &sock_type, &optlen);
    if (state)
        error("getsockopt() error");
    printf("TCP_sock type : %d \n optlen: %d\n", sock_type, optlen);
    state = getsockopt(UDP_sock, SOL_SOCKET, SO_TYPE, (void*) &sock_type, &optlen);
    if (state)
        error("getsockopt() error");
    printf("UDP_sock type : %d \n optlen: %d\n", sock_type, optlen);
    close(TCP_sock);
    close(UDP_sock);
    return 0;
}
