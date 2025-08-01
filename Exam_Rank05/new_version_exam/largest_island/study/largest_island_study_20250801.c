#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

void ft_putchar(char c)
{
    write(1, &c, 1);
}

void ft_itoa(int n)
{
    if (n >= 10)
        ft_itoa(n / 10);
    ft_putchar((n % 10) + '0');
}

void ft_putstr(char *str)
{
    int i = 0;
    while(str[i])
    {
        write(1, &str[i], 1);
        i++;
    }
}

void ft_putcharptr(char **map)
{
    int i = 0;
    while(map[i])
    {
        ft_putstr(map[i]);
        i++;
    }
}

int ft_strlen(char *str)
{
    int i = 0;
    while(str[i])
        i++;
    return (i);
}

int ft_map_error()
{
    ft_putstr("Map Error.\n");
    return (1);
}

char* ft_strjoin(char *buffer, char *temp)
{
    int i = 0;
    int j = 0;
    int buffer_len = ft_strlen(buffer);
    int temp_len = ft_strlen(temp);
    char *res = NULL;
    res = malloc(buffer_len + temp_len + 1 * sizeof(char));
    if (!res)
    {
        free(buffer);
        return (NULL);
    }
    while(buffer[i])
    {
        res[i] = buffer[i];
        i++;
    }
    while(temp[j])
    {
        res[i + j] = temp[j];
        j++;
    }
    res[i + j] = '\0';
    free(buffer);
    return(res);
}

char* readfile(int fd)
{
    char *buffer = NULL;
    char *temp = NULL;
    int bytes_read = 0;
    buffer = malloc(10 + 1 * sizeof(char));
    if (!buffer)
        return (NULL);
    bytes_read = read(fd, buffer, 10);
    if (bytes_read == -1)
    {
        free(buffer);
        return (NULL);
    }
    buffer[bytes_read] = '\0';
    while(bytes_read == 10)
    {
        temp = malloc(10 + 1 * sizeof(char));
        if (!temp)
            return (NULL);
        bytes_read = read(fd, temp, 10);
        if (bytes_read == -1)
        {
            free(temp);
            free(buffer);
            return (NULL);
        }
        buffer = ft_strjoin(buffer, temp);
        if (!buffer)
        {
            free(temp);
            return (NULL);
        }
        free(temp);
    }
    return (buffer);
}

int ft_count_cols(char *buffer)
{
    int i = 0;
    while(buffer[i] != '\n')
        i++;
    // ft_putstr("COLS: ");
    // ft_itoa(i);
    return (i);
}

int ft_count_rows(int num_cols, char *buffer)
{
    int rows = 0;
    int check_cols = 0;
    int i = 0;
    while(buffer[i])
    {
        if (buffer[i] != '\n' && buffer[i] != '.' && buffer[i] != 'X')
        {
            return (-1);
        }
        if (buffer[i] == '\n')
        {
            if (check_cols != num_cols)
                return (-1);
            check_cols = 0; 
        }
        if (buffer[i] != '\n')
            check_cols++;
        rows++;
        i++;
    }
    // ft_putstr("ROWS: ");
    // ft_itoa(rows);
    return (rows);
}

char** parse_map(char *buffer)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int num_cols = ft_count_cols(buffer);
    int num_rows = ft_count_rows(num_cols, buffer);
    if (num_rows == -1)
        return (NULL);
    char **map = NULL;
    map = malloc(num_rows + 1 * sizeof(char *));
    if (!map)
        return (NULL);
    while(buffer[i])
    {
        map[j] = malloc(num_cols + 1 * sizeof(char));
        if (!map[j])
        {
            while(j > 0)
            {
                free(map[j]);
                j--;
            }
            free(map);
            return (NULL);
        }
        while(buffer[i] && buffer[i] != '\n')
        {
            map[j][k] = buffer[i];
            k++;
            i++;
        }
        map[j][k] = buffer[i];
        k++;
        map[j][k] = '\0';
        k = 0;
        j++;
        i++;
    }
    map[j] = NULL;
    return (map);
}

int ft_count_map_cols(char **map)
{
    int cols = 0;
    while(map[0][cols] != '\n')
        cols++;
    return (cols);
}

int ft_count_map_rows(char **map)
{
    int rows = 0;
    while(map[rows])
        rows++;
    return (rows);
}

int flood_fill_recursive(char **map, int y, int x, int rows, int cols)
{
    if (x < 0 || y < 0 || x >= cols || y >= rows || map[y][x] != 'X')
        return(0);
    map[y][x] = 'A';
    int size = 1;
    size += flood_fill_recursive(map, y - 1, x, rows, cols);
    size += flood_fill_recursive(map, y + 1, x, rows, cols);
    size += flood_fill_recursive(map, y, x - 1 ,rows, cols);
    size += flood_fill_recursive(map, y, x + 1, rows, cols);
    return (size);
}

void flood_fill(char **map)
{
    int cols = ft_count_map_cols(map);
    int rows = ft_count_map_rows(map);
    int biggest_island = 0;
    int current_island = 0;
    int i = 0;
    int j = 0;

    while(map[i])
    {
        while(map[i][j])
        {
            if (map[i][j] == 'X')
            {
                current_island = flood_fill_recursive(map, i, j, rows, cols);
                if (current_island > biggest_island)
                    biggest_island = current_island;
            }
            j++;
        }
        j = 0;
        i++;
    }
    ft_itoa(biggest_island);
}

int main(int argc, char **argv)
{
    int fd = 0;
    char *buffer = NULL;
    char** map = NULL;

    if (argc == 2)
    {
        //1)Open file:
        fd = open(argv[1], O_RDONLY);
        if (fd == -1)
            return (1);
        //2)Read file into buffer:
        buffer = readfile(fd);
        if (!buffer)
            return(ft_map_error());
        // ft_putstr(buffer); //temp
        //3)Parse map:
        map = parse_map(buffer);
        if (!map)
            return(ft_map_error());
        free(buffer);
        //4}Flood fill:
        flood_fill(map);
        // ft_putcharptr(map);
    }
    else
        return(ft_map_error());
    return (0);
}