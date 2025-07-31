#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
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

void ft_putcharpointer(char **pointer)
{
    int i = 0;
    while (pointer[i])
    {

        // printf("%i\n", i);
        ft_putstr(pointer[i]);
        write(1, "\n", 1);
        i++;
    }
    // ft_putstr(pointer[i]);
    // write(1, "\n", 1);
}

void ft_putchar(char c)
{
    write(1, &c, 1);
}

void ft_itoa(int n)
{
    if(n >= 10)
        ft_itoa(n / 10);
    ft_putchar ((n % 10) + '0');
}

int ft_strlen(char *str)
{
    int i = 0;
    while(str[i])
        i++;
    return (i);
}

char* ft_strjoin(char * buffer, char *temp)
{
    int i = 0;
    int j = 0;
    int buffer_len = ft_strlen(buffer);
    int temp_len = ft_strlen(temp);
    int total_len = buffer_len + temp_len;
    char *result = malloc((total_len + 1) * sizeof(char));
    if (!result)
        return (NULL);
    
    while (i < buffer_len)
    {
        result[i] = buffer[i];
        i++;
    }
    while (j < temp_len)
    {
        result[i + j] = temp[j];
        j++;
    }
    result[total_len] = '\0';
    free(buffer);
    return (result);
}

char* read_file(int fd)
{
    char *buffer = NULL;
    char *temp = NULL;
    int bytes_read = 0;
    buffer = malloc(1024 * sizeof(char));
    if (!buffer)
        return (NULL);
    bytes_read = read(fd, buffer, 1023); // Leave space for null terminator
    if (bytes_read < 0)
    {
        free(buffer);
        return (NULL);
    }
    buffer[bytes_read] = '\0'; // Null terminate
    
    temp = malloc(1024 * sizeof(char));
    if (!temp)
    {
        free(buffer);
        return (NULL);
    }
    while (bytes_read > 0 && bytes_read == 1023) // Only continue if buffer was full
    {
        bytes_read = read(fd, temp, 1023);
        if (bytes_read <= 0)
            break ;
        temp[bytes_read] = '\0'; // Null terminate temp
        buffer = ft_strjoin(buffer, temp);
        if (!buffer)
        {
            free(temp);
            return (NULL);
        }
    }
    free(temp);
    return (buffer);
}

int ft_len_first_row(char *buffer)
{
    int len = 0;
    while (buffer[len] != '\n' && len < 1024){
        // write(1, "2HELLLOOOO!\n", 12);
        len++;

    }
    return (len);
}

int ft_count_rows(char *buffer, int first_row_len)
{
    int count_rows = 0;
    int i = 0;
    int row_len = 0;
    while (buffer[i])
    {
        if (buffer[i] != 'X' && buffer[i] != '.' && buffer[i] != '\n')
            return (-1);
        if (buffer[i] == '\n'){
            // printf("ROW_LEN: %i\n", row_len); //temp
            if (row_len == 0 || row_len != first_row_len)
                return (-1);
            count_rows++;
            row_len = 0;
        }
        else
            row_len++;
        i++;
    }
    // if (row_len == 0 || row_len != first_row_len){
    //     // printf("EXTRA CHECK!\n\n"); //temp
    //     return (-1);
    // }
    count_rows++;
    return (count_rows);
}

char** parse_map(char *buffer)
{
    char **map = NULL;
    int i = 0;
    int j = 0;
    int k = 0;
    
    if (!buffer || *buffer == '\0')
        return (NULL);
        
    int len_first_row = ft_len_first_row(buffer);
    int count_rows = ft_count_rows(buffer, len_first_row);
    if (count_rows == -1)
        return (NULL);
    map = malloc((count_rows + 1) * sizeof(char *));
    if (!map)
        return (NULL);
    while (buffer[i] && j < count_rows)
    {
        map[j] = malloc((len_first_row + 1) * sizeof(char));
        if (!map[j])
        {
            // Free previously allocated memory
            while (--j >= 0)
                free(map[j]);
            free(map);
            return (NULL);
        }
        while (buffer[i] && buffer[i] != '\n')
        {
            map[j][k] = buffer[i];
            i++;
            k++;
        }
        map[j][k] = '\0';
        if (!buffer[i])
            break ;
        k = 0;
        j++;
        i++;
    }
    map[j + 1] = NULL; // Properly terminate the array
    return (map);
}

int ft_max_rows(char **map)
{
    int res = 0;
    while(map[res])
        res++;
    return (res);
}

int ft_line_len(char **map)
{
    int len = 0;
    while(map[0][len])
        len++;
    return (len);
}

void flood_fill_recursive(char **map, int row, int col, int max_rows, int line_len, char island_char)
{
    // Base cases: check bounds and if current cell is not 'X'
    if (row < 0 || row >= max_rows || col < 0 || col >= line_len || map[row][col] != 'X')
        return;
    
    // Mark current cell as part of the island
    map[row][col] = island_char;
    
    // Recursively fill all 4 adjacent cells (up, down, left, right)
    flood_fill_recursive(map, row - 1, col, max_rows, line_len, island_char); // up
    flood_fill_recursive(map, row + 1, col, max_rows, line_len, island_char); // down
    flood_fill_recursive(map, row, col - 1, max_rows, line_len, island_char); // left
    flood_fill_recursive(map, row, col + 1, max_rows, line_len, island_char); // right
}

char** flood_fill(char **map)
{
    int i = 0;
    int j = 0;
    int island_num = 0;
    int max_rows = ft_max_rows(map);
    int line_len = ft_line_len(map);

    while(map[i])
    {
        while (map[i][j])
        {
            if (map[i][j] == 'X')
            {
                // Start recursive flood fill from this position
                flood_fill_recursive(map, i, j, max_rows, line_len, island_num + '0');
                island_num++;
            }
            j++;
        }
        j = 0;
        i++;
    }
    return (map);
}

void ft_free(char **map)
{
    int i = 0;
    while(map[i])
    {
        free(map[i]);
        i++;
    }
    free(map);
}

int return_error()
{
    ft_putchar('\n');
    return (1);

}

int main (int argc, char **argv)
{
    int fd = 0;
    char *buffer = NULL;
    char **map = NULL;
    if (argc == 2)
    {
        //1)open file
        fd = open(argv[1], O_RDONLY);
        if (fd == -1)
            return (return_error());
        //2)read file
        buffer = read_file(fd);
        close(fd);
        if (buffer == NULL)
            return(return_error());
        //3)parse map + count rows and columns.
        map = parse_map(buffer);
        free(buffer);
        if (!map)
            return (return_error());
        // ft_putcharpointer(map); //temp
        //5)flood fill.
        map = flood_fill(map);
        //6)output map.
        ft_putcharpointer(map);
        //7)clean up
        ft_free(map);
    }
    else
        return(return_error());
    return (0);
}