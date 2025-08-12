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

void ft_putstrptr(char **map, int rows)
{
    int i = 0;
    int j = 0;
    while(i < rows)
    {
        j = 0;
        while(map[i][j])
        {
            putchar(map[i][j]);
            j++;
        }
        putchar('\n');
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
            return (NULL);
        x = 0;
        while(x < cols)
        {
            map[y][x] = ' ';
            x++;
        }
        map[y][x] = '\0';
        y++;
    }
    map[y] = NULL;
    return (map);
}

int ft_count_n(char **map, int y, int x, int cols, int rows)
{
    int count_n = 0;
    int dy, dx;
    
    for (dy = -1; dy <= 1; dy++) {
        for (dx = -1; dx <= 1; dx++) {
            if (dy == 0 && dx == 0) continue; // Skip current cell
            int ny = y + dy;
            int nx = x + dx;
            if (ny >= 0 && ny < rows && nx >= 0 && nx < cols) {
                if (map[ny][nx] == 'O')
                    count_n++;
            }
        }
    }
    return count_n;
}

void hellogenerations(char **map, int cols, int rows, int iterations)
{
    int i = 0;
    char **next_map = createMap(cols, rows);
    
    while(i < iterations)
    {
        // Copy current state to next_map first
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                next_map[y][x] = map[y][x];
            }
        }
        
        // Apply Game of Life rules to next_map based on current map
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                int count_n = ft_count_n(map, y, x, cols, rows);
                if (map[y][x] == 'O') {
                    // Live cell with < 2 or > 3 neighbors dies
                    if (count_n < 2 || count_n > 3)
                        next_map[y][x] = ' ';
                } else {
                    // Dead cell with exactly 3 neighbors becomes alive
                    if (count_n == 3)
                        next_map[y][x] = 'O';
                }
            }
        }
        
        // Swap maps
        // char **temp = map; //unused
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                map[y][x] = next_map[y][x];
            }
        }
        i++;
    }
    
    // Clean up next_map
    for (int y = 0; y < rows; y++)
        free(next_map[y]);
    free(next_map);
}

void gameOfLife(char **map, int cols, int rows, char *instructions)
{
    int pos_x = 0;
    int pos_y = 0;
    int x_on = 0;
    int i = 0;
    
    while(instructions[i])
    {
        if (instructions[i] == 'w')
        {
            if (pos_y > 0)
                pos_y--;
        }
        else if (instructions[i] == 'a')
        {
            if (pos_x > 0)
                pos_x--;
        }
        else if (instructions[i] == 's')
        {
            if (pos_y < rows - 1)
                pos_y++;
        }
        else if (instructions[i] == 'd')
        {
            if (pos_x < cols - 1)
                pos_x++;
        }
        else if (instructions[i] == 'x')
        {
            x_on = !x_on;
        }
        
        if (x_on && pos_x >= 0 && pos_x < cols && pos_y >= 0 && pos_y < rows)
        {
            map[pos_y][pos_x] = 'O';
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    if (argc == 4)
    {
        int cols = atoi(argv[1]);
        int rows = atoi(argv[2]);
        int iterations = atoi(argv[3]);
        
        char *buffer = readinstructions();
        if (!buffer)
            return (1);
            
        char **map = createMap(cols, rows);
        if (!map)
        {
            free(buffer);
            return (1);
        }
        
        gameOfLife(map, cols, rows, buffer);
        hellogenerations(map, cols, rows, iterations);
        ft_putstrptr(map, rows);
        
        // Clean up
        free(buffer);
        for (int i = 0; i < rows; i++)
            free(map[i]);
        free(map);
    }
    else
    {
        ft_putstr("Usage: ./life width height iterations\n");
        return (1);
    }
    return (0);
}