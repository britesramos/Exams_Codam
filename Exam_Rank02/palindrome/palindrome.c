#include <unistd.h>
#include <stdio.h>
int	ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}
int	is_pal(const char *str, int start, int len)
{
	int	i = 0;
	while (i < (len/2))
	{
		if (str[start + i] != str[start + len - 1 - i])
			return (0);
		i++;
	}
	return (1);
}
int	main(int argc, char **argv)
{
	char	*str = argv[1];
	int		len = ft_strlen(str);
	printf("STR LENGHT: %i\n", len);
	int		i = 0;
	int		start;
	if (argc == 2)
	{
		while (i <= len)
		{
			start = i;
			while (start >= 0)
			{
				if (is_pal(str, start, len - i) == 1)
				{
					write(1, &str[start], len - i);
					write(1, "\n", 1);
					return (0);
				}
				start--;
			}
			i++;
		}
	}
	write(1, "\n", 1);
	return (0);
}