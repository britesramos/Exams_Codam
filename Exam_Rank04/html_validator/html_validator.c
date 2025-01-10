/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   html_validator.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: sramos <sramos@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/17 15:47:08 by sramos        #+#    #+#                 */
/*   Updated: 2025/01/10 12:30:26 by sramos        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void	ft_free(char **array)
{
	int	a = 0;

	if (array[a])
	{
		while(array[a])
		{
			free(array[a]);
			array[a] = NULL;
			a++;
		}
	}
	if (array)
		free(array);
}

char	**ft_calloc(size_t nmemb, size_t size)
{
	size_t	i = 0;
	char	**array;

	array = malloc(nmemb * size);
	while(i <= size)
	{
		array[i] = NULL;
		i++;
	}
	return (array);
}

int	alnumdigit(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return (0);
	return (1);
}

int	main(int argc, char *argv[])
{
	int	i = 0;
	int	a = 0;
	char	**array = NULL;
	int	j = 0;
	if (argc != 2)
		return (1);
	else if (argc == 2)
	{
		array = ft_calloc(sizeof(char *), 1024); //alloc for array (doesnt matter the size)
		while (argv[1][i])
		{
			if (argv[1][i] == '<')
			{
				i++;
				if (argv[1][i] == '/')
				{
					i++;
					while (alnumdigit(argv[1][i + j]) == 0 || argv[1][i + j] == '_')
					{
						if (!argv[1][i + j])
						{
							ft_free(array);
							return (1);
						}
						j++;
					}
					if (strncmp(&argv[1][i], array[a], j) == 0) // if closing tag matches last entry in the array, null it and a--.
					{
						array[a] = NULL;
						a--;
					}
					else //If there is no match, return 1 (ERROR).
					{
						ft_free(array); 
						return (1);
					}
					i = i + j;
				}
				else if (argv[1][i]) //If it is opening tag.
				{
					if (strncmp(&argv[1][i], "img", 3) == 0) //If it is "img" just skip it.
						i = i + 3;
					else
					{
						if (array[a] != NULL) //If the array has already something move to the next to add the new entry.
							a++;
						while (alnumdigit(argv[1][i]) == 0 || argv[1][i] == '_') //Add to array
						{
							if (!argv[1][i])
							{
								ft_free(array);
								return (1);
							}
							array[a] = malloc(sizeof(char) * 1024);
							i++;
							j++;
						}
						strncpy(array[a], &argv[1][i - j], j); //Copy to array.
					}
				}
			}
			j = 0;
			i++;
		}
		a = 0;
		while(array[a])
			a++;
		if (a != 0)
		{
			ft_free(array);
			return (1);
		}
		ft_free(array);
	}
	return (0);
}