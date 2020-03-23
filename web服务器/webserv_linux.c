#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#define SIZE_BUF 2048
#define SI 200

char cnt_len[] = "Content-length:4096\r\n";
char server[] = "Server:Linux Web Server \r\n";
void error_msg(char *msg);
void *request_cli(void *arg);
void error_send(FILE *fp);
void send_msg(FILE *fp, char *ct, char *filename);
char *cont_typ(char *filename);


int main(int argv, char *argc[])
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
    if (listen(ser_sock, 20) == -1)
        error_msg("listen error");

    while (1) {
        cli_adr_sz = sizeof(cli_adr);
        cli_sock = accept(ser_sock, (struct sockaddr*)&cli_adr, &cli_adr_sz);
        puts("sss");

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
    puts("request");
    char filename[SIZE_BUF];
    char ct[SI];
    char request_typ[SI];
    char buf[SIZE_BUF];
    FILE *read_cli, *write_cli;
    read_cli = fdopen(sock, "r");
    write_cli = fdopen(dup(sock), "w");
    fgets(buf, SIZE_BUF, read_cli);
    puts(buf);
    if (strstr(buf, "HTTP/") == NULL) {
        puts("HTTP");
        error_send(write_cli);
        fclose(write_cli);
        fclose(read_cli);
        return NULL;
    }
    
    strcpy(request_typ, strtok(buf, " /"));
    strcpy(filename, strtok(NULL, " /"));
    puts("help");
    strcpy(ct, cont_typ(filename));
    puts(ct);
    if (strcmp(request_typ, "GET") != 0) {
        puts("GET");
        error_send(write_cli);
        fclose(write_cli);
        fclose(read_cli);
        return NULL;
    }
    fclose(read_cli);
    send_msg(write_cli, ct, filename);
//    fflush(write_cli);
    return NULL;
}

void send_msg(FILE *fp, char *ct, char *filename) {
    char buf[SIZE_BUF];
    char prolocol[] = "HTTP/1.0 200 OK\r\n";
    char cnt_type[SI];
    FILE *file;
    puts(filename);
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
//    fputs(buf, fp);
    puts(cnt_type);
    while (fgets(buf, SIZE_BUF, file) != NULL) {
        fputs(buf, fp);
        puts(buf);
        fflush(fp);
    }
    fflush(fp);
    fclose(fp);
    return ;
}


void error_send(FILE *fp) {
    puts("error _send");
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
    char file_name[SI];
    strcpy(file_name, filename);
 //   puts(file_name);
    strtok(file_name, ".");
    strcpy(ct, strtok(NULL, "."));
    if (!strcmp(ct, "html") || !strcmp(ct, "htm"))
        return "text/html";
    else 
        return "text/plain";
}
