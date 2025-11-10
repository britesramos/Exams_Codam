#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>

typedef struct t_client
{
    int fd;
    int id;
    char *buf;
} s_client;

typedef struct t_data
{
    int port;
    int server_fd;
    int max_fd;
    struct t_client *clients;
    int next_id;

} s_data;

int extract_message(char **buf, char **msg) //From main.c
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

char *str_join(char *buf, char *add) //From main.c
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
    if (data){
        if (data->clients)
            free(data->clients);
        free(data);
    }
    write(2, "Fatal error\n", 12);
    return (1); //It might need to exit with status 1 instead of this.
}

int start_server(struct t_data *data){
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(data->port); //EXAM ERROR!

    data->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (data->server_fd == -1)
        return(1);
    if ((bind(data->server_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
        return(1);
    if (listen(data->server_fd, 10) != 0)
        return(1);
    data->max_fd = data->server_fd;
    return(0);
}

int main_loop(struct t_data *data){
    fd_set fds_set;
    while(1){
        FD_ZERO(&fds_set);
        FD_SET(data->server_fd, &fds_set);
        for(int i = 0; data->clients && data->clients[i].fd != -1; i++){
            FD_SET(data->clients[i].fd, &fds_set);
        }

        if (select((data->max_fd + 1), &fds_set, NULL, NULL, NULL) < 0)
            continue ;
        write(1, "HERE\n", 5);
        if (FD_ISSET(data->server_fd, &fds_set)){
            int new_client_fd = accept(data->server_fd, NULL, NULL);
            if (new_client_fd != -1)
                write(1, "Add new client\n", 15);
                // add_client(data, new_client_fd);
            //accept new connection
            //add_client
        }
        for(int i = 0; data->clients && data->clients[i].fd != -1; i++){
            if (FD_ISSET(data->clients[i].fd, &fds_set)){
                write(2, "HERE1\n", 6);
                // int bytes_read = recv();
                // if (bytes_read <= 0)
                //     remove_client(data, data->clients[i].fd);
                // else{

                // }

            }
        }
    }


    return (0);
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
    data->clients = malloc(sizeof(s_client));
    if (!data->clients)
        return(ft_system_call_fail(data));
    data->next_id = 0;
    data->clients[0].fd = -1;
    data->clients[0].id = 0;
    data->clients[0].buf = NULL;
    //Start Server:
    if (start_server(data) == 1)
        return(ft_system_call_fail(data));
    else //temp
        write(1, "Success\n", 8); //temp
    //Main Loop:
    if (main_loop(data) == 1)
        return(ft_system_call_fail(data));
    return (0);
}