#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <signal.h>

#define mx 100

void read_childproc(int sig);
void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
