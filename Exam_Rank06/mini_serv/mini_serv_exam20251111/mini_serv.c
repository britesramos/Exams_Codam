#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

typedef struct t_client{
    int fd;
    int id;
    char *buf;
}s_client;

typedef struct t_data{
    int server_fd;
    int port;
    int max_fd;
    int next_id;
    struct t_client *clients;
}s_data;

int extract_message(char **buf, char **msg) //FROM MAIN.C
{
	char	*newbuf;
	int	i;

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

char *str_join(char *buf, char *add) //FROM MAIN.C
{
	char	*newbuf;
	int		len;

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

int ft_system_call_fail(struct t_data *data){
    if(data){
        if(data->clients)
            free(data->clients);
        free(data);
    }
    write(2, "Fatal error\n", 12);
    return (1);
}

int start_server(struct t_data *data){
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(data->port);

    data->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (data->server_fd == -1)
        return (1);
    if ((bind(data->server_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
        return (1);
    if (listen(data->server_fd, 10) != 0)
        return (1);
    if(data->server_fd > data->max_fd)
        data->max_fd = data->server_fd;
    return (0);
}

void broadcast_msg(struct t_data *data, char *msg, int client_fd){
   for(int i = 0; data->clients && data->clients[i].fd != -1; i++){
        if(data->clients[i].fd != client_fd)
            send(data->clients[i].fd, msg, strlen(msg), 0);
   }
}

int add_client(struct t_data *data, int client_fd){
    int count = 0;
    char msg[100];
    while(data->clients && data->clients[count].fd != -1)
        count++;
    struct t_client *new = realloc(data->clients, sizeof(s_client) * (count + 2));
    if(!new)
        return (1);
    data->clients = new; //MISSED THIS!
    data->clients[count].fd = client_fd;
    data->clients[count].id = data->next_id;
    data->next_id++;
    data->clients[count].buf = NULL;

    data->clients[count + 1].fd = -1;
    data->clients[count + 1].id = -1;
    data->clients[count + 1].buf = NULL;

    if(data->max_fd < client_fd)
        data->max_fd = client_fd;

    sprintf(msg, "server: client %d just arrived\n", data->clients[count].id);
    broadcast_msg(data, msg, client_fd);
    return (0);
}

void remove_client(struct t_data *data, int client_fd){
    int idx = -1;
    int i = 0;
    int j = 0;
    char msg[100];
    while(data->clients && data->clients[i].fd != -1){
        if(data->clients[i].fd == client_fd)
            idx = i;
        i++;
    }
    if(idx == -1)
        return;
    sprintf(msg, "server: client %d just left\n", data->clients[idx].id);
    broadcast_msg(data, msg, client_fd);
    close(data->clients[idx].fd);
    free(data->clients[idx].buf);

    while(data->clients && data->clients[idx + j].fd != -1){
        data->clients[idx + j] = data->clients[idx + j + 1];
        j++;
    }
}

int main_loop(struct t_data *data){
    fd_set readfds;
    int new_client_fd = -1;
    int bytes_read = 0;
    char buffer[200000];
    char msg[200000];
    while(1){
        FD_ZERO(&readfds);
        FD_SET(data->server_fd, &readfds);
        for(int i = 0; data->clients && data->clients[i].fd != -1; i++){
            FD_SET(data->clients[i].fd, &readfds);
        }

        if(select(data->max_fd + 1, &readfds, NULL, NULL, NULL) < 0)
            continue ;

        if(FD_ISSET(data->server_fd, &readfds)){
            new_client_fd = accept(data->server_fd, NULL, NULL);
            if(new_client_fd == -1)
                return (1);
            if(add_client(data, new_client_fd) == 1)
                return (1);
        }

        for(int i = 0; data->clients && data->clients[i].fd != -1; i++){
            if(FD_ISSET(data->clients[i].fd, &readfds)){
                bytes_read = recv(data->clients[i].fd, buffer, 199999, 0);
                if(bytes_read == 0){
                    remove_client(data, data->clients[i].fd);
                    i--;
                }
                else{
                    buffer[bytes_read] = '\0';
                    data->clients[i].buf = str_join(data->clients[i].buf, buffer);
                    char *extracted_msg = NULL;
                    while(extract_message(&data->clients[i].buf, &extracted_msg)){
                        sprintf(msg, "client %d: %s", data->clients[i].id, extracted_msg);
                        broadcast_msg(data, msg, data->clients[i].fd);
                        free(extracted_msg);
                        extracted_msg = NULL;
                    }
                }
            }
        }
    }
}

int main(int argc, char **argv){
    if(argc != 2){
        write(2, "Wrong number of arguments\n", 26);
        return (1);
    }
    struct t_data *data = malloc(sizeof(s_data));
    if(!data)
        return(ft_system_call_fail(NULL));
    data->port = atoi(argv[1]);
    data->server_fd = -1;
    data->max_fd = -1;
    data->next_id = 0;
    struct t_client *client = malloc(sizeof(s_client));
    if(!client)
        return(ft_system_call_fail(data));
    data->clients = client;
    data->clients[0].fd = -1;
    data->clients[0].id = 0;
    data->clients[0].buf = NULL;
    if(start_server(data) == 1)
        return (ft_system_call_fail(data));
    if(main_loop(data) == 1)
        return (1);

    return (0);
}