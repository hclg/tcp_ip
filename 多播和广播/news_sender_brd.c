#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define mx 30
void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int send_sock;
    struct sockaddr_in brd_adr;
    FILE *fp;
    char buf[mx];
    int so_brd = 1;
    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(& brd_adr, 0, sizeof(brd_adr));
    brd_adr.sin_family = AF_INET;
    brd_adr.sin_addr.s_addr = inet_addr(argv[1]);
    brd_adr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*) &so_brd, sizeof(so_brd));
    if ((fp = fopen("text.txt", "r")) == NULL)
        error("fopen error");
    while (!feof(fp)) {
        fgets(buf, mx, fp);
        sendto(send_sock, buf, sizeof(buf), 0, (struct sockaddr*) &brd_adr, sizeof(brd_adr));
        sleep(2);
    }
    close(send_sock);
    fclose(fp);
    return 0;

}
