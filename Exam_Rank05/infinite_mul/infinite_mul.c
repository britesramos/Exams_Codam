#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void ft_putstr(char *str)
{
	int i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
}

int ft_strlen(char *str)
{
	int i = 0;
	while(str[i])
		i++;
	return (i);
}

char* infinite_mul(char *s1, char *s2)
{
	char* temp = NULL;
	char* res = NULL;
	int s1_len = ft_strlen(s1);
	int s2_len = ft_strlen(s2);
	int i = s1_len;
	int j = s2_len;
	int carry = 0;

	temp = malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!temp)
	{
		return (1);
	}
	//Fill temp with zeros. And Null terminate.
	for (int i = 0; i < s1_len + s2_len; i++)
		temp[i] = '0';
	temp[s1_len + s2_len] = '\0';
	//1)Multiplication
	while ((s1_len + s2_len) - i > 0)
	{
		carry = 0;
		while ((s1_len + s2_len) - j > 0)
		{
			temp[s1_len + s2_len - j] = ((s1[s1_len - j] - '0') * (s2[s2_len - i] - '0')) + carry + temp;
			j++;
		}
		i++;
	}

	//2)Remove zeros.
	//3)Copy string to result.


	return (res);
}

int main (int argc, char **argv)
{
	char *s1 = argv[1];
	char *s2 = argv[2];

	if (argc == 3){
		ft_putstr(s1);
		ft_putstr("\n");
		ft_putstr(s2);
		//1)Deal with "-" sign.
		int neg1 = 0;
		int neg2 = 0;
		if (argv[1][0] == '-')
			neg1 = 1;
		if (argv[2][0] == '-')
			neg2 = 1;
		int negres = neg1 + neg2;
		char* mulresult = infinte_mul(s1, s2);

		if (negres == 1)
			printf("-");
		ft_putstr(mulresult);


	}
	else
		return 1;
		// printf("Wrong input!\n");

	return 0;
}