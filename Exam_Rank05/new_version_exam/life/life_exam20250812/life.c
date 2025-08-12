#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void ft_putstr(char *str)
{
    int i = 0;
    while(str[i])
    {
        putchar(str[i]);
        i++;
    }
}

void ft_putstrptr(char **map)
{
    int i = 0;
    int j = 0;
    while(map[i])
    {
        while(map[i][j])
        {
            putchar(map[i][j]);
            j++;
        }
        putchar('\n');
        j = 0;
        i++;
    }
}

char* readinstructions()
{
    int bytes_read = 0;
    char *buffer = NULL;
    buffer = calloc(1000, sizeof(char));
    if (!buffer)
        return (NULL);
    bytes_read = read(0, buffer, 1000);
    return(buffer);
}

int ft_strlen(char *str)
{
    int i = 0;
    while(str[i])
        i++;
    return (i);

}

int ft_atoi(char *str)
{
    int len = ft_strlen(str);
    int i = 0;
    int j = 0;
    int n = 1;
    len--;
    // putchar(str[len]);
    while(len >= 0)
    {
        j = str[len] - '0';
        i = j * n + i;
        n *= 10;
        len--;
    }
    
    // printf("I: %i\n", i);
    return (i);
}

char **createMap(int cols, int rows)
{
    int y = 0;
    int x = 0;
    int i = 0;
    int j = 0;
    char **map = NULL;
    map = calloc(rows + 1, sizeof(char *));
    if (!map)
        return (NULL);
    while(y < rows)
    {
        // printf("Y: %i\n ROWS: %i\n", y, rows);
        map[i] = calloc(cols + 1, sizeof(char));
        if (!map[i])
        {
            while(i > 0)
            {
                i--;
                free(map[i]);
            }
            free(map);
            return (NULL);
        }
        while(x < cols)
        {
            map[i][j] = ' '; //change to spaces.
            j++;
            x++;
        }
        map[i][j] = '\0';
        j = 0;
        x = 0;
        i++;
        y++;
    }
    return (map);
}

int ft_count_n(char **map, int y, int x, int cols, int rows)
{
    int count_n = 0;

    if (y > 0)
        if (map[y - 1][x] == '0')
            count_n++;
    if (y < rows)
        if (map[y + 1][x] == '0')
            count_n++;
    if (x > 0)
        if (map[y][x - 1] == '0')
            count_n++;
    if (x < cols)
        if (map[y][x + 1] == '0')
            count_n++;
    return(count_n);
}

void hellogenerations(char **map, int cols, int rows, int iterations)
{
    int i = 0;
    int x = 0;
    int y = 0;
    // char **map2 = NULL;
    // map2 = calloc(rows + 1, sizeof(char*));
    // int k = 0;
    // int l = 0;
    // int j = 0;
    
    int count_n = 0;
    while(i < iterations)
    {
        // while(k < cols)
        // {
        //     map2[k] = calloc(cols + 1, sizeof(char));
        //     while(map[i][j]){
        //         map2[k][l] = map[i][j];
        //         l++;
        //         j++;
        //     }
        //     map[k][l] = '\0';
        //     l = 0;
        //     j = 0;
        //     i++;
        //     k++;
        // }
        // k = 0;
        // l = 0;
        while(map[y])
        {
            while(map[y][x])
            {
                count_n = ft_count_n(map, y, x, cols, rows);
                if (map[y][x] == '0') //is cell
                {
                    if (count_n < 2 || count_n > 3)
                        map[y][x] = ' ';
                }
                else
                    if (count_n == 3)
                        map[y][x] = '0';

                x++;
            }
            x = 0;
            y++;
        }
        // map = map2;
        // free(map2); // ???
        y = 0;
        i++;
    }
}

void gameOfLife(char **map, int cols, int rows, char *instructions)
{
    int pos_x = 0;
    int pos_y = 0;
    int x_on = 0;
    int i = 0;
    while(instructions[i])
    {
        if (x_on == 1)
            map[pos_y][pos_x] = '0';
        if (instructions[i] == 'w')
        {
            if (pos_y > 0)
                pos_y--;
        }
        else if (instructions[i] == 's')
        {
            if (pos_y < rows)
                pos_y++;
        }
        else if (instructions[i] == 'a')
        {
            if (pos_x > 0)
                pos_x--;
        }
        else if (instructions[i] == 'd')
        {
            if (pos_x < cols)
                pos_x++;
        }
        else if (instructions[i] == 'x')
        {
            if (x_on == 0)
                x_on = 1;
            else
                x_on = 0;
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    if (argc == 4){
        char *colsc = argv[1];
        char *rowsc = argv[2];
        char *iterationsc = argv[3];
        int cols = ft_atoi(colsc);
        int rows = ft_atoi(rowsc);
        int iterations = ft_atoi(iterationsc);

        char *buffer = NULL;
        buffer = readinstructions();
        if (!buffer)
            return 1;
        // ft_putstr(buffer); //temp
        char **map = NULL;
        map = createMap(cols, rows);
        if (!map)
        {
            // free(buffer);
            return (1);
        }
        // ft_putstrptr(map); //temp
        gameOfLife(map, cols, rows, buffer);
        if (iterations > 0)
            hellogenerations(map, cols, rows, iterations);
        ft_putstrptr(map); //temp
    }

    else
    {
        ft_putstr("Invalid input\n");
        return (1);
    }
    return (0);
}