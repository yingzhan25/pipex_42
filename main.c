/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:32:19 by yingzhan          #+#    #+#             */
/*   Updated: 2025/08/07 16:33:42 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	invalid_input(int ac)
{
	if (ac != 5)
	{
		ft_putstr_fd("Input invalid", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static void	pipe_init(t_pipex *p, char **av)
{
	p->in_fd = open(av[1], O_RDONLY);
	if (p->in_fd == -1)
		error_exit("Open infile");
	p->out_fd = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (p->out_fd == -1)
		error_exit("Open outfile");
	if (pipe(p->pfd) == -1)
	{
		close(p->in_fd);
		close(p->out_fd);
		error_exit("Pipe");
	}
	p->exit_code = 0;
}

static void	cmd_exec_1(t_pipex *p, char **av, char **ep)
{
	p->pid[0] = fork();
	if (p->pid[0] == -1)
	{
		close(p->in_fd);
		close(p->out_fd);
		error_exit("fork");
	}
	else if (p->pid[0] == 0)
	{
		close(p->pfd[0]);
		close(p->out_fd);
		dup2(p->in_fd, STDIN_FILENO);
		close(p->in_fd);
		dup2(p->pfd[1], STDOUT_FILENO);
		close(p->pfd[1]);
		parse_args(p, 0, av[2]);
		execve(p->path[0], p->cmd[0], ep);
		ft_putstr_fd("pipex: execve1 failed unexpectedly", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	else
		close(p->pfd[1]);
}

static void	cmd_exec_2(t_pipex *p, char **av, char **ep)
{
	p->pid[1] = fork();
	if (p->pid[1] == -1)
	{
		close(p->in_fd);
		close(p->out_fd);
		error_exit("fork");
	}
	else if (p->pid[1] == 0)
	{
		close(p->pfd[1]);
		close(p->in_fd);
		dup2(p->pfd[0], STDIN_FILENO);
		close(p->pfd[0]);
		dup2(p->out_fd, STDOUT_FILENO);
		close(p->out_fd);
		parse_args(p, 1, av[3]);
		execve(p->path[1], p->cmd[1], ep);
		ft_putstr_fd("pipex: execve2 failed unexpectedly", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	else
		close(p->pfd[0]);
}

int	main(int ac, char **av, char *ep[])
{
	t_pipex	p;
	int		status;

	if (invalid_input(ac))
		return (1);
	pipe_init(&p, av);
	cmd_exec_1(&p, av, ep);
	waitpid(p.pid[0], &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(p.pfd[0]);
		close(p.in_fd);
		close(p.out_fd);
		return (WEXITSTATUS(status));
	}
	cmd_exec_2(&p, av, ep);
	waitpid(p.pid[1], &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		p.exit_code = WEXITSTATUS(status);
	close(p.in_fd);
	close(p.out_fd);
	return (p.exit_code);
}
