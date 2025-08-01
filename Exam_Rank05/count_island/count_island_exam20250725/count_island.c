#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

void ft_putstr(char *str)
{
    int i = 0;
    while(str[i])
    {
        write(1, &str[i], 1);
        i++;
    }
}

void ft_putcharpointer(char **pointer)
{
    int i = 0;
    while (pointer[i])
    {

        // printf("%i\n", i);
        ft_putstr(pointer[i]);
        write(1, "\n", 1);
        i++;
    }
    // ft_putstr(pointer[i]);
    // write(1, "\n", 1);
}

void ft_putchar(char c)
{
    write(1, &c, 1);
}

void ft_itoa(int n)
{
    if(n >= 10)
        ft_itoa(n / 10);
    ft_putchar ((n % 10) + '0');
}

int ft_strlen(char *str)
{
    int i = 0;
    while(str[i])
        i++;
    return (i);
}

char* ft_strjoin(char * buffer, char *temp)
{
    int i = 0;
    int total_len = ft_strlen(buffer) + ft_strlen(temp);
    char *placeholder = NULL;
    placeholder = malloc(total_len * sizeof(char));
    if (!placeholder)
        return (NULL);
    while (i < ft_strlen(buffer))
    {
        placeholder[i] = buffer[i];
        i++;
    }
    for(int j = 0; j < ft_strlen(temp); ++j){
        placeholder[i + j] = temp[j];
    }
    free(buffer);
    buffer = malloc(total_len * sizeof(char));
    if (!buffer)
    {
        free(placeholder);
        return (NULL);
    }
    for (int k = 0; k < total_len; ++k){
        buffer[k] = placeholder[k];
    }
    buffer = placeholder;
    free(placeholder);
    return (buffer);
}

char* read_file(int fd)
{
    char *buffer = NULL;
    char *temp = NULL;
    int bytes_read = 0;
    buffer = malloc(1024 * sizeof(char));
    if (!buffer)
        return (buffer);
    bytes_read = read(fd, buffer, 1024);
    // ft_putstr(buffer); //temp
    // printf("1BYTES_READ: %i\n", bytes_read); //temp
    if (bytes_read < 0)
        return (NULL);
    temp = malloc(1024 * sizeof(char));
    if (!temp)
    {
        free(buffer);
        return (NULL);
    }
    while (bytes_read > 0) //THIS MIGHT NEED TWEACK FOR A BIGGER MAP.
    {
        bytes_read = read(fd, temp, 1024);
        if (bytes_read == 0)
            break ;
        // printf("2BYTES_READ: %i\n", bytes_read); //temp
        // ft_putstr(temp);
        // ft_putstr(buffer);
        ft_strjoin(buffer, temp);
        // ft_putstr(buffer);
    }
    // ft_putstr(buffer);
    free(temp);
    return (buffer);
}

int ft_len_first_row(char *buffer)
{
    int len = 0;
    while (buffer[len] != '\n' && len < 1024){
        // write(1, "2HELLLOOOO!\n", 12);
        len++;

    }
    return (len);
}

int ft_count_rows(char *buffer, int first_row_len)
{
    int count_rows = 0;
    int i = 0;
    int row_len = 0;
    while (buffer[i])
    {
        if (buffer[i] != 'X' && buffer[i] != '.' && buffer[i] != '\n')
            return (-1);
        if (buffer[i] == '\n'){
            // printf("ROW_LEN: %i\n", row_len); //temp
            if (row_len == 0 || row_len != first_row_len)
                return (-1);
            count_rows++;
            row_len = 0;
        }
        else
            row_len++;
        i++;
    }
    // if (row_len == 0 || row_len != first_row_len){
    //     // printf("EXTRA CHECK!\n\n"); //temp
    //     return (-1);
    // }
    count_rows++;
    // printf("SUCCESS!\n"); //temp
    return (count_rows);
}

char** parse_map(char *buffer)
{
    char **map = NULL;
    int i = 0;
    int j = 0;
    int k = 0;
    // write(1, "1HELLLOOOO!\n", 12);
    int len_first_row = ft_len_first_row(buffer);
    // printf("LEN FIRST ROW: %i\n\n", len_first_row); //temp
    int count_rows = ft_count_rows(buffer, len_first_row);
    // printf("NUM ROW: %i", count_rows); //temp
    if (count_rows == -1)
        return (NULL);
        // ft_putstr("Misconfiguration.\n");
    map = malloc((count_rows + 1) * sizeof(char *));
    if (!map)
        return (NULL);
    while (buffer[i])
    {
        map[j] = malloc(len_first_row + 1 * sizeof(char));
        //TODO: CLEAN UP.
        // printf("J: %i\n", j); //temp
        while (buffer[i] && buffer[i] != '\n')
        {
            map[j][k] = buffer[i];
            i++;
            k++;
        }
        map[j][k] = '\0';
        // ft_putstr(map[j]);
        if (!buffer[i])
            break ;
        k = 0;
        j++;
        i++;
    }
    // map[j] = NULL;
    return (map);
}

typedef struct t_data
{
    int x;
    int y;
    struct t_data *next;
}      s_data;

struct t_data* create_node(int y, int x)
{
    struct t_data *node;
    node = malloc(sizeof(s_data));
    node->x = x;
    node->y = y;
    node->next = NULL;
    return (node);
}

void add_node(struct t_data *node, int y, int x)
{
    struct t_data* new_node = create_node(y, x);
    struct t_data* current = node;
    while(current->next != NULL)
    {
        current = current->next;
    }
    current->next = new_node;
}

void remove_first_node(struct t_data *node)
{
    struct t_data *temp = node;
    node = node->next;
    free(temp);
}

int ft_max_rows(char **map)
{
    int res = 0;
    while(map[res])
        res++;
    return (res);
}

int ft_line_len(char **map)
{
    int len = 0;
    while(map[0][len])
        len++;
    // printf("LEN: %i\n", len);
    return (len);
}

char** flood_fill(char **map)
{
    int i = 0;
    int j = 0;
    int island_num = 0;
    struct t_data *node = NULL;
    int max_rows = ft_max_rows(map);
    int line_len = ft_line_len(map);

    // printf("MAX ROWS: %i\n\n", max_rows); //temp
    while(map[i])
    {
        // printf("MAP[%i]: %s\n\n", i, map[i]);
        while (map[i][j])
        {
            // printf("MAP[%i][%i]: %c\n", i, j, map[i][j]);
            if (map[i][j] == 'X')
            {
                // printf("HeLLO!\n");
                // printf("I: %i, J: %i\n\n", i, j);
                map[i][j] = island_num + '0';
                node = create_node(i, j);
                while(node)
                {
                    // printf("ISLAND: %i, ISLAND CHAR: %c\n\n", island_num, island_num + '0');
                    // printf("NODE X: %i, NODE Y: %i\n\n", node->x, node->y); //temp
                    if (node->y != 0 && map[node->y - 1][node->x] == 'X'){
                        // printf("1) %c\n", map[node->y - 1][node->x]);
                        map[node->y - 1][node->x] = island_num + '0';
                        add_node(node, node->y - 1, node->x);
                    }
                    if (node->y + 1 < max_rows && map[node->y + 1][node->x] == 'X'){
                        // printf("2) %c\n", map[node->y + 1][node->x]);
                        map[node->y + 1][node->x] = island_num + '0';
                        add_node(node, node->y + 1, node->x);
                    }
                    if (node->x != 0 && map[node->y][node->x - 1] == 'X'){
                        // printf("3) %c\n", map[node->y][node->x - 1]);
                        map[node->y][node->x - 1] = island_num + '0';
                        add_node(node, node->y, node->x - 1);
                    }
                    if (node->x + 1 <= line_len && map[node->y][node->x + 1] == 'X'){
                        // printf("4) %c\n", map[node->y][node->x + 1]);
                        map[node->y][node->x + 1] = island_num + '0';
                        add_node(node, node->y, node->x + 1);
                        // printf("HERE!\n");
                    }
                    node = node->next;
                }
                island_num++;
                // printf("COUNT ISLAND++\n\n");
            }
            j++;
        }
        j = 0;
        i++;
    }
    return (map);
}

void ft_free(char **map)
{
    int i = 0;
    while(map[i])
    {
        free(map[i]);
        i++;
    }
    free(map);
}

int main (int argc, char **argv)
{
    int fd = 0;
    char *buffer = NULL;
    char **map = NULL;
    if (argc == 2)
    {
        //open file.
        fd = open(argv[1], O_RDONLY);
        if (fd == -1)
        {
            ft_putchar('\n');
            return (1);
        }
        //read file
        buffer = read_file(fd);
        if (buffer == NULL)
        {
            close(fd);
            ft_putchar('\n');
            return (1);
        }
        // printf("BUFFER BEFORE PARSING:\n"); //temp
        // ft_putstr(buffer); //temp
        //parse map + count rows and columns.
        // printf("HERE!\n"); //No segfault with this? WHYYYYYY?????
        map = parse_map(buffer);
        // write(1, "HELLLOOOOO!\n", 12);
        // free(buffer);
        if (!map)
        {
            ft_putchar('\n');
            return (1);
        }
        // ft_putstr(map[0]); //temp
        // ft_putcharpointer(map); //temp
        // write(1, "\n", 1); //temp
        //flood fill.
        map = flood_fill(map);
        //output map.
        ft_putcharpointer(map);
        //clean up
        ft_free(map);
    }
    else
        ft_putchar('\n');
    return (0);
}