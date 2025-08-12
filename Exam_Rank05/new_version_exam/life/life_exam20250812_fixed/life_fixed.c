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

// int ft_atoi(char *str) //NOT NEEDED
// {
//     int len = ft_strlen(str);
//     int i = 0;
//     int j = 0;
//     int n = 1;
//     len--;
//     // putchar(str[len]);
//     while(len >= 0)
//     {
//         j = str[len] - 'O';
//         i = j * n + i;
//         n *= 10;
//         len--;
//     }
    
//     // printf("I: %i\n", i);
//     return (i);
// }

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
            map[y][x] = ' ';
            x++;
        }
        map[y][x] = '\0';
        x = 0;
        y++;
    }
	map[y] = NULL; //Dont forget to NULL terminate.
    return (map);
}

//Include diagonally checks.
int ft_count_n(char **map, int y, int x, int cols, int rows)
{
    int count_n = 0;

    if (y > 0){
        if (map[y - 1][x] == 'O')
            count_n++;
		if (x > 0)
			if (map[y - 1][x - 1] == 'O')
				count_n++;
		if (x < cols - 1)
			if (map[y - 1][x + 1] == 'O')
				count_n++;
	}
    if (y < rows - 1){
        if (map[y + 1][x] == 'O')
			count_n++;
		if(x > 0)
			if (map[y + 1][x - 1] == 'O')
				count_n++;
		if (x < cols - 1)
			if (map[y + 1][x + 1] == 'O')
				count_n++;
	}
    if (x > 0)
        if (map[y][x - 1] == 'O')
            count_n++;
    if (x < cols - 1)
        if (map[y][x + 1] == 'O')
            count_n++;
    return(count_n);
}

void hellogenerations(char **map, int cols, int rows, int iterations)
{
    int i = 0;
    int x = 0;
    int y = 0;
    char **map2 = createMap(cols, rows); //Can use already made function to createMap.
    int count_n = 0;
    while(i < iterations)
    {
		for (int j = 0; j < rows; j++) {
            for (int k = 0; k < cols; k++) {
                map2[j][k] = ' ';
            }
        }
		// ft_putstrptr(map);
		// ft_putstr("=======\n");
		// ft_putstrptr(map2);
		while(map[y])
        {
			while(map[y][x] != '\0')
            {
				// putchar(map[y][x]);
				// putchar('\n');
                count_n = ft_count_n(map, y, x, cols, rows);
				// printf("COUNT_N: %i\n", count_n);
                if (map[y][x] == 'O')
                {
                    if (count_n == 2 || count_n == 3)
                        map2[y][x] = 'O';
                }
                else
                    if (count_n == 3)
                        map2[y][x] = 'O';
                x++;
            }
            x = 0;
            y++;
        }
        y = 0;
        i++;
		// ft_putstrptr(map2);
		// Swap maps. Copy map2 to map.
		for (int j = 0; j < rows; j++) {
			for (int k = 0; k < cols; k++) {
				map[j][k] = map2[j][k];
            }
        }
    }
	//Free map2.
	for (int y = 0; y < rows; y++)
        free(map2[y]);
    free(map2);
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
            map[pos_y][pos_x] = 'O';
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
        int cols = atoi(argv[1]);
        int rows = atoi(argv[2]);
        int iterations = atoi(argv[3]);

        char *buffer = readinstructions();
        if (!buffer)
            return 1;
        // ft_putstr(buffer); //temp
        char **map = createMap(cols, rows);
        if (!map)
        {
            free(buffer);
            return (1);
        }
        // ft_putstrptr(map); //temp
        gameOfLife(map, cols, rows, buffer);
        hellogenerations(map, cols, rows, iterations);
        ft_putstrptr(map); //temp

		//Clean up:
		free(buffer);
		for (int i = 0; i < rows; i++)
            free(map[i]);
        free(map);
    }

    else
    {
        ft_putstr("Invalid input\n");
        return (1);
    }
    return (0);
}