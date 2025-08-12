/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 09:52:44 by yingzhan          #+#    #+#             */
/*   Updated: 2025/08/12 18:15:50 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static char	**parse_cmd(char *str, t_pipex *p)
{
	char		**arr;
	const char	*s;

	s = str;
	arr = ft_split(s, ' ');
	if (!arr)
	{
		ft_putstr_fd("Failed memory allocation", STDERR_FILENO);
		free_pipex(p);
		exit(EXIT_FAILURE);
	}
	if (!*arr)
	{
		ft_putstr_fd("Failed parsing command", STDERR_FILENO);
		free(arr);
		free_pipex(p);
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

static void	alloc_arr(t_pipex *p)
{
	p->cmd = ft_calloc(p->cmd_num + 1, sizeof(char **));
	if (!p->cmd)
	{
		ft_putstr_fd("Failed memory allocation", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	p->path = ft_calloc(p->cmd_num + 1, sizeof(char *));
	if (!p->path)
	{
		free_pipex(p);
		ft_putstr_fd("Failed memory allocation", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

static void	fill_arr(t_pipex *p, int j, char **av)
{
	int	i;

	i = 0;
	while (i < p->cmd_num)
	{
		p->cmd[i] = parse_cmd(av[j], p);
		p->path[i] = parse_path(p->cmd[i][0], getenv("PATH"));
		if (!p->path[i])
		{
			ft_putstr_fd("Command not found: ", STDERR_FILENO);
			ft_putstr_fd(p->cmd[i][0], STDERR_FILENO);
			free_pipex(p);
			exit(EXIT_FAILURE);
		}
		i++;
		j++;
	}
}

void	args_parse(t_pipex *p, int ac, char **av)
{
	int	j;

	if (p->heredoc)
	{
		p->limiter = av[2];
		p->cmd_num = ac - 4;
		j = 3;
	}
	else
	{
		p->cmd_num = ac - 3;
		j = 2;
	}
	alloc_arr(p);
	fill_arr(p, j, av);
}
