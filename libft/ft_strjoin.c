/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:34:39 by yingzhan          #+#    #+#             */
/*   Updated: 2025/05/15 18:18:47 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		len1;
	int		len2;
	char	*buff;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	i = 0;
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	buff = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!buff)
		return (NULL);
	ft_strlcpy(buff, s1, len1 + 1);
	ft_strlcat(buff, s2, len1 + len2 + 1);
	return (buff);
}
/*
#include <stdio.h>

int	main(void)
{
	const char *str1 = "Hello!";
	const char *str2 = "World!";
	char *str;

	str = ft_strjoin(str1, str2);
	printf("%s\n", str);
	free(str);
	return (0);
}*/