/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:55:48 by yingzhan          #+#    #+#             */
/*   Updated: 2025/08/12 17:04:28 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex_bonus.h>

static void	free_path(t_pipex *p)
{
	int	i;

	if (p->path)
	{
		i = 0;
		while (p->path[i] && i < p->cmd_num)
		{
			free(p->path[i]);
			i++;
		}
		free(p->path);
	}
}

void	free_pipex(t_pipex *p)
{
	int	i;

	if (p->pids)
		free(p->pids);
	if (p->cmd)
	{
		i = 0;
		while (p->cmd[i] && i < p->cmd_num)
		{
			clean_array(p->cmd[i]);
			i++;
		}
		free(p->cmd);
	}
	free_path(p);
	if (p->in_fd != -1)
		close(p->in_fd);
	if (p->out_fd != -1)
		close(p->out_fd);
	if (p->prev != -1)
		close(p->prev);
}
