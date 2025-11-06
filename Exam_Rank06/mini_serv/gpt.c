#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_client {
    int id;
    char *buf;
} t_client;

t_client clients[FD_SETSIZE];
int next_id = 0;
int max_fd = 0;
fd_set active_fds, read_fds, write_fds;

int extract_message(char **buf, char **msg)
{
    char *newbuf;
    int i;

    *msg = 0;
    if (*buf == 0)
        return (0);
    i = 0;
    while ((*buf)[i])
    {
        if ((*buf)[i] == '\n')
        {
            newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
            if (newbuf == 0)
                return (-1);
            strcpy(newbuf, *buf + i + 1);
            *msg = *buf;
            (*msg)[i + 1] = 0;
            *buf = newbuf;
            return (1);
        }
        i++;
    }
    return (0);
}

char *str_join(char *buf, char *add)
{
    char *newbuf;
    int len;

    if (buf == 0)
        len = 0;
    else
        len = strlen(buf);
    newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
    if (newbuf == 0)
        return (0);
    newbuf[0] = 0;
    if (buf != 0)
        strcat(newbuf, buf);
    free(buf);
    strcat(newbuf, add);
    return (newbuf);
}

void fatal_error(void)
{
    write(2, "Fatal error\n", 12);
    exit(1);
}

void send_to_all(int except_fd, char *msg)
{
    for (int fd = 0; fd <= max_fd; fd++)
    {
        if (FD_ISSET(fd, &write_fds) && fd != except_fd)
            send(fd, msg, strlen(msg), 0);
    }
}

void accept_client(int sockfd)
{
    struct sockaddr_in cli;
    socklen_t len = sizeof(cli);
    int connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
    
    if (connfd < 0)
        return;
    
    // Add to active fds
    FD_SET(connfd, &active_fds);
    if (connfd > max_fd)
        max_fd = connfd;
    
    // Initialize client
    clients[connfd].id = next_id++;
    clients[connfd].buf = NULL;
    
    // Notify all clients
    char msg[100];
    sprintf(msg, "server: client %d just arrived\n", clients[connfd].id);
    send_to_all(connfd, msg);
}

void remove_client(int fd)
{
    char msg[100];
    sprintf(msg, "server: client %d just left\n", clients[fd].id);
    send_to_all(fd, msg);
    
    FD_CLR(fd, &active_fds);
    close(fd);
    free(clients[fd].buf);
    clients[fd].buf = NULL;
}

void handle_client(int fd)
{
    char buffer[4097];
    int ret = recv(fd, buffer, 4096, 0);
    
    if (ret <= 0)
    {
        remove_client(fd);
        return;
    }
    
    buffer[ret] = '\0';
    
    // Append to buffer
    clients[fd].buf = str_join(clients[fd].buf, buffer);
    if (!clients[fd].buf)
        fatal_error();
    
    // Extract and send complete messages
    char *msg;
    while (extract_message(&clients[fd].buf, &msg))
    {
        if (msg == 0)
            fatal_error();
        
        char full_msg[4200];
        sprintf(full_msg, "client %d: %s", clients[fd].id, msg);
        send_to_all(fd, full_msg);
        free(msg);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        write(2, "Wrong number of arguments\n", 26);
        exit(1);
    }
    
    int port = atoi(argv[1]);
    
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        fatal_error();
    
    // Bind and listen
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1
    servaddr.sin_port = htons(port);
    
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        fatal_error();
    
    if (listen(sockfd, 128) < 0)
        fatal_error();
    
    // Initialize
    FD_ZERO(&active_fds);
    FD_SET(sockfd, &active_fds);
    max_fd = sockfd;
    
    for (int i = 0; i < FD_SETSIZE; i++)
        clients[i].buf = NULL;
    
    // Main loop
    while (1)
    {
        read_fds = write_fds = active_fds;
        
        if (select(max_fd + 1, &read_fds, &write_fds, NULL, NULL) < 0)
            continue;
        
        for (int fd = 0; fd <= max_fd; fd++)
        {
            if (!FD_ISSET(fd, &read_fds))
                continue;
            
            if (fd == sockfd)
                accept_client(sockfd);
            else
                handle_client(fd);
        }
    }
    
    return 0;
}