#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void ft_putstr(char *str)
{
    int i = 0;
    while(str[i])
    {
        putchar(str[i]);
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

char *ft_strjoin(char *buffer, char *temp)
{
    int i = 0;
    int j = 0;
    char *res = NULL;
    int buffer_len = ft_strlen(buffer);
    int temp_len = ft_strlen(temp);
    res = calloc(buffer_len + temp_len + 1, sizeof(char));
    if (!res){
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
    return (res);
}

char *readFile()
{
    char *buffer = NULL;
    char *temp = NULL;
    int bytes_read = 0;
    buffer = calloc(10 + 1, sizeof(char));
    if (!buffer)
        return (NULL);
    bytes_read = read(0, buffer, 10);
    if (bytes_read == -1)
    {
        free(buffer);
        return (NULL);
    }
    buffer[bytes_read] = '\0'; //not needed
    while(bytes_read == 10)
    {
        temp = calloc(10 + 1, sizeof(char));
        if (!temp)
        {
            free(buffer);
            return (NULL);
        }
        bytes_read = read(0, temp, 10);
        if (bytes_read == -1)
        {
            free(temp);
            free(buffer);
            return (NULL);
        }
        buffer = ft_strjoin(buffer, temp);
        if (!buffer)
            return (NULL);
        free(temp);
    }
    return (buffer);
}

void printMap(char **map)
{
    int i = 0;
    while(map[i])
    {
        ft_putstr(map[i]);
        putchar('\n');
        i++;
    }
}

char **createMap(int cols, int rows)
{
    int y = 0;
    int x = 0;
    char **map = NULL;
    map = calloc(rows + 1, sizeof(char *));
    if (!map)
        return (NULL);
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
            return (NULL);
        }
        while(x < cols)
        {
            map[y][x] = ' '; //Change to spaces.
            x++;
        }
        x = 0;
        y++;
    }
    return (map);
}

void applyMovements(char *buffer, char **map, int cols, int rows)
{
    int i = 0;
    int x = 0;
    int y = 0;
    int x_on = 0;
    while(buffer[i])
    {
        if (x_on == 1) //Move this to last if statment.
            map[y][x] = 'O';
        if (buffer[i] == 'x')
        {
            if (x_on == 0)
                x_on = 1;
            else
                x_on = 0;
        }
        if (buffer[i] == 'd')
        {
            if (x < cols - 1)
                x++;
        }
        if (buffer[i] == 's')
        {
            if (y < rows - 1)
                y++;
        }
        if (buffer[i] == 'a')
        {
            if (x > 0)
                x--;
        }
        if (buffer[i] == 'w')
        {
            if (y > 0)
                y--;
        }
        i++;
    }
}

int ft_count_n(char **map, int y, int x, int rows, int cols)
{
    int n = 0;
    if (y > 0)
    {
        if (map[y - 1][x] == 'O')
            n++;
        if (x > 0)
        {
            if (map[y - 1][x - 1] == 'O')
                n++;
        }
        if (x < cols - 1)
        {
            if (map[y - 1][x + 1] == 'O')
                n++;
        }
    }
    if (x > 0)
    {
        if (map[y][x - 1] == 'O')
            n++;
    }
    if (y < rows - 1)
    {
        if (map[y + 1][x] == 'O')
            n++;
        if (x > 0)
        {
            if (map[y + 1][x - 1] == 'O')
                n++;
        }
        if (x < cols - 1)
        {
            if (map[y + 1][x + 1] == 'O')
                n++;
        }
    }
    if (x < cols - 1)
    {
        if (map[y][x + 1] == 'O')
            n++;
    }
    return (n);
}

void gameOfLife(char **map, int cols, int rows, int iterations)
{
    char **temp_map = NULL;
    int y = 0;
    int x = 0;
    int i = 0;
    int j = 0;
    int count_n = 0;
    while(iterations > 0)
    {
        temp_map = createMap(cols, rows);
        if (!temp_map)
            return ; //Or break???
        while(map[i])
        {
            while(map[i][j])
            {
                temp_map[i][j] = map[i][j];
                j++;
            }
            j = 0;
            i++;
        }
        i = 0;
        while(map[y])
        {
            while(map[y][x])
            {
                count_n = ft_count_n(map, y, x, rows, cols);
                // printf("X: %i === Y: %i\n", x, y);
                if (map[y][x] != 'O')
                {
                    if (count_n == 3){
                        // ft_putstr("HERE\n");
                        temp_map[y][x] = 'O';
                    }
                }
                else
                {
                    if (count_n < 2)
                    {
                        // ft_putstr("HERE1\n");
                        temp_map[y][x] = ' '; //change to spaces
                    }
                    if (count_n > 3)
                    {
                        // ft_putstr("HERE2\n");
                        temp_map[y][x] = ' '; //change to spaces
                    }
                }
                x++;
            }
            x = 0;
            y++;
        }
        while(temp_map[i])
        {
            while(temp_map[i][j])
            {
                map[i][j] = temp_map[i][j];
                j++;
            }
            j = 0;
            i++;
        }
        i = 0;
        free(temp_map);
        y = 0;
        iterations--;
    }
}

int ft_isDigit(char *str)
{
    int i = 0;
    while(str[i])
    {
        if (!(str[i] >= '0' && str[i] <= '9'))
            return (1);
        i++;
    }
    return (0);
}

int main(int argc, char **argv)
{
    if (argc == 4)
    {
        char *buffer = NULL;
        char **map = NULL;
        int cols = atoi(argv[1]);
        int rows = atoi(argv[2]);
        char *iterations_c = argv[3];
        if (ft_isDigit(iterations_c) == 1)
            return (1);
        int iterations = atoi(argv[3]);
        buffer = readFile();
        if (!buffer)
            return (1);
        // ft_putstr(buffer); //temp
        map = createMap(cols, rows);
        if (!map)
            return (1);
        // printMap(map); //temp
        applyMovements(buffer, map, cols, rows);
        // printMap(map); //temp
        gameOfLife(map, cols, rows, iterations);
        // ft_putstr("AFTER\n"); //temp
        printMap(map);
    }
    else
        return (1);
    return (0);
}