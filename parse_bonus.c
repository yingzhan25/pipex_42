/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:52:44 by yingzhan          #+#    #+#             */
/*   Updated: 2025/08/08 12:16:41 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	**parse_cmd(char *str)
{
	char	**arr;
	const char	*s;

	s = str;
	arr = ft_split(s, ' ');
	if (!arr)
	{
		ft_putstr_fd("Failed memory allocation", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	return (arr);
}

char	*parse_path(char *cmd, char *path_env)
{
	char	**dirs;
	char	*path;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (check_access(cmd));
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		path = path_join(dirs[i], cmd);
		if (!path)
		{
			clean_array(dirs);
			return (NULL);
		}
		if (!access(path, X_OK))
		{
			clean_array(dirs);
			return (path);
		}
		free(path);
		i++;
	}
	clean_array(dirs);
	return (NULL);
}