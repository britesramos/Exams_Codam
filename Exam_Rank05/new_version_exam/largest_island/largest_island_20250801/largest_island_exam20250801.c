#include <unistd.h>
#include <fcntl.h>
// #include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int ft_strlen(char *str)
{
    int i = 0;
    while(str[i])
        i++;
    return (i);
}

void ft_putchar(char c)
{
    write(1, &c, 1);
}

void ft_itoa(int n)
{
    if (n >= 10)
        ft_itoa (n / 10);
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
    int j = 0;
    while(map[i])
    {
        while (map[i][j])
        {
            write(1, &map[i][j], 1);
            j++;
        }
        j = 0;
        i++;
        // write(1, "HERE", 4);
    }
}

int ft_map_error()
{
    ft_putstr("Map Error\n");
    return (1);
}

char* ft_strjoin(char *buffer, char* temp)
{
    int i = 0;
    int j = 0;
    int len_buffer = ft_strlen(buffer);
    int len_temp = ft_strlen(temp);
    char *res = NULL;
    res = malloc((len_buffer + len_temp) + 1 * sizeof(char));
    if (!res)
        return (NULL);
    while (i < len_buffer)
    {
        res[i] = buffer[i];
        i++;
    }
    while (j < len_temp)
    {
        res[i + j] = temp[j];
        j++;
    }
    // res[i + j] = '\0'; //CHANGED
    return (res); 
}

char* read_file(int fd)
{
    int bytes_read = 0;
    char * buffer = NULL;
    char * temp = NULL;

    buffer = malloc(6 * sizeof(char));
    if (!buffer)
    return (NULL);
    bytes_read = read(fd, buffer, 5);
    if (bytes_read == -1)
    {
        free(buffer);
        return (NULL);
    }
    buffer[bytes_read] = '\0';
    temp = malloc(6 * sizeof(char));
    if (!temp){
        free(buffer);
        return (NULL);
    }
    while (bytes_read == 5 || bytes_read > 0)
    {
        bytes_read = read(fd, temp, 5);
        if (bytes_read == 0)
            break ;
        buffer = ft_strjoin(buffer, temp);
        if (!buffer)
        {
            free(temp);
            return (NULL);
        }
        // for(int i = 0; i < 4; ++i)
        //     temp[i] = '\0';
    }
    // ft_putstr(buffer);
    // write(1, "---\n", 4);
    // write(1,"HELLO!\n", 7); //temp
    return (buffer);
}

int ft_count_col(char *buffer)
{
    int col = 0;
    while(buffer[col] != '\n')
    {
        col++;
    }
    return (col);
}

int ft_count_row(int count_col, char *buffer)
{
    int i = 0;
    int row = 0;
    int check_col = 0;
    (void)count_col;
    (void)check_col;
    ft_putstr(buffer);
    while (buffer[i])
    {
        if (buffer[i] != '.' && buffer[i] != 'X' && buffer[i] != '\n'){
            return (-1);
        }
        if (buffer[i] == '\n')
        {
            // write(1, "HERE\n", 5);
            if (check_col != count_col){
                // printf("COL: %i --- COL_CHECK: %i\n", count_col, check_col);
                return(-1);
            }
            row++;
            check_col = 0;
        }
        if (buffer[i] != '\n')
            check_col++;
        i++;
    }
    return(row);
}

char** parse_map(char *buffer)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int count_col = ft_count_col(buffer);
    int count_row = ft_count_row(count_col, buffer);
    // printf("COL: %i \n ROW: %i\n", count_col, count_row); //temp
    if (count_row == -1){
        return (NULL);
    }
    char **map = malloc(1000 * sizeof(char*)); //THERE IS A PROBLEM HERE WITH LARGE MAPS.
    if (!map){
        //  write(1, "HERE\n", 5);
        return (NULL);
    }
    // for(int i = 0; i < 1000; ++i)
    // {
    //     map[i] = "\0";
    // }
    while (k < count_row)
    {
        map[k] = malloc(1000 * sizeof(char));
        if (!map[k])
        {
            while(k > 0)
            {
                free(map[k]);
                k--;
            }
            free(map);
            // write(1, "HERE\n", 5);
            return (NULL);
        }
        // for(int a = 0; a < 1000; ++a)
        // {
        //     map[k][a] = '\0';
        // }
        while(buffer[i] != '\n')
        {
            map[k][j] = buffer[i];
            i++;
            j++;
        }
        map[k][j] = '\n';
        j = 0;
        k++;
        i++;
    }
    return (map);
}

int rec_flood_fill(int rows, int col, int x, int y, char **map)
{
    if (x > col || y > rows || x < 0 || y < 0 || (map[y][x] != 'X'))
        return (0);
   
    map[y][x] = 'A';
    int size = 1;

    size += rec_flood_fill(rows, col, x - 1, y, map);
    size += rec_flood_fill(rows, col, x + 1, y, map);
    size += rec_flood_fill(rows, col, x, y - 1, map);
    size += rec_flood_fill(rows, col, x, y + 1, map);
    return(size);
}

void floodfill(char *buffer, char **map)
{
    int x = 0;
    int y = 0;
    int largest_island = 0;
    int current_island = 0;
    int col = ft_count_col(buffer);
    int rows = ft_count_row(col, buffer);
    while(map[y])
    {
        while (map[y][x])
        {
            if (map[y][x] == 'X'){
                current_island = rec_flood_fill(rows, col, x, y, map);
                if (current_island > largest_island){
                    largest_island = current_island;
                }
            }
            x++;
        }
        x = 0;
        y++;
    }
    ft_itoa(largest_island);
    write(1, "\n", 1);
}

int main(int argc, char **argv)
{
    int fd = 0;
    char *buffer = NULL;
    char **map;

    if (argc == 2)
    {
        //1)Open file.
        fd = open(argv[1], O_RDONLY);
        if (fd == -1)
            return(ft_map_error());
        // else
        //     write(1, "FD SUCESS\n", 10);
        //2)Read into buffer.
        buffer = read_file(fd);
        if (buffer == NULL)
        {
            // write(1, "HERE\n", 5);
            return(ft_map_error());
        }

        // ft_putstr(buffer); //temp

        //3)Parse map
        map = parse_map(buffer);
        if (map == NULL){
            // write(1, "HERE\n", 5);
            return(ft_map_error());
        }
        //4)Flood fill + count largest_island.
        floodfill(buffer, map);
        // ft_putcharptr(map); //temp

    }
    else
        return (ft_map_error());
    return (0);
}