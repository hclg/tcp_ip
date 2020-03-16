#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100
#define NA_SI 20

void error(char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

char name[NA_SI];
void *recv_msg(void *arg);
void *send_msg(void *arg);

int main(int argv, char *argc[])
{
    int sock;
    struct sockaddr_in ser_adr;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = inet_addr(argc[1]);
    ser_adr.sin_port = htons(atoi(argc[2]));
    if (connect(sock, (struct sockaddr*)&ser_adr, sizeof(ser_adr)) == -1) 
        error("connect error");
    pthread_t send_id, recv_id;
    sprintf(name, "[%s]", argc[3]);
    pthread_create(&send_id, NULL, send_msg, (void*)&sock);
    pthread_create(&recv_id, NULL, recv_msg, (void*)&sock);

    pthread_join(send_id, NULL);
    pthread_join(recv_id, NULL);
    close(sock);
    return 0;

}

void *send_msg(void *arg) {
    int sock = *((int*) &arg);
    char msg[SIZE];
    while (1) {
       fgets(msg, SIZE, stdin);
        if (strcmp(msg, "Q") == 0|| strcmp(msg, "q") == 0)
            break;
        sprintf(msg, "%s %s", name, msg);
        write(sock, msg, sizeof(msg));
    }
    return NULL;
}
void *recv_msg(void *arg) {
    int sock = *((int *)&arg);
    char msg[SIZE];
    int str_len = 0;
    while ((str_len = read(sock, msg, NA_SI+SIZE-1)) != 0) {
        msg[str_len] = 0;
        fputs(msg, stdout);
    }
    return NULL;
}
