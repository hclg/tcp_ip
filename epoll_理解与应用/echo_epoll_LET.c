#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define SIZE 50

void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void setmode(int fd) {
    int flage = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flage|O_NONBLOCK);
}

int main(int argc, char *argv[]) {
    int ser_sock, cli_sock;
    struct sockaddr_in se_adr, cl_adr;
    socklen_t adr_sz;
    int str_len, i;
    char buf[4];

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&se_adr, 0, sizeof(se_adr));
    se_adr.sin_family = AF_INET;
    se_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    se_adr.sin_port = htons(atoi(argv[1]));
    if (bind(ser_sock, (struct sockaddr*)&se_adr, sizeof(se_adr)) == -1)
        error("bind() error");
    listen(ser_sock, 5);
    epfd = epoll_create(SIZE);
    ep_events = malloc(sizeof(struct epoll_event)*SIZE);

    setmode(ser_sock);
    event.events = EPOLLIN;
    event.data.fd = ser_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, ser_sock, &event);
    while (1) {
        event_cnt = epoll_wait(epfd, ep_events, SIZE, -1);
        if (event_cnt == -1)
            error("wait error");
        puts("wait......");
        for (int i = 0; i < event_cnt; ++i) {
            if (ep_events[i].data.fd == ser_sock) {
                adr_sz = sizeof(cl_adr);
                cli_sock = accept(ser_sock, (struct sockaddr*) &cl_adr, &adr_sz);
                event.events = EPOLLIN|EPOLLET;
                event.data.fd = cli_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cli_sock, &event);
                printf("new connected ......\n");
            }
            else {
                while (1) {
                    str_len = read(ep_events[i].data.fd, buf, SIZE);
                    if (str_len == 0) {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        printf("disconnect %d\n", ep_events[i].data.fd);
                        break;
                    }
                    else if (str_len < 0){
                        if (errno == EAGAIN)
                            break;
                    }
                    else {
                        write(ep_events[i].data.fd, buf, str_len);
                    }
                }
            }
        }
    }
    close(ser_sock);
    close(epfd);
    return 0;
}
