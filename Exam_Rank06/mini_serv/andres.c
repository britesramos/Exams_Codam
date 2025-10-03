#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct s_client {
    int fd;
    int id;
    char *buf;
} t_client;

t_client *clients = NULL;
char msg_to_send[1024];
int max_fd = 0;
int next_id = 0;
fd_set activefds, readfds, writefds;

int ft_strlen(char *s)
{
    int i = 0;
    while(s[i])
    {
        i++;
    }
    return i;
}

//why can't I exit in a non void function?
int fatal()
{
    write(2, "Fatal error\n",  ft_strlen("Fatal error\n"));
    exit(1);
}

void send_all(int exep_fd, char *msg)
{
    for (int i = 0; clients && clients[i].fd != -1; i++)
    {
        if (clients[i].fd != exep_fd && FD_ISSET(clients[i].fd, &writefds))
        {
            send(clients[i].fd, msg, ft_strlen(msg), MSG_DONTWAIT);
        }
    }
}

int get_client_idx(int client_fd)
{
    for (int i = 0; clients && clients[i].fd != -1; i++)
    {
        if (clients[i].fd == client_fd)
            return i;
    }
    return -1;
}

void add_client(int client_fd)
{
    int count = 0;
    while(clients && clients[count].fd != -1)
        count++;
    t_client *new_arr = realloc(clients, sizeof(t_client) * (count + 2));
    if (!new_arr)
        fatal();
    clients = new_arr;
    clients[count].fd = client_fd;
    clients[count].id = next_id++;
    clients[count].buf = NULL;
    clients[count + 1].fd = -1;
    if (client_fd > max_fd)
    {
        max_fd = client_fd;
    }
    memset(msg_to_send, 0, sizeof(msg_to_send));
    sprintf(msg_to_send, "server: client %d just arrived\n", clients[count].id);
    send_all(client_fd, msg_to_send);
}

void remove_client(int client_fd)
{
    int idx = get_client_idx(client_fd);
    if (idx == -1)
        return;
    memset(msg_to_send, 0, sizeof(msg_to_send));
    sprintf(msg_to_send, "server: client %d just left\n", clients[idx].id);
    send_all(clients[idx].fd, msg_to_send);
    close(clients[idx].fd);
    free(clients[idx].buf);
    int count = 0;
    while(clients && clients[count].fd != -1)
        count++;
    for (int i = idx; i < count; i++)
    {
        clients[i] = clients[i + 1];
    }
    t_client *new_arr = realloc(clients, sizeof(t_client) * (count + 1));
    if (!new_arr)
        fatal();
    if(new_arr || count == 0)
        clients = new_arr;
}

int extract_message(char **buf, char **msg) //In main.c
{
    char    *newbuf;
    int i;

    *msg = 0;
    if (*buf == 0)
        return (0);
    i = 0;
    while ((*buf)[i])
    {
        if ((*buf)[i] == '\n')
        {
            newbuf = calloc(1, sizeof(*newbuf) * (ft_strlen(*buf + i + 1) + 1));
            if (!newbuf) //Dif main.c
                fatal(); //Dif main.c
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

char *str_join(char *buf, char *add) //In main.c
{
    char    *newbuf;
    int     len;

    if (buf == 0)
        len = 0;
    else
        len = ft_strlen(buf);
    newbuf = malloc(sizeof(*newbuf) * (len + ft_strlen(add) + 1));
    if (!newbuf) //Dif main.c
        fatal(); //Dif main.c
    newbuf[0] = 0;
    if (buf != 0)
        strcat(newbuf, buf);
    free(buf);
    strcat(newbuf, add);
    return (newbuf);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in servaddr, cli;
    memset(msg_to_send, 0, sizeof(msg_to_send));
    if (argc != 2)
    {
        write(2, "Wrong number of arguments\n", ft_strlen("Wrong number of arguments\n"));
        return (1);
    }
    int port = atoi(argv[1]);
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
        fatal();
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(2130706433);
    servaddr.sin_port = htons(port);
    if (bind(socket_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        fatal();
    if (listen(socket_fd, 10) != 0)
        fatal();
    max_fd = socket_fd;
    FD_ZERO(&activefds);
    FD_SET(socket_fd, &activefds);
    clients = malloc(sizeof(t_client) + 1);
    if (!clients)
        fatal();
    clients[0].fd = -1;
    while(1)
    {
        readfds = activefds;
        writefds = activefds;
        if (select(max_fd + 1, &readfds, &writefds, NULL, NULL) < 0)
            continue;
        if (FD_ISSET(socket_fd, &readfds))
        {
            socklen_t len = sizeof(cli);
            int client_fd = accept(socket_fd, (struct sockaddr *)&cli, &len);
            if (client_fd >= 0)
            {
                FD_SET(client_fd, &activefds);
                add_client(client_fd);
            }
        }
        for (int i = 0; clients && clients[i].fd != -1; i++)
        {
            int client_fd = clients[i].fd;
            if (FD_ISSET(client_fd, &readfds))
            {
                char buffer[1000];
                memset(buffer, 0, sizeof(buffer));
                memset(msg_to_send, 0, sizeof(msg_to_send));
                int n = recv(client_fd, buffer, 999, 0);
                if (n <= 0)
                {
                    FD_CLR(client_fd, &activefds);
                    remove_client(client_fd);
                    i--;
                }
                else
                {
                    char *msg = NULL;
                    clients[i].buf = str_join(clients[i].buf, buffer);
                    while(extract_message(&clients[i].buf, &msg))
                    {
                        memset(msg_to_send, 0, sizeof(msg_to_send));
                        sprintf(msg_to_send, "client %d: ", clients[i].id);
                        send_all(client_fd, msg_to_send);
                        send_all(client_fd, msg);
                        free(msg);
                        msg = NULL;
                    }
                }
            }
        }
    }
    return 0;
}