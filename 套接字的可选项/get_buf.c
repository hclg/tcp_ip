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
    int sock;
    int snd_buf, rcv_buf, state;
    socklen_t len;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if (state)
        error("getsockopt() error");
    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if (state)
        error("getsockopt() error");
    printf("INPUT buffer: %d\n", rcv_buf);
    printf("OUTput buffer: %d\n", snd_buf);
    close(sock);
    return 0;
}
