#include <stdlib.h> //For malloc()
#include <unistd.h> //For write()
#include <netinet/in.h> //For servaddr in function start_server

#include <stdio.h> //temp(for printf)


typedef struct t_data
{
	int server_fd;
	int port;
} s_data;

int ft_system_call_error(struct t_data *data){
	if (data)
		free(data);
	write(2, "Fatal error\n", 12);
	return (1);
}

int start_server(struct t_data *data){
	struct sockaddr_in servaddr; //TODO:What does this do?
	// bzero(&servaddr, sizeof(servaddr)); //This is in main.c //TODO:What does this do?
	servaddr.sin_family = AF_INET; //This is in main.c
	servaddr.sin_addr.s_addr = htonl(2130706433); //This is in main.c
	servaddr.sin_port = htons(data->port); //This is in main.c

	//1)Start server socket fd:
	if ((data->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) //This is in main.c
		return(1);
	//2)BIND server socket fd to the port:
	if (bind(data->server_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) //This is in main.c
		return(1);
	//3)Start LISTEN to the server socket fd:
	if (listen(data->server_fd, 128) == -1) //This is in main.c
		return(1);
	return (0);
}


int main_loop(struct t_data *data){
	while (1){
		//If server connection new client
		//If client connection there ir a new message or the client left
	}
}

int main(int argc, char **argv){

	if (argc != 2){
		write(2, "Wrong number of arguments\n", 26);
		return (1);
	}
	struct t_data *data = malloc(sizeof(s_data));
	if (!data)
		return(ft_system_call_error(NULL));
	data->port = atoi(argv[1]);
	//1)Start server:
	if (start_server(data) == 1)
		return (ft_system_call_error(data));
	else //temp
		printf("Start server sucessfull\n"); //temp
	//2)Main loop:
	if (main_loop(data) == 1)
		return (ft_system_call_error(data));
	return(0);
}