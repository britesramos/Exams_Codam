#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h> //I changed this
#include <stdio.h> //I changed this

int max_fd = 0;
int count_ids = 0;
int client_ids[42000];
char *client_buf[42000];
fd_set rfds, wfds, afds;
char write_buf[42];
char read_buf[1001];

int extract_message(char **buf, char **msg)
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

char *str_join(char *buf, char *add)
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

void fatal_error(){
    write(2, "Fatal error\n", 12);
    exit(1);
}

void broadcast(char *msg, int author){
    for(int fd = 0; fd <= max_fd; fd++){
        if(FD_ISSET(fd, &wfds) && fd != author)
            send(fd, msg, strlen(msg), 0);
    }
}

void add_client(int client_fd){
    if(client_fd > max_fd)
        max_fd = client_fd;
    client_ids[client_fd] = count_ids++;
    client_buf[client_fd] = NULL;
    FD_SET(client_fd, &afds);
    sprintf(write_buf, "server: client %d just arrived\n", client_ids[client_fd]);
    broadcast(write_buf, client_fd);
}

void remove_client(int client_fd){
    sprintf(write_buf, "server: client %d just left\n", client_ids[client_fd]);
    broadcast(write_buf, client_fd);
    close(client_fd);
    free(client_buf[client_fd]);
    FD_CLR(client_fd, &afds);
}

int main(int argc, char **argv) {

    if(argc != 2){
        write(2, "Wrong number of arguments\n", 26);
        exit(1);
    }
	int sockfd;
	struct sockaddr_in servaddr; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		fatal_error();
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(argv[1])); 
  
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
		fatal_error();
	} 
	else
		printf("Socket successfully binded..\n");
	if (listen(sockfd, 10) != 0) {
		fatal_error();
	}

    FD_ZERO(&afds);
    FD_SET(sockfd, &afds);
    max_fd = sockfd; //DONT FORGET THIS!

    while(1){
        rfds = wfds = afds;
        if(select(max_fd + 1, &rfds, &wfds, NULL, NULL) < 0)
            fatal_error();
        for(int fd = 0; fd <= max_fd; fd++){
            if(!FD_ISSET(fd, &rfds))
                continue ;
            if(fd == sockfd){
                socklen_t len = sizeof(servaddr);
                int new_client_fd = accept(sockfd, (struct sockaddr *)&servaddr, &len);
                if(new_client_fd >= 0){
                    add_client(new_client_fd);
                    break ;
                }
            }
            else {
                int bytes_read = recv(fd, read_buf, 1000, 0);
                if(bytes_read <= 0){
                    remove_client(fd);
                    break ;
                }
                else{
                    read_buf[bytes_read] = '\0';
                    client_buf[fd] = str_join(client_buf[fd], read_buf);
                    char *extracted_msg;
                    while(extract_message(&client_buf[fd], &extracted_msg)){
                        sprintf(write_buf, "client %d: ", client_ids[fd]);
                        broadcast(write_buf, fd);
                        broadcast(extracted_msg, fd);
                        free(extracted_msg);
                        extracted_msg = NULL;
                    }
                }
            }
        }
    }

    return (0);
}