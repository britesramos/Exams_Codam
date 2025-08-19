#include <stdio.h>

int ft_error_map()
{
    fprintf(stdout, "Error map\n");
    return (1);
}

int main(int argc, char **argv)
{
    for(int i = 0; i < argc; ++i)
    {
        FILE* fptr;

        fptr = fopen(argv[1], "r");
        if (!fptr)
            return (ft_error_map());
    }
    return (0);
}