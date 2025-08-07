/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:14:59 by yingzhan          #+#    #+#             */
/*   Updated: 2025/05/17 14:30:01 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nmemb(int nb)
{
	int	count;

	count = 1;
	if (nb < 0)
	{
		count++;
	}
	while (nb <= -10 || nb >= 10)
	{
		count++;
		nb = nb / 10;
	}
	return (count);
}

static void	ft_assign(char *s, int len, int nb)
{
	unsigned int	num;

	s[len] = '\0';
	if (nb == 0)
		s[0] = '0';
	if (nb < 0)
	{
		num = (unsigned int)(-nb);
		s[0] = '-';
	}
	else
		num = (unsigned int)(nb);
	while (num > 0)
	{
		s[--len] = num % 10 + '0';
		num = num / 10;
	}
}

char	*ft_itoa(int n)
{
	int		i;
	int		c;
	char	*str;

	i = 0;
	c = ft_nmemb(n);
	str = malloc(sizeof(char) * (c + 1));
	if (!str)
		return (NULL);
	if (n == INT_MIN)
		ft_strlcpy(str, "-2147483648", 12);
	else
		ft_assign(str, c, n);
	return (str);
}
/*
#include <stdio.h>
int	main(void)
{
	char *s = ft_itoa(-2147483648);
	printf("ft: %s\n", s);
	free(s);
	return (0);
}*/