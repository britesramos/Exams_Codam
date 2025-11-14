#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h> // new

#include <stdlib.h>
#include <stdio.h>

int max_fd = 0;
int client_count = 0;
int client_ids[42000];
char *messages[42000];
fd_set rfds, wfds, afds;
char readbuf[1001];
char writebuf[42];

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

void fatal_error()
{
    write(2, "Fatal error\n", 12);
    exit(1);
}

void notify_all(int author, char *message)
{
    for (int fd = 0; fd <= max_fd; fd++)
    {
        if (FD_ISSET(fd, &wfds) && author != fd)
            send(fd, message, strlen(message), 0);
    }
}

void register_client(int fd)
{
    if (fd > max_fd)
        max_fd = fd;
    client_ids[fd] = client_count++;
    messages[fd] = NULL;
    FD_SET(fd, &afds);
    sprintf(writebuf, "server: client %d just arrived\n", client_ids[fd]);
    notify_all(fd, writebuf);
}

void remove_client(int fd)
{
    sprintf(writebuf, "server: client %d just left\n", client_ids[fd]);
    notify_all(fd, writebuf);
    free(messages[fd]);
    FD_CLR(fd, &afds);
    close(fd);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        write(2, "Wrong number of arguments\n", 26);
        exit(1);
    }    
	// socket create and verification 
    FD_ZERO(&afds);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
        fatal_error();
	}
    max_fd = sockfd;
    FD_SET(sockfd, &afds);

    struct sockaddr_in servaddr; 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(argv[1])); 
  	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
        fatal_error();
	} 
	if (listen(sockfd, 10) != 0) {
        fatal_error();
	}
    while (1)
    {
        rfds = wfds = afds;
        if (select(max_fd + 1, &rfds, &wfds, NULL, NULL) < 0)
            fatal_error();
        for (int fd = 0; fd <= max_fd; fd++)
        {
            if (!FD_ISSET(fd, &rfds))
                continue;
            if (sockfd == fd)
            {
                socklen_t len = sizeof(servaddr);
	            int new_fd = accept(sockfd, (struct sockaddr *)&servaddr, &len);
	            if (new_fd >= 0)
                {
                    register_client(new_fd);
                    break;
                }
            }
            else
            {
                int read_byte = recv(fd, readbuf, 1000, 0);
                if (read_byte <= 0)
                {
                    remove_client(fd);
                    break;
                }
                readbuf[read_byte] = '\0';
                messages[fd] = str_join(messages[fd], readbuf);
                char *message;
                while (extract_message(&(messages[fd]), &message))
                {
                    sprintf(writebuf, "client %d: ", client_ids[fd]);
                    notify_all(fd, writebuf);
                    notify_all(fd, message);
                    free(message);
                }
            }
        }
    }
	return 0;
}
