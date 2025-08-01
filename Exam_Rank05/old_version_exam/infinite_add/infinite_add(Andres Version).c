#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int ft_strlen(char *str)
{
  int i = 0;
  while(str[i])
  {
    i++;
  }
  return i;
}
int cmp_abs(char *num1, int len1, char *num2, int len2)
{
//Compare lenghts:
  if (len1 > len2)
    return 1;
  if (len1 < len2)
    return 2;
//If lenghts are the same, compare from left to right.
  for(int i = 0; i < len1; i++)
  {
    if (num1[i] != num2[i])
      return (num1[i] > num2[i] ? 1 : 2); //If num1[i] id bigger than num2[i] return 1 else return 2.
  }
  return 0;
}
void add_nums(char *num1, int len1, char*num2, int len2)
{
  int maxl = len1 + len2;
  int carry = 0;
  char *result = malloc((maxl + 1) * sizeof(char));
  if (!result)
    return;
  result[maxl] = '\0';
  int i = 0;
  while(i < maxl)
  {
    int d1 = i < len1 ? num1[len1 - 1 - i] - '0' : 0;
    int d2 = i < len2 ? num2[len2 - 1 - i] - '0' : 0;
    int sum = d1 + d2 + carry;
    result[maxl - 1 - i] = sum % 10 + '0';
    carry = sum / 10;
    i++;
  }
  i = 0;
  while (result[i] == '0' && i < maxl - 1){
    i++;
  }
  write(1, &result[i], ft_strlen(&result[i]));
  write(1, "\n", 1);
  free(result);
}
void substract_nums(char *num1, int len1, char*num2, int len2)
{
  int maxl = len1 + len2;
  int borrow = 0;
  char *result = malloc((maxl + 1) * sizeof(char));
  if (!result)
    return;
  result[maxl] = '\0';
  int i = 0;
  while(i < maxl)
  {
    int d1 = i < len1 ? num1[len1 - 1 - i] - '0' : 0;
    int d2 = i < len2 ? num2[len2 - 1 - i] - '0' : 0;
    int subs = d1 - d2 - borrow;
    if (subs < 0)
    {
      subs += 10;
      borrow = 1;
    } else
      borrow = 0;
    result[maxl - 1 - i] = subs + '0';
    i++;
  }
  i = 0;
  while(result[i] == '0' && i < maxl - 1)
  {
    i++;
  }
  write(1, &result[i], ft_strlen(&result[i]));
  write(1, "\n", 1);
  free(result);
}
int main(int argc, char *argv[])
{
  if (argc != 3)
    return 1;
  char *num1 = argv[1];
  char *num2 = argv[2];
  //Check sign.
  int neg1 = (num1[0] == '-');
  int neg2 = (num2[0] == '-');
  if (neg1)
    num1++;
  if (neg2)
    num2++;
  int len1 = ft_strlen(num1);
  int len2 = ft_strlen(num2);
  //If signs are equal just add values.
  if (neg1 == neg2)
  {
    if (neg1)
      write(1, "-", 1);
    add_nums(num1, len1, num2, len2);
  }
  //If they are different:
  else
  {
	//Compare them. Equal values and different signs means the result is 0.
    int cmp_result = cmp_abs(num1, len1, num2, len2);
    if (cmp_result == 0)
      write(1, "0\n", 2);
	//If num1 is bigger than num2.
    else if (cmp_result == 1)
    {
      if (neg1) //If neg1 is negative:
        write(1, "-", 1);
      substract_nums(num1, len1, num2, len2);
    }
	//If num2 is bigger
    else 
    {
      if (neg2) //If neg2 is negative:
        write(1, "-", 1);
      substract_nums(num2, len2, num1, len1);
    }
  }
  return 0;
}