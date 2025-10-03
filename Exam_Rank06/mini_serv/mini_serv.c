#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>

typedef struct t_data
{
    int server_fd;
    int port;
} s_data;


void ft_putstr(char *str)
{
    int i = 0;
    while(str[i]){
        write(2, &str[i], 1);
        i++;
    }
}

int ft_system_call_error() //For allocating memory for example.
{
    ft_putstr("Fatal error\n");
    return (1);
}

int start_server(struct t_data data)
{
    //Start server socket fd:
    if ((data.server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        return(ft_system_call_error());
    //Bind server socket fd to the port:
    if (bind(data.server_fd, (struct sockaddr*)&address, sizeof(address)) == -1)
        return(ft_system_call_error());
    if (listen(data.server_fd, 128) == -1) //128 is the linux default.
        return(ft_system_call_error());
}

int main(int argc, char **argv)
{
    struct t_data *data = malloc(sizeof(s_data));
    //Listen to client to connect to port 127.0.0.1 and let clients speak to each other.
    //1 Argument: port to bind to.
    //Should only listen to 127.0.0.1
    if (argc != 2){
        ft_putstr("Wrong number of arguments\n");
        return (1);
    }
    data.port = argv[1];
    if (start_server(data) == 1)
        return (1);
    return (0);
}