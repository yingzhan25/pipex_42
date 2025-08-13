/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:31:12 by yingzhan          #+#    #+#             */
/*   Updated: 2025/08/13 11:33:36 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**parse_cmd(char *str)
{
	char		**arr;
	const char	*s;

	s = str;
	arr = ft_split(s, ' ');
	if (!arr)
	{
		ft_putstr_fd("Failed memory allocation", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (!*arr)
	{
		ft_putstr_fd("Failed parsing command", STDERR_FILENO);
		free(arr);
		exit(EXIT_FAILURE);
	}
	return (arr);
}

static char	*parse_path(char *cmd, char *path_env)
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
		if (!path || !access(path, X_OK))
		{
			clean_array(dirs);
			return (return_path(path));
		}
		free(path);
		i++;
	}
	clean_array(dirs);
	return (NULL);
}

void	parse_args(t_pipex *p, int i, char *av)
{
	char	*path_env;

	path_env = getenv("PATH");
	if (!path_env)
	{
		ft_putstr_fd("PATH not found in environment", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	p->cmd[i] = parse_cmd(av);
	p->path[i] = parse_path(p->cmd[i][0], path_env);
	if (!p->path[i])
	{
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putstr_fd(p->cmd[i][0], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		clean_array(p->cmd[i]);
		exit(EXIT_CMD_NOT_FOUND);
	}
}
