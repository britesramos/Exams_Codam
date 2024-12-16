/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   html_tags2.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sramos <sramos@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/16 11:34:39 by sramos        #+#    #+#                 */
/*   Updated: 2024/12/16 15:56:11 by sramos        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include <string.h>

int	ft_strlen(char *str)
{
	int	len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*add_to_array(char *str, int i)
{
	int j = 0;
	char	array[1024];

	if (str[i] == '<' && strncmp(&str[i], "img", 3) != 0)
	{
		i++;
		while (str[i] != '>')
		{
			array[j] = str[i];
			j++;
			i++;
		}
	}
	return (array);
}

int	main(int argc, char *argv[])
{
	int	i = 0;
	int j = 0;
	char	**array;
	char	*tmp = NULL;
	int	a = 0;
	if (argc == 2)
	{
		array = malloc(sizeof(char) * 1024);
		if (!array)
			return (-1);
		
		while (argv[1][i])
		{
			//if found closing tag
				//compare to last entrace in array.
			if (argv[1][i] == '<' && argv[1][i + 1] == '/')
			{
				i = i + 2;
				while (argv[1][i] != '>')
				{
					tmp[j] = argv[1][i + j];
					j++;
				}
				//if is a match
					//remove from array.
				if (strncmp(array[a], tmp, ft_strlen(array[a])) == 0)
				{
					array[a] = NULL;
					a--;
				}
				//else
					//clean up.
					//return (1)
				else
					return (1);
			}
			//if found_open_tag
				//Add it to the array.
			else if (argv[1][i] == '<' && argv[1][i + 1])
			{
				array[a] = add_to_array(argv[1], i);
				while (argv[1][i] != '>')
					i++;
			}
			i++;
		}
		a = 0;
		while (array[a])
			a++;
		if (a != 0)
			return (1);
		//if array not empty.
			//Clean up.
			//return (1)
	}
	else
		return (1);
	return (0);
}