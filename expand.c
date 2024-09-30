/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sramos <sramos@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 17:47:57 by sramos        #+#    #+#                 */
/*   Updated: 2024/09/30 18:48:29 by sramos        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaleman <jaleman@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/16 00:06:09 by jaleman           #+#    #+#             */
/*   Updated: 2016/11/16 00:06:09 by jaleman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


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
			// printf("Array[%i][%i]: %s\n", word, letter, array[word]);
		}
		
		array[word][letter] = '\0';
		while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
			i++;
		word++;
	}
	array[word] = NULL;
	return(array);
}


int		main(int argc, char *argv[])
{
	int		i;
	int		words = 0;
	char **array;

	if (argc == 2)
	{
		array = ft_split(argv[1]);
		while(array[words])
			words++;
		printf("%i\n", words);
		i = 0;
		while (argv[1][i] == ' ' || argv[1][i] == '\t')
			i += 1;
		while (argv[1][i] || words >= 0)
		{
			if (!(argv[1][i] == ' ' || argv[1][i] == '\t'))
			{
				while (!(argv[1][i] == ' ' || argv[1][i] == '\t'))
				{
					write(1, &argv[1][i], 1);
					i++;
				}
				words--;
				write(1, "   ", 3);	
			}
			i += 1;
		}
	}
	write(1, "\n", 1);
	return (0);
}

// int	main(int argc, char *argv[])
// {
// 	int i = 0;
// 	int	words = 0;

// 	if (argc == 2)
// 	{
// 		while(argv[1][i] != '\0')
// 		{
// 			if (argv[1][i] == ' ')
// 				i++;
// 			else
// 			{
// 				while(argv[1][i] != ' ')
// 					i++;
// 				words++;
// 			}
// 		}
// 		i = 0;
		
// 		if (argv[1][i] == ' ')
// 		{	
			
// 			while(argv[1][i] == ' ')
// 				i++;
// 		}
		
// 		while(argv[1][i] != '\0')
// 		{
// 			while(argv[1][i] != ' ')
// 			{
// 				write(1, &argv[1][i], 1);
// 				i++;
// 			}
// 			words--;
// 			if (words == 0)
// 				break;
// 			write(1, "   ", 3);
// 			if (argv[1][i] == ' ')
// 			{
// 				while(argv[1][i] == ' ')
// 					i++;
// 			}
// 		}
// 	}
// 	return (0);
// }
