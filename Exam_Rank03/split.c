/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   split.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sramos <sramos@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 17:14:30 by sramos        #+#    #+#                 */
/*   Updated: 2024/09/30 17:27:40 by sramos        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

char **ft_split(char *str)
{
	int	i = 0;
	int word = 0;
	int letter;
	char **array;

	array = malloc(sizeof(char *) * 50);
	if(!array)
		return (NULL);
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
		i++;
	
	while (str[i])
	{
		letter = 0;
		array[word] = malloc(sizeof(char) * 10);
		if(!array[word])
			return (NULL);
		while(str[i] && str[i] != ' ' && str[i] != '\n' && str[i] != '\t')
		{
			array[word][letter] = str[i];
			letter++;
			i++;
			printf("Array[%i][%i]: %s\n", word, letter, array[word]);
		}
		
		array[word][letter] = '\0';
		while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
			i++;
		word++;
	}
	array[word] = NULL;
	return(array);
}

int	main(void)
{
	char *str = "HEELLO     muchcha djf\n";
	char **array = ft_split(str);
	int i = 0;
	while(array[i])
	{
		printf("[%i]: %s\n", i, array[i]);
		i++;
	}
	return (0);
}