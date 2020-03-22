#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#define SIZE_BUF 1024
#define SI 100

char cnt_len[] = "Content-length:2048\r\n";
char server[] = "Server:Linux Web Server \r\n";
void error_msg(char *msg);
void *request_cli(void *arg);
void error_send(FILE *fp);
void send_msg(FILE *fp, char *ct, char *filename);
char *cont_typ(char *filename);


int main(int argv, char **argc)
{
    int ser_sock, cli_sock;
    pthread_t pid;
    struct sockaddr_in ser_adr, cli_adr;
    socklen_t cli_adr_sz;
    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_adr.sin_port = htons(atoi(argc[1]));

    if (bind(ser_sock, (struct sockaddr*)&ser_adr, sizeof(ser_adr)) == -1)
        error_msg("bind error");
    if (listen(ser_sock, 5) == -1)
        error_msg("listen error");

    while (1) {
        cli_sock = accept(ser_sock, (struct sockaddr*)&cli_adr, &cli_adr_sz);

        pthread_create(&pid, NULL, request_cli, (void *)&cli_sock);
        pthread_detach(pid);
    }
    close(ser_sock);
    return 0;
}
void error_msg(char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);

}

void *request_cli(void *arg) {
    int sock = *((int*)arg);
    char filename[SIZE_BUF];
    char ct[SI];
    char request_typ[SI];
    char buf[SIZE_BUF];
    FILE *read_cli, *write_cli;
    read_cli = fdopen(sock, "r");
    write_cli = fdopen(dup(sock), "w");
    fgets(buf, SIZE_BUF, read_cli);
    if (strstr(buf, "HTTP/") == NULL) {
        error_send(write_cli);
        fclose(write_cli);
        fclose(read_cli);
        return NULL;
    }
    strcpy(request_typ, strtok(buf, " /"));
    strcpy(filename, strtok(NULL, " /"));
    strcpy(ct, cont_typ(filename));
    if (strcmp(request_typ, "GET") != 0) {
        error_send(write_cli);
        fclose(write_cli);
        fclose(read_cli);
        return NULL;
    }
    send_msg(write_cli, ct, filename);
    fflush(write_cli);
    fclose(read_cli);
    return NULL;
}

void send_msg(FILE *fp, char *ct, char *filename) {
    char buf[SIZE_BUF];
    char prolocol[] = "HTTP/1.0 200 OK/r/n";
    char cnt_type[SI];
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL) { 
        error_send(fp);
        fclose(fp);
        return;
    }
    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    fputs(prolocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fputs(buf, fp);
    fflush(fp);
    fclose(fp);
    return ;
}


void error_send(FILE *fp) {
    char status[] = "HTTP/1.0 400 Request\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
        "<body><font size=+5><br>发生错误！查看请求文件名和请求方式！"
        "</font></body></html>";
    fputs(status, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fputs(content, fp);
    fflush(fp);
}

char * cont_typ(char *filename) {
    
    char ct[SI];
    strtok(filename, ".");
    strcpy(ct, strtok(filename, "."));
    if (!strcmp(ct, "html") || !strcmp(ct, "htm"))
        strcpy(ct, "text/html");
    else 
        strcpy(ct, "text/plain");
    return ct;
}
