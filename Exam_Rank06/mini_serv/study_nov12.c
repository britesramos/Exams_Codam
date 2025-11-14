#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h> //Added this
#include <stdlib.h> //Added this
#include <stdio.h>


int max_fd;
int count_ids;
int client_ids[42000];
char *messages[42000];
fd_set rfds, wfds, afds;
char readbuffer[1001];
char *extracted_msg;
char msg[42];


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

void broadcast(char *msg, int client_fd){
    for(int fd = 0; fd <= max_fd; fd++){
        if(FD_ISSET(fd, &wfds) && client_fd != fd){
            send(fd, msg, strlen(msg), 0);
        }
    }
}

void add_client(int fd){
    client_ids[fd] = count_ids++;
    messages[fd] = NULL;
    sprintf(msg, "server: client %d just arrived\n", client_ids[fd]);
    broadcast(msg, fd);
    if(fd > max_fd)
        max_fd = fd;
}

void remove_client(int fd){
    sprintf(msg, "server: client %d just left\n", client_ids[fd]);
    broadcast(msg, fd);
    close(fd);
    free(messages[fd]); //Missed this
}

int main(int argc, char **argv) {
    if(argc != 2){
        write(2, "Wrong number of arguments\n", strlen("Wrong number of arguments\n"));
        return (1);
    }

	int sockfd;
	struct sockaddr_in servaddr; 

	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1)
		fatal();
    max_fd = sockfd; //
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433);
	servaddr.sin_port = htons(atoi(argv[1])); 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		fatal(); 
	if (listen(sockfd, 10) != 0)
		fatal();
    FD_ZERO(&afds); //This!
    FD_SET(sockfd, &afds);

    while(1){
        rfds = wfds = afds; //THIS!
        if(select(max_fd + 1, &rfds, &wfds, NULL, NULL) < 0)
            fatal();
        for (int fd = 0; fd <= max_fd; fd++){ //Less or equal!
            if(!FD_ISSET(fd, &rfds)) //THIS! And check for read fds not active fds
                continue;
            if(fd == sockfd){
                socklen_t len = sizeof(servaddr);
                int new_client_fd = accept(sockfd, (struct sockaddr *)&servaddr, &len);
                if (new_client_fd < 0)
                   fatal();
                add_client(new_client_fd); //TODO
                FD_SET(new_client_fd, &afds);
                break; //MISSED THIS!
            }
            else{ //NO NEED FOR FD_ISSET CHECKS IN HERE
                int bytes_read = recv(fd, readbuffer, 1000, 0);
                if(bytes_read <= 0){
                    remove_client(fd); //TODO
                    FD_CLR(fd, &afds);
                    break ;
                }
                else{
                    readbuffer[bytes_read] = '\0'; //DONT FORGET TO NULL TERMINATE
                    messages[fd] = str_join(messages[fd], readbuffer);
                    while(extract_message(&messages[fd], &extracted_msg)){
                        sprintf(msg, "client %d: ", client_ids[fd]);
                        broadcast(msg, fd); //One at the time
                        broadcast(extracted_msg, fd); //One at the time
                        free(extracted_msg);
                    }
                }
            }
        }
    }
    return (0);
}