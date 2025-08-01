#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

void ft_putstr(char *str)
{
	int i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
}

char* infin_mul(char *s1, char *s2)
{
	int i = 0;
	int j = 0;
	int mul = 0;
	int carry = 0;
	int start = 0;
	int s1_len = ft_strlen(s1);
	int s2_len = ft_strlen(s2);
	char *temp = NULL;
	char *res = NULL;

	temp = malloc ((s1_len + s2_len + 1) * sizeof(char)); //CHANGED
	if (!temp)
		return (NULL);
	while (i < s1_len + s2_len)
	{
		temp[i] = '0';
		i++;
	}
	temp[i] = '\0';
	i = 0;
	
	while (i < s1_len)
	{
		carry = 0;
		while (j < s2_len)
		{
			mul = (s1[s1_len - i - 1] - '0') * (s2[s2_len - j - 1] - '0') + carry + (temp[s1_len + s2_len - i - j - 1] - '0');
			carry = mul / 10;
			temp[s1_len + s2_len - j - i - 1] = mul % 10 + '0';
			j++;
		}
		temp[s1_len + s2_len - j - i - 1] = carry + '0';
		j = 0;
		i++;
	}
	while (temp[start] == '0')
		start++;
	res = malloc ((s1_len + s2_len - start + 2) * sizeof(char)); //CHANGED
	if (!res)
	{
		free(temp);
		return (NULL);
	}
	i = 0;
	while (temp[i + start])
	{
		res[i] = temp[i + start];
		i++;
	}
	free(temp); //CHANGED
	return (res);
}

int main(int argc, char **argv)
{
	char *s1 = argv[1];
	char *s2 = argv[2];
	int sign1 = 0;
	int sign2 = 0;
	int signres = 0;
	char *res;

	if (argc == 3)
	{
		//1)Check if input is zero.
		if (s1[0] == '0' || s2[0] == '0')
		{
			write(1, "0", 1);
			write(1, "\n", 1);
			return (0);
		}
		//2)Sign check. (Don't forget to move the pointer if there is a sign.)
		if (s1[0] == '-')
		{
			sign1 = 1;
			s1++; //Move the pointer - ignore the sign if it exists.
		}
		if (s2[0] == '-')
		{
			sign2 = 1;
			s2++; //Move the pointer - ignore the sign if it exists.
		}
		signres = sign1 + sign2;
		res = infin_mul(s1, s2);
		if (signres == 1) //Write the sign ig signres is one.
			write(1, "-", 1);
		ft_putstr(res);
		write(1, "\n", 1);
	}
	else
		return (1);
	return (0);
}