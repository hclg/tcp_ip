#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]) {
    int sock, state;
    int snd_buf = 1024*3;
    int rcv_buf = 1024*3;
    socklen_t len;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    len = sizeof(rcv_buf);
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&rcv_buf, sizeof(rcv_buf));
    if (state)
        error("setsockopt() 1 error");
    len = sizeof(snd_buf);
    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&snd_buf, sizeof(snd_buf));
    if (state)
        error("setsockopt() 2 error");
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&rcv_buf, &len);
    if (state)
        error("getsockopt() 1 error");
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&snd_buf, &len);
    if (state)
        error("getsockopt() 2 error");
    printf("INPUT :%d\n", rcv_buf);
    printf("OUTPUT :%d\n", snd_buf);
    close(sock);
    return 0;
}
