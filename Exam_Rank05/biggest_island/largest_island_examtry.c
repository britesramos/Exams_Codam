#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct t_queue
{
    int x;
    int y;
    struct t_queue *next;

}           s_queue;

void ft_putstr(char *str) //This is working :)
{
    int i = 0;
    while (str[i])
    {
        write(1, &str[i], 1);
        i++;
    }
}

void ft_put2darray(char **map)
{
    int i = 0;
    while(map[i])
    {
        ft_putstr(map[i]);
        write(1, "\n", 1);
        i++;
    }
}


char** create_map(char *file_buffer)
{
    int i = 0;
    int j = 0;
    int k = 0;
    char **map = NULL;
    map = malloc(1000 * sizeof(char*));
    if (!map){
        ft_putstr("Fail to malloc map\n");
        return (NULL);
    }
    while(file_buffer[i])
    {
        map[j] = malloc(1000 * sizeof(char));
        if (!map[j]){
            ft_putstr("Fail to malloc map\n");
            return (NULL);
        }
        while (file_buffer[i] != '\n')
        {
            map[j][k] = file_buffer[i];
            // write(1, &file_buffer[i], 1);
            // write(1, &map[j][k], 1);
            // ft_putstr("HOLLA!\n");
            i++;
            k++;
        }
        i++;
        k = 0;
        j++;
    }
    return (map);
}


int flood_fill(char **map)
{
    int biggest_insland = 0;
    int i = 0;
    int j = 0;
    int island_size = 0;
    struct t_queue *queue = NULL;
    while (map[i])
    {
        // ft_putstr("HOLA OUT queeu!\n");
        // ft_put2darray(map);
        while (map[i][j])
        {
            if (map[i][j] == 'X')
            {
                map[i][j] = '0';
                island_size++;
                queue = malloc(sizeof(s_queue));
                if (!queue)
                    return -1;
                queue->next = NULL;
                queue->x = i;
                queue->y = j;
                while(queue)
                {
                    if (queue->x != 0 && map[queue->x - 1][queue->y] == 'X')
                    {
                        // printf("1HERE\n");
                        map[queue->x - 1][queue->y] = '0';
                        island_size++;
                        struct t_queue *node = NULL;
                        node = malloc(sizeof(s_queue));
                        if(!node)
                            return -1;
                        node->x = queue->x - 1;
                        node->y = queue->y;
                        node->next = NULL;
                        struct t_queue *temp = queue;
                        while (temp->next != NULL)
                            temp = temp->next;
                        temp->next = node;
                    }
                    if (map[queue->x + 1][queue->y] && map[queue->x + 1][queue->y] == 'X')
                    {
                        // printf("2HERE\n");
                        map[queue->x + 1][queue->y] = '0';
                        island_size++;
                        struct t_queue *node = NULL;
                        node = malloc(sizeof(s_queue));
                        if(!node)
                            return -1;
                        node->x = queue->x + 1;
                        node->y = queue->y;
                        node->next = NULL;
                        struct t_queue *temp = queue;
                        while (temp->next != NULL)
                            temp = temp->next;
                        temp->next = node;
                    }
                    if (queue->y != 0 && map[queue->x][queue->y - 1] == 'X')
                    {
                        // printf("3HERE\n");
                        map[queue->x][queue->y - 1] = '0';
                        island_size++;
                        struct t_queue *node = NULL;
                        node = malloc(sizeof(s_queue));
                        if(!node)
                            return -1;
                        node->x = queue->x;
                        node->y = queue->y - 1;
                        node->next = NULL;
                        struct t_queue *temp = queue;
                        while (temp->next != NULL)
                            temp = temp->next;
                        temp->next = node;
                    }
                    if (map[queue->x][queue->y + 1] && map[queue->x][queue->y + 1] == 'X')
                    {
                        // printf("4HERE\n");
                        map[queue->x][queue->y + 1] = '0';
                        island_size++;
                        struct t_queue *node = NULL;
                        node = malloc(sizeof(s_queue));
                        if(!node)
                            return -1;
                        node->x = queue->x;
                        node->y = queue->y + 1;
                        node->next = NULL;
                        struct t_queue *temp = queue;
                        while (temp->next != NULL)
                            temp = temp->next;
                        temp->next = node;
                    }
                    struct t_queue *temp = queue;
                    queue = queue->next;
                    free(temp);
                }

            }
            j++;
        }
        // printf("Island_number: %i\n", island_number);
        // printf("Island size: %i\n", island_size);
        if (island_size > 0)
        {
            if (island_size > biggest_insland)
                biggest_insland = island_size;
        }
        island_size = 0;
        j = 0;
        i++;
    }
    return (biggest_insland);
}

char* itoa(int biggest_insland)
{
    int module = 0;
    int num = biggest_insland;
    char *res = malloc(1000 * sizeof(char));
    int i = 0;

    // printf("BIGGESt: %i\n", biggest_insland);
    while (num != 0){
        module = num % 10;
        num = num / 10;
        // printf("num: %i\n", num);
        // printf("module: %i\n", module);
        itoa(num);
        res[i] = module + '0';
        // printf("res[%i]: %c\n", i, module + '0');
        i++;
    }
    // printf("RES: %s\n\n", res);
    return (res);
}

int main (int argc, char *argv[])
{
    char *file_buffer = NULL;
    char **map = NULL;
    int fd = 0;
    int biggest_insland = 0;
    int bytes_read = 0;
    if (argc == 2)
    {
        //1)Open and read file. Extract to a char *.
        fd = open(argv[1], O_RDONLY);
        if (fd == -1)
            return (1);
        file_buffer = malloc(1000 * 1000 * sizeof(char));
        if (!file_buffer)
            return (1);
        bytes_read = read(fd, file_buffer, 10000000); //This BUFFER_SIZE might generate a problem.
        // while (bytes_read != 0)
        // {
        //     bytes_read = read(fd, file_buffer, 10);
        // }
        // ft_putstr(file_buffer);
        //2)Create map based on this.
        map = create_map(file_buffer);
        if (map == NULL)
            return (1);
        ft_put2darray(map);
        //3)Flod fill
        biggest_insland = flood_fill(map);
        // printf("BIGGEST_ISLAND: %i\n", biggest_insland);
        // write(1, "\n", 1);
        // ft_put2darray(map);
        char *str = itoa(biggest_insland);
        int a = 0;
        while(str[a])
            a++;
        while (a >= 0){
            write(1, &str[a], 1);
            a--;
        }
        write(1, "\n", 1);
    }
    else
        ft_putstr("Map Error\n");

    return (0);
}