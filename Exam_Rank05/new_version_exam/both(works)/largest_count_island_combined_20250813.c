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

void printMap(char **map)
{
    int i = 0;
    while(map[i])
    {
        ft_putstr(map[i]);
        write(1, "\n", 1);
        i++;
    }
}

int ft_error()
{
    ft_putstr("Error map!\n");
    return (1);
}

int ft_strlen(char *str)
{
    int i = 0;
    while(str[i])
        i++;
    return (i);
}

char* ft_strjoin(char *buffer, char *temp)
{
    int i = 0;
    int j = 0;
    char *res = NULL;
    int buffer_len = ft_strlen(buffer);
    int temp_len = ft_strlen(temp);
    res = malloc((buffer_len + temp_len + 1) * sizeof(char));
    if (!res){
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
    return (res);
}

char *readFile(int fd)
{
    char *buffer = NULL;
    char *temp = NULL;
    int bytes_read = 0;
    buffer = malloc((10 + 1) * sizeof(char));
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
        temp = malloc((10 + 1) * sizeof(char));
        if (!temp)
        {
            free(buffer);
            return (NULL);
        }
        bytes_read = read(fd, temp, 10);
        if (bytes_read == -1)
        {
            free(temp);
            free(buffer);
            return (NULL);
        }
        temp[bytes_read] = '\0';
        buffer = ft_strjoin(buffer, temp);
        free(temp);
        if (!buffer)
            return (NULL);
    }
    return (buffer);
}

int ft_count_cols(char *buffer)
{
    int cols = 0;
    while(buffer[cols] && buffer[cols] != '\n') 
        cols++;
    return (cols);
}

int ft_count_rows(int cols, char *buffer)
{
    int i = 0;
    int rows = 0;
    int cols_check = 0;
    while(buffer[i])
    {
        if (buffer[i] != 'X' && buffer[i] != '.' && buffer[i] != '\n')
            return (-1);
        if (buffer[i] == '\n')
        {
            if (cols_check != cols)
                return (-1);
            cols_check = 0;
            rows++;
        }
        else
           cols_check++;
        i++;
    }
    if (cols_check == cols)
        rows++;
    return (rows);
}

char **parseMap(char *buffer)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int cols = ft_count_cols(buffer);
    int rows = ft_count_rows(cols, buffer);
    if (rows == -1)
    return (NULL);
    char **map = NULL;
    map = malloc((rows + 1) * sizeof(char *));
    if (!map)
        return (NULL);
    while(buffer[i])
    {
        if (!buffer[i])
            break ;
        map[j] = malloc((cols + 1) * sizeof(char));
        if (!map[j])
        {
            while(j > 0)
            {
                j--;
                free(map[j]);
            }
            free(map);
            return (NULL);
        }
        while(buffer[i] && buffer[i] != '\n')
        {
            map[j][k] = buffer[i];
            i++;
            k++;
        }
        j++;
        k = 0;
        i++;
    }
    return (map);

}

void ft_putchar(char c)
{
    write(1, &c, 1);
}

void ft_itoa(int n)
{
    if (n > 9)
        ft_itoa(n / 10);
    ft_putchar((n % 10) + '0');
}

int floodfill_rec(char **map, int y, int x, int cols, int rows, int island_id)
{
    if (x < 0 || y < 0 || x >= cols || y >= rows || map[y][x] != 'X') //Bigger or equal is out of bounds.
        return 0;
    map[y][x] = island_id + '0';
    int size = 1;
    size += floodfill_rec(map, y - 1, x, cols, rows, island_id);
    size += floodfill_rec(map, y + 1, x, cols, rows, island_id);
    size += floodfill_rec(map, y, x - 1, cols, rows, island_id);
    size += floodfill_rec(map, y, x + 1, cols, rows, island_id);
    return (size);
}

int floodfill(char **map, char *buffer)
{
    int cols = ft_count_cols(buffer);
    int rows = ft_count_rows(cols, buffer);
    int y = 0;
    int x = 0;
    int current_island = 0;
    int biggest_island = 0;
    int island_id = 0;
    while(map[y])
    {
        while (map[y][x])
        {
            if (map[y][x] == 'X')
            {
                if (island_id > 9)
                    island_id = 0;
                current_island = floodfill_rec(map, y, x, cols, rows, island_id);
                if (current_island > biggest_island)
                    biggest_island = current_island;
                island_id++;
            }
            x++;
        }
        x = 0;
        y++;
    }
    return biggest_island;
}

void ft_freeMap(char **map)
{
    int i = 0;
    while(map[i])
    {
        free(map[i]);
        i++;
    }
    free(map);
}

int main(int argc, char **argv)
{
    int fd = 0;
    char *buffer = NULL;
    char **map = NULL;
    int biggest_island = 0;
    if (argc == 2)
    {
        fd = open(argv[1], O_RDONLY);
        if (fd == -1)
            return (ft_error());
        buffer = readFile(fd);
        close(fd);
        if (!buffer)
            return (ft_error());
        // ft_putstr(buffer); //temp
        map = parseMap(buffer);
        if (!map){
            free(buffer);
            return (ft_error());
        }
        // printMap(map); //temp
        biggest_island = floodfill(map, buffer);
        free(buffer);
        printMap(map);
        write(1, "BIGGEST_ISLAND: ", 16);
        ft_itoa(biggest_island);
        ft_freeMap(map);
    }
    else
        return (ft_error());
    return (0);
}