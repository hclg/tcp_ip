#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define mx 30

int main(int argc, char* argv[]) {
    int sock;
    FILE *fp = fopen("close.dat", "wb");
    sockaddr_in ser_addr;
    socklen_t ser_adr_si = sizeof(ser_addr);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    inet_aton(argv[1], &ser_addr.sin_addr);
    ser_addr.sin_port = htons(atoi(argv[2]));
    connect(sock, (sockaddr*)&ser_addr, sizeof(ser_addr));
    int ra;
    char a[mx];
    while ((ra = read(sock, a, mx)) != 0)
        fwrite((void*)a, 1, ra, fp);
    printf("----\n");
    write(sock, "thank you", 10);
    fclose(fp);
    close(sock);
    return 0;
}
