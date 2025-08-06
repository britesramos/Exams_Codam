#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void ft_putstr(char *str)
{
    int i = 0;
    while(str[i])
    {
        write(1, &str[i], 1);
        i++;
    }
}

void ft_putchar(char c)
{
    write(1, &c, 1);
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

int ft_map_error()
{
    ft_putstr("Map Error\n");
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
    int len_buf = ft_strlen(buffer);
    int len_temp = ft_strlen(temp);
    res = malloc((len_buf + len_temp + 1) * sizeof(char));
    if (!res){
        free(buffer);
        free(temp);
        return(NULL);
    }
    while(buffer[i]){
        res[i] = buffer[i];
        i++;
    }
    while(temp[j]){
        res[i + j] = temp[j];
        j++;
    }
    res[i + j] = '\0';
    return (res);
}

char* read_file(int fd)
{
    int bytes_read = 0;
    char *buffer = NULL;
    char *temp = NULL;
    buffer = malloc(5 * sizeof(char));
    if (!buffer)
        return (NULL);
    bytes_read = read(fd, buffer, 4);
    if (bytes_read == -1){
        free(buffer);
        return (NULL);
    }
    buffer[bytes_read] = '\0';
    while(bytes_read == 4)
    {
        temp = malloc(5 * sizeof(char));
        if (!temp){
            free(buffer);
            return (NULL);
        }
        bytes_read = read(fd, temp, 4);
        buffer = ft_strjoin(buffer, temp);
        if (!buffer)
            return (NULL);  
    }
    return (buffer);
}

int ft_count_cols(char *buffer)
{
    int cols = 0;
    while(buffer[cols] != '\n')
        cols++;
    return (cols);
}

int ft_count_rows(int cols, char *buffer)
{
    int rows = 0;
    int i = 0;
    int cols_check = 0;
    while(buffer[i])
    {
        if (buffer[i] != '\n' && buffer[i] != '.' && buffer[i] != 'X')
            return (-1);
        if (buffer[i] == '\n')
        {
            if (cols_check != cols)
                return (-1);
            rows++;
            cols_check = 0;
        }
        else
            cols_check++;
        i++;
    }
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
    map = malloc((num_rows + 1) * sizeof(char *));
    if (!map)
        return (NULL);
    while(buffer[i])
    {
        map[j] = malloc((num_cols + 1) * sizeof(char));
        if (!map[j])
        {
            j--;
            while(j > 0)
            {
                free(map[j]);
                j--;
            }
            free(map);
            return (NULL);
        }
        while(buffer[i] != '\n')
        {
            map[j][k] = buffer[i];
            i++;
            k++;
        }
        map[j][k] = '\n';
        j++;
        k = 0;
        i++;
    }
    map[j] = NULL;
    return (map);
}

int flood_fill_recursive(char **map, int y, int x, int rows, int cols, int island_id)
{
    if (x < 0 || y < 0 || x >= cols || y >= rows || map[y][x] != 'X') 
        return (0);
    int size = 1;
    map[y][x] = island_id + '0';
    size += flood_fill_recursive(map, y - 1, x, rows, cols, island_id);
    size += flood_fill_recursive(map, y + 1, x, rows, cols, island_id);
    size += flood_fill_recursive(map, y, x - 1, rows, cols, island_id);
    size += flood_fill_recursive(map, y, x + 1, rows, cols, island_id);
    return (size);
}

int ft_count_rows_map(char **map)
{
    int rows = 0;
    while(map[rows])
        rows++;
    return (rows);
}

int ft_coutn_cols_map(char **map)
{
    int cols = 0;
    while(map[0][cols] != '\n')
        cols++;
    return (cols);
}

int flood_fill(char **map)
{
    int rows = ft_count_rows_map(map);
    int cols = ft_coutn_cols_map(map);
    int y = 0;
    int x = 0;
    int biggest_island = 0;
    int current_island = 0;
    int island_id = 0;
    while(map[y])
    {
        while(map[y][x]){
            if (map[y][x] == 'X')
            {
                if (island_id > 9)
                    island_id = 0;
                current_island = flood_fill_recursive(map, y, x, rows, cols, island_id);
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
        if (fd == -1){
            
            ft_putstr("Here!\n");
            return (ft_map_error());
        }
        buffer = read_file(fd);
        if (!buffer){
            return (ft_map_error());
        }
        // ft_putstr(buffer); //temp.
        // write(1, "\n", 1); //temp.
        map = parse_map(buffer);
        biggest_island = flood_fill(map);
        ft_putcharptr(map);
        ft_itoa(biggest_island);
        write(1, "\n", 1);
    }
    else
        return (ft_map_error());
    return (0);
}