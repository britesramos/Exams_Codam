#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int ft_strlen(char *str)
{
    int i = 0;
    while(str[i])
        i++;
    return (i);
}

char *ft_strjoin(char *buffer, char *temp)
{
    int i = 0;
    int j = 0;
    int buffer_len = ft_strlen(buffer);
    int temp_len = ft_strlen(temp);
    char *res;
    res = calloc(buffer_len + temp_len + 1, sizeof(char));
    if (!res)
    {
        free(buffer);
        return(NULL);
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
    res[i + j] = '\0'; //Not needed because of calloc.
    return (res);
}

char *readfile()
{
    char *buffer = NULL;
    char *temp = NULL;
    int bytes_read = 0;
    buffer = calloc((5 + 1), sizeof(char));
    if (!buffer)
        return (NULL);
    bytes_read = read(0, buffer, 5);
    if (bytes_read == -1)
    {
        free(buffer);
        return (NULL);
    }
    buffer[bytes_read] = '\0'; //Not needed becaus of calloc.
    while(bytes_read == 5)
    {
        temp = calloc((5 + 1), sizeof(char));
        if (!temp)
        {
            free(buffer);
            return (NULL);
        }
        bytes_read = read(0, temp, 5); //Read from 0.
        if (bytes_read == -1)
        {
            free(buffer);
            free(temp);
            return (NULL);
        }
        temp[bytes_read] = '\0';  //Not needed because of calloc.
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

void ft_putstr(char *str)
{
    int i = 0;
    while(str[i])
    {
        write(1, &str[i], 1);
        i++;
    }
}

void ft_printMap(char **map)
{
    int i = 0;
    while(map[i])
    {
        ft_putstr(map[i]);
        write(1, "\n", 1);
        i++;
    }
}

char **creategrid(int rows, int cols)
{
    char **map = NULL;
    int y = 0;
    int x = 0;
    map = calloc(rows + 1, sizeof(char*));
    if (!map)
        return NULL;
    while(y < rows)
    {
        map[y] = calloc(cols + 1, sizeof(char));
        if (!map[y])
        {
            while(y > 0)
            {
                y--;
                free(map[y]);
            }
            free(map);
        }
        while(x < cols)
        {
            map[y][x] = '.';
            x++;
        }
        x = 0;
        y++;
    }

    return (map);
}

void follow_instructions(char *buffer, char **map, int rows, int cols)
{
    int i = 0;
    int y = 0;
    int x = 0;
    int is_x_on = 0;
    while(buffer[i])
    {
        if (is_x_on == 1) //Move this check to last.
            map[y][x] = 'O';
        if (buffer[i] == 'x'){
            if (is_x_on == 0)
                is_x_on = 1;
            else
                is_x_on = 0;
        }
        if (buffer[i] == 'd')
            if (x < cols - 1)
                x++;
        if (buffer[i] == 's')
            if (y < rows - 1)
                y++;
        if (buffer[i] == 'a')
            if (x > 0)
                x--;
        if (buffer[i] == 'w')
            if (y > 0)
                y--;
        i++;
    }
}

int ft_count_n(char **map, int y, int x, int rows, int cols)
{
    int count_n = 0;
    if (y > 0)
    {
        if (map[y - 1][x] == 'O')
            count_n++;
        if (x > 0)
            if (map[y - 1][x - 1] == 'O')
                count_n++;
        if (x < cols - 1)
            if (map[y - 1][x + 1] == 'O')
                count_n++;
    }
    if (x > 0)
        if (map[y][x - 1] == 'O')
            count_n++;
    if (y < rows - 1)
    {
        if (map[y + 1][x] == 'O')
            count_n++;
        if (x > 0)
            if (map[y + 1][x - 1] == 'O')
                count_n++;
        if (x < cols - 1)
            if (map[y + 1][x + 1] == 'O')
                count_n++;
    }
    if (x < cols - 1)
        if (map[y][x + 1] == 'O')
            count_n++;

    return(count_n);
}

char **ft_iterations(int iterations, char **map, int rows, int cols)
{
    char **map_temp;
    int y = 0;
    int x = 0;
    int i = 0;
    int j = 0;
    int count_n = 0;
    while(iterations > 0)
    {   
        map_temp = creategrid(rows, cols); //outside the loop.
        if (!map_temp)
            return (NULL);
        while(map[y])
        {
            while(map[y][x])
            {
                count_n = ft_count_n(map, y, x, rows, cols);
                if (map[y][x] == 'O')
                {
                    if (count_n == 2 || count_n == 3)
                        map_temp[y][x] = 'O';
                }
                else{
                    if (count_n == 3)
                        map_temp[y][x] = 'O';
                }
                x++;
            }
            x = 0;
            y++;
        }
        y = 0;
        while(map_temp[i])
        {
            while(map_temp[i][j])
            {
                map[i][j] = map_temp[i][j];
                j++;
            }
            j = 0;
            i++;
        }
        i = 0;
        iterations--;
    }
    free(map_temp);
    return(map);
}

int main(int argc, char **argv)
{
    char *buffer = NULL;
    char **map = NULL;
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    if (argc == 4)
    {
        buffer = readfile();
        if (!buffer)
        {
            write(1, "\n", 1);
            return 1;
        }
        // ft_putstr(buffer); //temp
        map = creategrid(rows, cols);
        if (!map){
            free(buffer);
            return (1);
        }
        // ft_printMap(map);
        follow_instructions(buffer, map, rows, cols);
        free(buffer);
        if (iterations > 0)
            ft_iterations(iterations, map, rows, cols);
        ft_printMap(map);
    }
    else
        write(1, "\n", 1);
    return (0);
}