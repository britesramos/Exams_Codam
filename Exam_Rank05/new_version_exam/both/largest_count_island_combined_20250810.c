#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
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

void ft_putcharptr(char **map)
{
    int i = 0;
    while(map[i]){
        ft_putstr(map[i]);
        write(1, "\n", 1);
        i++;
    }
}

int ft_error()
{
    ft_putstr("Error map\n");
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
    int buffer_len = ft_strlen(buffer);
    int temp_len = ft_strlen(temp);
    char *res = NULL;
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

char* readfile(int fd)
{
    int bytes_read = 0;
    char *buffer = NULL;
    char *temp = NULL;
    buffer = malloc((10 + 1) * sizeof(char));
    if (!buffer)
        return (NULL);
    bytes_read = read(fd, buffer, 10);
    if (bytes_read == -1)
    {
        free(buffer);
        return(NULL);
    }
    buffer[bytes_read] = '\0';
    while(bytes_read == 10)
    {
        temp = malloc((10 + 1) * sizeof(char));
        if (!temp)
        {
            free(buffer);
            return(NULL);
        }
        bytes_read = read(fd, temp, 10);
        if (bytes_read == -1)
        {
            free(temp);
            free(buffer);
            return(NULL);
        }
        temp[bytes_read] = '\0'; //Dont forget null terminate temp.
        buffer = ft_strjoin(buffer, temp);
        if (!buffer){
            free(temp);
            return(NULL);
        }
        free(temp);
    }
    return (buffer);
}

int ft_count_cols(char *buffer)
{
    int cols = 0;
    while(buffer[cols] != '\n' && buffer[cols] != '\0') //missing null check.
        cols++;
    return (cols);
}

int ft_count_rows(int cols, char *buffer)
{
    int rows = 0;
    int check_cols = 0;
    int i = 0;

    while(buffer[i])
    {
        if (buffer[i] != '\n' && buffer[i] != '.' && buffer[i] != 'X')
            return (-1);
        if (buffer[i] == '\n')
        {
            if (check_cols != cols)
                return (-1);
            check_cols = 0;
            rows++;
        }
        else
            check_cols++;
        i++;
    }
    //Might need extra row.
    // if (check_cols == cols)
    //     rows++;
    return (rows);
}

char** parseMap(char *buffer)
{
    int i = 0;
    int j = 0;
    int k = 0;
    char **map = NULL;
    int cols = ft_count_cols(buffer);
    int rows = ft_count_rows(cols, buffer);
    // printf("\nCOLS: %i\nROWS: %i\n", cols, rows); //temp
    if (rows == -1)
        return (NULL);
    map = malloc((rows + 1) * sizeof(char *));
    if (!map)
        return (NULL);
    while(buffer[i])
    {
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
        while(buffer[i] != '\n' && buffer[i] != '\0') //important check null terminator
        {
            map[j][k] = buffer[i];
            k++;
            i++;
        }
        map[j][k] = '\0';
        k = 0;
        j++;
        if (buffer[i] == '\n') //extra check.
            i++;
    }
    map[j] = NULL;
    return (map);
}

int ft_count_cols_map(char **map)
{
    int cols = 0;
    while(map[0][cols] != '\0') //Check for null terminator not new line.
        cols++;
    return (cols);
}

int ft_count_rows_map(char **map)
{
    int rows = 0;
    while(map[rows])
        rows++;
    return (rows);
}

int floodfill_rec(char **map, int y, int x, int rows, int cols, int island_id)
{
    if (x < 0 || y < 0 || x >= cols || y >= rows || map[y][x] != 'X')
        return (0);
    int size = 1;
    map[y][x] = island_id + '0';
    size += floodfill_rec(map, y, x - 1, rows, cols, island_id);
    size += floodfill_rec(map, y, x + 1, rows, cols, island_id);
    size += floodfill_rec(map, y - 1, x, rows, cols, island_id);
    size += floodfill_rec(map, y + 1, x, rows, cols, island_id);
    return (size);
}

int floodfill(char **map)
{
    int cols = ft_count_cols_map(map);
    int rows = ft_count_rows_map(map);
    int y = 0;
    int x = 0;
    int island_id = 0;
    int current_island = 0;
    int biggest_island = 0;
    while(map[y])
    {
        while(map[y][x])
        {
            if (map[y][x] == 'X')
            {
                if (island_id > 9)
                    island_id = 0;
                current_island = floodfill_rec(map, y, x, rows, cols, island_id);
                if (current_island > biggest_island)
                    biggest_island = current_island;
                island_id++;
            }
            x++;
        }
        x = 0;
        y++;
    }
    return (biggest_island);
}

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
            return(ft_error());
        buffer = readfile(fd);
        close(fd);
        if (!buffer)
            return(ft_error());
        // ft_putstr(buffer); //temp
        map = parseMap(buffer);
        if (!map)
            return(ft_error());
        biggest_island = floodfill(map);
        ft_putcharptr(map);
        write(1, "BIGGEST ISLAND: ", 16);
        ft_itoa(biggest_island);
    }
    else
        return(ft_error());
    return 0;
}