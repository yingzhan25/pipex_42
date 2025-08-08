/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:36:16 by yingzhan          #+#    #+#             */
/*   Updated: 2025/08/08 12:15:21 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex_bonus.h>


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

void	free_pipex(t_pipex *p)
{
	int	i;
	int	j;

	if (p->pids)
		free(p->pids);
	if (p->cmd)
	{
		i = 0;
		while (i < p->cmd_num)
		{
			j = 0;
			while (j < p->cmd_num)
			{
				free(p->cmd[i][j]);
				j++;
			}
			free(p->cmd[i]);
			i++;
		}
		free(p->cmd);
	}
	if (p->path)
	{
		i = 0;
		while (i < p->cmd_num)
		{
			free(p->path[i]);
			i++;
		}
		free(p->path);
	}
	if (p->in_fd > 0)
		close(p->in_fd);
	if (p->out_fd > 0)
		close(p->out_fd);
}