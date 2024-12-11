/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   html_tags.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sramos <sramos@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/12/11 11:01:39 by sramos        #+#    #+#                 */
/*   Updated: 2024/12/11 11:19:13 by sramos        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	count_tags(char *str)
{
	int	i = 0;
	int	tag_count = 0;

	while (str[i])
	{
		if (str[i] == '<' && str[i + 1])
		{
			while (str[i])
			{
				if (str[i] == '>')
					tag_count++;
				i++;
			}
		}
		i++;
	}
	return (tag_count);
}

int	check_invalid_input(char *str)
{
	int	i = 0;
	int	close = 0;
	int	open = 0;
	while (str[i])
	{
		if (str[i] == '>' && open == 0)
			return (1);
		if (str[i] == '<' && close == 1)
			return (1);
		if (str[i] == '<' && close == 0)
			open = 1;
		if (str[i] == '>' && open == 1)
			close = 1;
		if (open == 1 && close == 1)
		{
			open = 0;
			close = 0;
		}
		i++;
	}
	if (open == 1 || close == 1)
		return (1);
	return (0);
}

int	ft_isalnum(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_');
}

char	**save_tags(char *str, char **array)
{
	int	i = 0;
	int	j = 0;
	int	a = 0;
	while (str[i])
	{
		if (str[i] == '<' && str[i + 1])
		{
			i++;
			if (strncmp(&str[i], "img", 3) == 0)
				i = i + 3;
			else
			{
				while (str[i + j] != '>' && (ft_isalnum(str[i + j]) || str[i + j] == '/'))
					j++;
				array[a] = malloc(sizeof(char) * (j + 1));
				strncpy (array[a], &str[i], j);
				a++;
				i = i + j;
				if (str[i] != '>')
				{
					while (str[i] != '>')
						i++;
				}
				j = 0;
			}
		}
		i++;
	}
	return (array);
}

int	ft_strlen(char *str)
{
	int	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int	open_close_exist (char **array)
{
	int	array_count = 0;
	int	start = 0;
	int	end = 0;
	
	while (array[array_count])
		array_count++;
	array_count--;
	if (array_count % 2 != 0)
		return (1);
	start = (array_count / 2) - 1;
	end = array_count / 2;
	while (array[end] && (start >= 0 || end <= array_count))
	{
		if (strncmp(array[start], &array[end][1], ft_strlen(array[start])) != 0)
			return (1);
		start--;
		end++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	char	**array = NULL;
	int		tags_count = 0;

	if (argc != 2)
		return (1);
	if (argc == 2)
	{
		if (check_invalid_input(argv[1]) == 1)
			return (1);
		tags_count = count_tags(argv[1]);
		array = malloc (sizeof(char) * (tags_count + 1));
		array = save_tags(argv[1], array);
		if (open_close_exist(array) == 1)
			return (1);
	}
	return (0);
}