void ft_itoa(int n)
{
    if(n >= 10)
        ft_itoa(n / 10);
    ft_putchar ((n % 10) + '0');
}