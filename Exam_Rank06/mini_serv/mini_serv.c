#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>

typedef struct t_data
{
    int server_fd;
    int port;
} s_data;

typedef struct t_client
{
	int fd;
	int id;
	char *buf;
} s_client;


void ft_putstr(char *str)
{
    int i = 0;
    while(str[i]){
        write(2, &str[i], 1);
        i++;
    }
}

int ft_system_call_error(struct t_data *data) //For allocating memory for example.
{
	if (data)
		free(data);
    ft_putstr("Fatal error\n");
    return (1);
}

int start_server(struct t_data *data)
{
	struct sockaddr_in servaddr; //Needed for biding.
	bzero(&servaddr, sizeof(servaddr)); //Not sure if this necessary.
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433);
	servaddr.sin_port = htons(data->port);

    //Start server socket fd:
    if ((data->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        return(ft_system_call_error(data));
    //Bind server socket fd to the port:
    if (bind(data->server_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
        return(ft_system_call_error(data));
    if (listen(data->server_fd, 128) == -1){//128 is the linux default.
        return(ft_system_call_error(data));
	} 
	return (0);
}

int main_loop(struct t_data *data)
{
	struct sockaddr_in cli;
	fd_set current_sockets, read_sockets, write_sockets;
	FD_ZERO(&current_sockets); //Clear set like clearing the poll in Epoll.
	FD_SET(data->server_fd, &current_sockets); //Adding the server socket to the Set.
	int max_fd = 0; //Andres set this equal to server socket. That seams odd.

	//Alocating for client structure with fd flag -1, like Andres?

	
	while(1)
	{
		read_sockets = current_sockets; //Because select is destructive.
		write_sockets = current_sockets; //Because select is destructive.
		
		if (select(max_fd + 1, &read_sockets, &write_sockets, NULL, NULL) < 0)
			continue; //We cant break every time that there is an error with select(), that would crash our server.
		
		if (FD_ISSET(data->server_fd, &read_sockets)) //The server socket is ready.
		{
			socklen_t len = sizeof(cli); //???how does this work?
			int client_fd = accept(data->server_fd, (struct sockaddr *)&cli, &len);
			if (client_fd >= 0)
			{
				FD_SET(client_fd, &current_sockets); //Adding client fd to set.
				// add_client(client_fd); //TODO
			}
		}

		
	}
	return (0);
}

int main(int argc, char **argv)
{
	if (argc != 2){
		ft_putstr("Wrong number of arguments\n");
		return (1);
	}

    struct t_data *data = malloc(sizeof(s_data));
	if (!data)
		return(ft_system_call_error(NULL));
	
    //Listen to client to connect to port 127.0.0.1 and let clients speak to each other.
    //1 Argument: port to bind to.
    //Should only listen to 127.0.0.1
    data->port = atoi(argv[1]);
    if (start_server(data) == 1)
        return (1);
	else //temp
		ft_putstr("Start Server Sucessfull\n"); //temp
	if (main_loop(data) == 1)
		return (1);
	free(data);
    return (0);
}