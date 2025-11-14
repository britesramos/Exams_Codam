#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h> //I changed this
#include <stdio.h> //I changed this


int max_fd;
int client_fds[42000];
char *client_buf[42000];
fd_set rfds, wfds, afds;
char msg[42];
char read_buffer[1001];


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

void fatal(){
    write(2, "Fatal error\n", 12);
    exit(1);
}

void broadcast(char *msg, int author){
    for(int fd = 0; fd <= max_fd; fd++){
        if(FD_ISSET(fd, &wfds) && author != fd)
            send(fd, msg, strlen(msg), 0);
    }
}


int main(int argc, char **argv) {

    if(argc != 2){
        write(2, "Wrong number of arguments\n", 26);
        return (1);
    }

	int sockfd;
	struct sockaddr_in servaddr; 
    int new_client_fd;
    int id = 0;
 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1)
		fatal();
    max_fd = sockfd; 
	bzero(&servaddr, sizeof(servaddr)); 

	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(argv[1])); 
  
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		fatal();
	// else
	// 	printf("Socket successfully binded..\n");
	if (listen(sockfd, 10) != 0)
		fatal();
    FD_ZERO(&afds);
    FD_SET(sockfd, &afds);
    while(1){
        rfds = wfds = afds; //DONT FORGET
        if(select(max_fd + 1, &rfds, &wfds, NULL, NULL) < 0)
            fatal();
        for(int fd = 0; fd <= max_fd; fd++){
            if(!FD_ISSET(fd, &rfds))
                continue ;
            if (fd == sockfd){
                socklen_t len = sizeof(servaddr);
                new_client_fd = accept(sockfd, (struct sockaddr *)&servaddr, &len);
                if (new_client_fd >= 0) { //Do not check for negative new_client_fds
                    //ADD CLIENT
                    client_fds[new_client_fd] = id++;
                    client_buf[new_client_fd] = NULL;
                    if(new_client_fd > max_fd)
                        max_fd = new_client_fd;
                    FD_SET(new_client_fd, &afds);
                    sprintf(msg, "server: client %d just arrived\n", client_fds[new_client_fd]);
                    broadcast(msg, new_client_fd);
                    break; //missed this.
                }
            }
            else{
                int bytes_read = recv(fd, read_buffer, 1000, 0);
                if(bytes_read <= 0){
                    //REMOVE CLIENT
                    sprintf(msg, "server: client %d just left\n", client_fds[fd]);
                    broadcast(msg, fd);
                    FD_CLR(fd, &afds);
                    close(fd);
                    free(client_buf[fd]); //Missed this.
                    client_buf[fd] = NULL;
                }
                else{
                    read_buffer[bytes_read] = '\0';
                    client_buf[fd] = str_join(client_buf[fd], read_buffer);
                    char *extracted_msg = NULL;
                    while(extract_message(&client_buf[fd], &extracted_msg)){
                        sprintf(msg, "client %d: ", client_fds[fd]);
                        broadcast(msg, fd);
                        broadcast(extracted_msg, fd);
                        free(extracted_msg);
                        extracted_msg = NULL;
                    }
                }
            }
        }

    }






}