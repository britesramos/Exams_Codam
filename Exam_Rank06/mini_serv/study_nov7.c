
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

typedef struct t_client
{
    int fd;
    int id;
    char *buf;
} s_client;

typedef struct t_data
{
    int server_fd;
    int port;
    struct t_client *clients; //ARRAY OF CLIENT STRUCTS
    int next_id;
    int max_fd;

} s_data;

int extract_message(char **buf, char **msg) //FROM MAIN
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

char *str_join(char *buf, char *add) //FROM MAIN
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

int ft_system_call_error(struct t_data *data){
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
        return(1);
    if ((bind(data->server_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
        return(1);
    if (listen(data->server_fd, 10) != 0)
        return(1);
    data->max_fd = data->server_fd; //MISSED THIS!
    return (0);
}

void broadcast_message(struct t_data *data, char *msg, int new_client_fd){
    for(int i = 0; data->clients && data->clients[i].fd != -1; i++){
        if (data->clients[i].fd != new_client_fd){ //Might be missing something else in here as well.
            send(data->clients[i].fd, msg, strlen(msg), 0); //DONT FORGET THE ZERO AT THE END.
        }
    }
}

int add_client(struct t_data *data, int new_client_fd){
    int count = 0;
    char msg[100]; //For sprintf

    while(data->clients && data->clients[count].fd != -1) //DONT FORGET THE FIRST PART OF THE CONDITION - CHECK IF DATA->CLIENTS EXISTS.
        count++;
    struct t_client *new = realloc(data->clients, sizeof(s_client) * (count + 2));
    if (!new)
        return (1);
    data->clients = new;
    data->clients[count].fd = new_client_fd;
    data->clients[count].id = data->next_id;
    data->next_id++;
    data->clients[count].buf = NULL;
    data->clients[count + 1].fd = -1;
    if (new_client_fd > data->max_fd)
        data->max_fd = new_client_fd;
    sprintf(msg, "server: client %d just arrived\n", data->clients[count].id);
    broadcast_message(data, msg, new_client_fd);
    return (0);
}

void remove_client(struct t_data *data, int client_fd){
    char msg[100];
    int idx = -1;

    for(int i = 0; data->clients && data->clients[i].fd != -1; i++){
        if (data->clients[i].fd == client_fd){
            idx = i;
            break ;
        }
    }
    if (idx == -1)
        return ;
    sprintf(msg, "server: client %d just left\n", data->clients[idx].id);
    broadcast_message(data, msg, client_fd);
    close(data->clients[idx].fd);
    free(data->clients[idx].buf);

    int count = 0;
    while(data->clients[count].fd != -1)
        count++;
    for(int i = idx; i < count; i++){
        data->clients[i] = data->clients[i + 1];
    }
}

int main_loop(struct t_data *data){
    fd_set set_fds;
    char buffer[4100];
    int bytes_read = 0;
    char *extracted_msg = NULL;
    char msg[4100];

    while(1){
        FD_ZERO(&set_fds);
        FD_SET(data->server_fd, &set_fds);

        for(int i = 0; data->clients && data->clients[i].fd != -1; i++){
            FD_SET(data->clients[i].fd, &set_fds);
        }

        if (select(data->max_fd + 1, &set_fds, NULL, NULL, NULL) < 0)
            continue ;

        if (FD_ISSET(data->server_fd, &set_fds)){
            int new_client_fd = accept(data->server_fd, NULL, NULL);
            if(new_client_fd >= 0){
                if(add_client(data, new_client_fd) == 1)
                    return(1);
            }
        }
        for(int i = 0; data->clients && data->clients[i].fd != -1; i++){
            if (FD_ISSET(data->clients[i].fd, &set_fds)){
                bytes_read = recv(data->clients[i].fd, buffer, 4099, 0);
                if (bytes_read <= 0){
                    remove_client(data, data->clients[i].fd); //TODO
                    i--; //DONT FORGET THIS.
                } else {
                    buffer[bytes_read] = '\0';
                    data->clients[i].buf = str_join(data->clients[i].buf, buffer);
                    if (!data->clients[i].buf)
                        return(1);
                    while(extract_message(&data->clients[i].buf, &extracted_msg) == 1){
                        sprintf(msg, "client %d: %s", data->clients[i].id, extracted_msg);
                        broadcast_message(data, msg, data->clients[i].fd);
                        free(extracted_msg);
                        extracted_msg = NULL;
                    }
                }
            }
        }
    }
}

int main (int argc, char **argv){
    if (argc != 2){
        write(2, "Wrong number of arguments\n", 26);
        return (1);
    }
    struct t_data *data = malloc(sizeof(s_data));
    if (!data)
        return(ft_system_call_error(NULL));
    data->port = atoi(argv[1]); //MISSED ATOI()
    data->next_id = 0; //DONT FORGET TO INIT THE DATA STRUCT
    data->clients = malloc(sizeof(s_client));
    if (!data->clients)
        return(ft_system_call_error(data));
    data->clients[0].fd = -1;
    if (start_server(data) == 1)
        return(ft_system_call_error(data));
    if (main_loop(data) == 1)
        return(ft_system_call_error(data));
    free(data);
    return (0);
}