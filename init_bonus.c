/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:40:54 by yingzhan          #+#    #+#             */
/*   Updated: 2025/08/13 12:05:43 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex_bonus.h>

void	init_pipex(t_pipex *p)
{
	p->in_fd = -1;
	p->out_fd = -1;
	p->prev = -1;
	p->cmd_num = 0;
	p->pids = NULL;
	p->path = NULL;
	p->cmd = NULL;
	p->limiter = NULL;
}

static void	write_to_pipe(t_pipex *p, int *pfd)
{
	char	*line;
	size_t	lim_len;

	lim_len = ft_strlen(p->limiter);
	ft_putstr_fd("heredoc> ", STDOUT_FILENO);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (!ft_strncmp(line, p->limiter, lim_len) && \
		(line[lim_len] == '\n' || line[lim_len] == '\0'))
		{
			free(line);
			break ;
		}
		if (write(pfd[1], line, ft_strlen(line)) == -1)
		{
			free(line);
			error_exit("Write");
		}
		free(line);
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
	}
}

static void	handle_heredoc(t_pipex *p)
{
	int			pfd[2];

	if (!p->limiter)
	{
		ft_putstr_fd("Null Limiter", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (pipe(pfd) == -1)
		error_exit("Pipe");
	p->in_fd = pfd[0];
	write_to_pipe(p, pfd);
	close(pfd[1]);
}

static void	alloc_pids(t_pipex *p)
{
	p->pids = malloc(sizeof(int) * p->cmd_num);
	if (!p->pids)
	{
		ft_putstr_fd("Failed memory allocation", STDERR_FILENO);
		free_pipex(p);
		exit(EXIT_FAILURE);
	}
}

void	handle_fdpid(t_pipex *p, int ac, char **av)
{
	alloc_pids(p);
	if (p->heredoc)
	{
		handle_heredoc(p);
		p->out_fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
	{
		p->in_fd = open(av[1], O_RDONLY);
		if (p->in_fd == -1)
		{
			free_pipex(p);
			error_exit("Open infile");
		}
		p->out_fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	if (p->out_fd == -1)
	{
		free_pipex(p);
		error_exit("Open outfile");
	}
}
