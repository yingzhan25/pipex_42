/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:32:56 by yingzhan          #+#    #+#             */
/*   Updated: 2025/08/07 15:23:55 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*path_join(char *dir, char *cmd)
{
	char	*path;
	size_t	len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	path = (char *) malloc(len);
	if (!path)
		return (NULL);
	ft_strlcpy(path, dir, len);
	ft_strlcat(path, "/", len);
	ft_strlcat(path, cmd, len);
	return (path);
}

void	clean_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free (arr[i]);
		i++;
	}
	free (arr);
}

char	*check_access(char *cmd)
{
	if (!access(cmd, X_OK))
		return (cmd);
	return (NULL);
}

void	error_exit(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

char	*return_path(char *path)
{
	if (!path)
		return (NULL);
	return (ft_strdup(path));
}
