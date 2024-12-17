/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   union.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sramos <sramos@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/02 17:00:01 by sramos        #+#    #+#                 */
/*   Updated: 2024/04/02 17:07:24 by sramos        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	main(int argc, char *argv[])
{
	int	tab[126] = {0}; //This was my error! I needed to initialized it.
	int	i;

	i = 0;
	if (argc != 3)
	{
		write(1, "\n", 1);
		return (0);
	}
	while (argv[1][i] != '\0')
	{
		if (tab[(int)argv[1][i]] == 0)
		{
			write(1, &argv[1][i], 1);
			tab[(int)argv[1][i]]++;
		}
		i++;
	}
	i = 0;
	while (argv[2][i] != '\0')
	{
		if (tab[(int)argv[2][i]] == 0)
		{
			write(1, &argv[2][i], 1);
			tab[(int)argv[2][i]]++;
		}
		i++;
	}
	write(1, "\n", 1);
	return (0);
}