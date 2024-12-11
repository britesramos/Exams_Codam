/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   anagram.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sramos <sramos@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/15 15:51:59 by sramos        #+#    #+#                 */
/*   Updated: 2024/03/15 16:27:26 by sramos        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_isanagram(char *a, char *b)
{
	int	tab[127] = {0};
	int i;

	i = 0;
	while (a[i] != '\0')
	{
		tab[(int)a[i]]++;
		i++;
	}
	i = 0;
	while (b[i] != '\0')
	{
		tab[(int)b[i]]--;
		i++;
	}
	i = 0;
	while (i < 127)
	{
		// printf("TAB[%i]: %i\n", i, tab[i]);
		if (tab[i] != 0)
		{
			printf("They are not anagrams!\n");
			return (1) ;
		}
		i++;
	}
	printf("They are anagrams!\n");
	return (0);
}

int	main(int argc, char *argv[])
{
	if (argc < 3 || argc > 3)
		return (1);
	if (ft_isanagram(argv[1], argv[2]) != 0)
		return(1);
	return (0);
}
