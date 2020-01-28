#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]) {
    int cli_sock;
    sockaddr_in ser_addr;
    int a, q;
    char oper;
    if (argc != 3) {
        printf("Usage :%s <IP> <port>\n", argv[0]);
        exit(1);
    }

    cli_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (cli_sock == -1)
        error("socket() error");
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    if (inet_aton(argv[1], &ser_addr.sin_addr) == 0)
        error("inet_aton error");
    ser_addr.sin_port = htons(atoi(argv[2]));
    if (connect(cli_sock, (sockaddr*)&ser_addr, sizeof(ser_addr)) == -1)
        error("connect error");
    printf("请输入数字个数: ");
    scanf("%d", &q);
    if (write(cli_sock, &q, sizeof(q)) == -1)
        error("write() error");
    while (q--) {
        scanf("%d", &a);
        if (write(cli_sock, &a, sizeof(a)) == -1)
            error("write() error");
    }
    printf("你的运算方法: ");
    scanf(" %c", &oper);
    if (write(cli_sock, &oper, sizeof(oper)) == -1)
        error("write() error");
    if (read(cli_sock, &a, sizeof(a)) == -1)
        error("read error");
    printf("结果是: %d\n", a);
    close(cli_sock);
    return 0;
}
