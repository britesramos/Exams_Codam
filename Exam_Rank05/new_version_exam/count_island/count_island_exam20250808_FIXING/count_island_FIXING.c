#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int error()
{
    write(1, "\n", 1);
    return(1);
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

int ft_strlen(char *str)
{
    int i = 0;
    while(str[i])
        i++;
    return (i);
}

void ft_putstrptr(char **map)
{
    // int i = 0;
    // int j = 0;
    // while(map[i])
    // {
    //     if (ft_strlen(map[i]) == 0)
    //         break ;
    //     while (map[i][j])
    //     {
    //         write(1, &map[i][j], 1);
    //         j++;
    //     }
    //     write(1, "\n", 1);
    //     j = 0;
    //     i++;
    // }
    int i = 0;
    while(map[i])
    {
        ft_putstr(map[i]);
        write(1, "\n", 1);
        i++;
    }
}

char* ft_strjoin(char *buffer, char *temp)
{
    int i = 0;
    int j = 0;
    int buffer_len = ft_strlen(buffer);
    int temp_len = ft_strlen(temp);
    char *res = NULL;
    res = malloc((buffer_len + temp_len + 1) * sizeof(char));
    if (!res)
    {
        free(buffer);
        free(temp);
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

char* read_file(int fd)
{
    int bytes_read = 0;
    char *buffer = NULL;
    char *temp = NULL;
    buffer = malloc((10 + 1) * sizeof(char));
    if (!buffer)
        return(NULL);
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
            return(NULL);
        }
        temp[bytes_read] = '\0';
        buffer = ft_strjoin(buffer, temp);
        if (!buffer)
            return(NULL);
        free(temp); //CHANGE
    }
    return (buffer);
}

int ft_count_cols(char *buffer)
{
    // int cols = 0;
    // int buffer_len = ft_strlen(buffer);
    // while(buffer[cols] != '\n')
    // {
    //     cols++;
    //     if (cols == buffer_len){
    //         return (cols);
    //     }
    // }
    // if (cols > 1024)
    //     return (-1);
    // return (cols);
    int i = 0;
    while(buffer[i] != '\n')
        i++;
    return (i); //  CHANGE
}

int ft_count_rows(int cols, char *buffer)
{
    int i = 0;
    int cols_check = 0;
    int rows = 0;
    while(buffer[i])
    {
        if (buffer[i] != '.' && buffer[i] != 'X' && buffer[i] != '\n')
            return (-1);
        if (buffer[i] == '\n')
        {
            if (cols_check != cols)
                return (-1);
            cols_check = 0;
            // rows++; //CHANGE
        }
        else
            cols_check++;
        rows++; //CHANGE
        i++;
    }
    // if (cols_check == cols) //CHANGE
    //     rows++; //CHANGE
    return (rows);
}

char** parse_map(char *buffer)
{
    char **map = NULL;
    int i = 0;
    int j = 0;
    int k = 0;
    int num_cols = ft_count_cols(buffer);
    // if (num_cols == -1) //CHANGE
    //     return (NULL);
    int num_rows = ft_count_rows(num_cols, buffer);
    if (num_rows == -1)
        return (NULL);
    // printf("ROWS: %i --- COLS: %i\n", num_rows, num_cols);
    map = malloc((num_rows + 1) * sizeof(char *));
    if (!map)
        return (NULL);
    while(j < num_rows)
    {
        map[j] = malloc((num_cols + 1) * sizeof(char));
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
        while(buffer[i] && buffer[i] != '\n') //IMPORTANT!
        {
            map[j][k] = buffer[i];
            k++;
            i++;
        }
        map[j][k] = '\0';
        k = 0;
        j++;
        // if (buffer[i] == '\n') //CHANGE
        i++;
    }
    map[j] = NULL;
    return (map);
}

int ft_count_cols_map(char **map)
{
    int cols = 0;
    while(map[0][cols])
        cols++;
    return (cols);
}

int ft_count_rows_map(char **map)
{
    int rows = 0;
    while(map[rows])
        rows++;
    return(rows);
}

void flood_fill_rec(char **map, int y, int x, int cols, int rows, int island_id)
{
    if (x < 0 || y < 0 || x >= cols || y >= rows || map[y][x] != 'X')
        return ;
    map[y][x] = island_id + '0';
    flood_fill_rec(map, y, x - 1, cols, rows, island_id);
    flood_fill_rec(map, y, x + 1, cols, rows, island_id);
    flood_fill_rec(map, y - 1, x, cols, rows, island_id);
    flood_fill_rec(map, y + 1, x, cols, rows, island_id);
}

void flood_fill(char **map)
{
    int rows = ft_count_rows_map(map);
    int cols = ft_count_cols_map(map);
    int y = 0;
    int x = 0;
    int island_id = 0;
    while(map[y])
    {
        while(map[y][x])
        {
            if (map[y][x] == 'X'){
                flood_fill_rec(map, y, x, cols, rows, island_id);
                island_id++;
            }
            x++;
        }
        x = 0;
        y++;
    }
}

int main(int argc, char **argv)
{
    int fd = 0;
    char *buffer = NULL;
    char **map = NULL;
    if (argc == 2)
    {
        fd = open(argv[1], O_RDONLY);
        if (fd == -1)
            return(1);
        buffer = read_file(fd);
        if (!buffer)
        {
            close(fd);
            return(error());
        }
        close(fd);
        // write(1, "BUFFER:\n", 8); //temp
        // ft_putstr(buffer); //temp
        map = parse_map(buffer);
        free(buffer);
        // write(1, "MAP:\n", 5); //temp
        // ft_putstrptr(map); //temp
        if (!map)
            return(error());
        flood_fill(map);
        // write(1, "FINAL:\n", 7); //temp
        ft_putstrptr(map);

    }
    else
        return(error());
    return (0);
}