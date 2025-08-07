#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

void ft_putstr(char *str)
{
    int i = 0;
    while(str[i])
    {
        write(1, &str[i], 1);
        i++;
    }
}

int ft_map_error()
{
    ft_putstr("Map Error!");
    write(1, "\n", 1);
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
        free(temp);
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

char* read_file(int fd)
{
    char* buffer = NULL;
    char *temp = NULL;
    int bytes_read = 0;

    buffer = malloc((5 + 1) * sizeof(char));
    if (!buffer)
        return (NULL);
    bytes_read = read(fd, buffer, 5);
    if (bytes_read == -1)
    {
        free(buffer);
        return (NULL);
    }
    buffer[bytes_read] = '\0';
    while(bytes_read == 5)
    {
        temp = malloc((5 + 1) * sizeof(char));
        if (!temp)
        {
            free(buffer);
            return (NULL);
        }
        bytes_read = read(fd, temp, 5);
        temp[bytes_read] = '\0'; //DONT FORGET
        buffer = ft_strjoin(buffer, temp);
        if (!buffer)
            return (NULL);
        free(temp);
    }
    
    return (buffer);
}

int ft_count_cols(char* buffer)
{
    int i = 0;
    while(buffer[i] != '\n')
        i++;
    return (i);
}

int ft_count_rows(int cols_count, char *buffer)
{
    int i = 0;
    int j = 0;
    int cols_check = 0;
    while(buffer[i])
    {
        if (buffer[i] != 'X' && buffer[i] != '.' && buffer[i] != '\n')
            return (-1);
        if (buffer[i] == '\n'){
            if (cols_check != cols_count)
                return (-1);
            cols_check = 0;
        }
        else
            cols_check++;
        j++;
        i++;
    }
    return (j);
}

char** parse_map(char* buffer)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int cols_count = ft_count_cols(buffer);
    int row_count = ft_count_rows(cols_count, buffer);
    if (row_count == -1)
    return (NULL);
    char **map = malloc(row_count + 1 * sizeof(char *));
    if (!map)
        return (NULL);
    while(buffer[i])
    {
        // write(1, "HELLO", 5);
        map[j] = malloc((cols_count + 1) * sizeof(char));
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
        while (buffer[i] && buffer[i] != '\n')
        {
            map[j][k] = buffer[i];
            k++;
            i++;
        }
        map[j][k] = '\0';
        k = 0;
        j++;
        i++;

    }
    map[j] = NULL; //DONT FORGET TO NULL TERMINATE THE MAP MAIN CONTAINER.
    // write(1, "Hola", 4);
    return (map);
}

void ft_putcharptr(char **map)
{
    int i = 0;
    while(map[i])
    {
        ft_putstr(map[i]);
        write(1, "\n", 1);
        i++;
    }
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

int ft_count_map_cols(char **map)
{
    int i = 0;
    while(map[0][i])
        i++;
    return (i);
}

int ft_count_map_rows(char **map)
{
    int i = 0;
    while(map[i])
        i++;
    return (i);
}

int flood_fill_recursive(char **map, int rows, int cols, int x, int y, int island_id)
{
    if (x < 0 || y < 0 || x >= cols || y >= rows || map[y][x] != 'X')
        return (0);
    int size = 1;
    map[y][x] = island_id + '0';
    size += flood_fill_recursive(map, rows, cols, x, y - 1, island_id);
    size += flood_fill_recursive(map, rows, cols, x, y + 1, island_id);
    size += flood_fill_recursive(map, rows, cols, x - 1, y, island_id);
    size += flood_fill_recursive(map, rows, cols, x + 1, y, island_id);
    return (size);
}

int flood_fill(char **map)
{
    int rows = ft_count_map_rows(map);
    int cols = ft_count_map_cols(map);
    int biggest_island = 0;
    int current_island = 0;
    int y = 0;
    int x = 0;
    int island_id = 0;
    while(map[y])
    {
        while(map[y][x])
        {
            if (map[y][x] == 'X'){
                if (island_id > 9)
                    island_id = 0;
                current_island = flood_fill_recursive(map, rows, cols, x, y, island_id);
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
            return (1);
        buffer = read_file(fd);
        if(!buffer){
            close(fd);
            return(ft_map_error());
        }
        // ft_putstr(buffer); //temp
        close(fd);
        map = parse_map(buffer);
        if (!map)
            return (ft_map_error());
        free(buffer);
        biggest_island = flood_fill(map);
        ft_putcharptr(map); //temp
        ft_itoa(biggest_island);
        write(1, "\n", 1);

    }
    else
        return(ft_map_error());
    return (0);
}