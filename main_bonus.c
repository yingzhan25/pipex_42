/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:40:50 by yingzhan          #+#    #+#             */
/*   Updated: 2025/08/12 17:23:04 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex_bonus.h>

int	invalid_input(int ac, char **av, t_pipex *p)
{
	p->heredoc = 0;
	if (ac >= 2 && !ft_strncmp(av[1], "here_doc", 8))
	{
		p->heredoc = 1;
		if (ac < 6)
		{
			ft_putstr_fd("Invalid input(here_doc)", STDERR_FILENO);
			return (1);
		}
	}
	else if (ac < 5)
	{
		ft_putstr_fd("Input invalid(normal)", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static void	redirect_pipe(int *pfd, int i, t_pipex *p)
{
	if (i < p->cmd_num - 1)
		close(pfd[0]);
	if (i == 0)
	{
		dup2(p->in_fd, STDIN_FILENO);
		close(p->in_fd);
	}
	else
	{
		dup2(p->prev, STDIN_FILENO);
		close(p->prev);
	}
	if (i == p->cmd_num - 1)
	{
		dup2(p->out_fd, STDOUT_FILENO);
		close(p->out_fd);
	}
	else
	{
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
	}
}

void	cmd_exec(t_pipex *p, int i, char **ep)
{
	int		pfd[2];

	if (i < p->cmd_num - 1 && pipe(pfd) == -1)
		error_exit("Pipe");
	p->pids[i] = fork();
	if (p->pids[i] == -1)
		error_exit("fork");
	else if (p->pids[i] == 0)
	{
		redirect_pipe(pfd, i, p);
		execve(p->path[i], p->cmd[i], ep);
		error_exit("execve cmd");
	}
	else
	{
		if (i > 0)
			close(p->prev);
		if (i < p->cmd_num - 1)
		{
			close(pfd[1]);
			p->prev = pfd[0];
		}
	}
}

void	wait_process(t_pipex *p)
{
	int	status;
	int	i;

	i = 0;
	while (i < p->cmd_num)
	{
		waitpid(p->pids[i], &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			free_pipex(p);
			exit(WEXITSTATUS(status));
		}
		i++;
	}
}

int	main(int ac, char **av, char *envp[])
{
	t_pipex	p;
	int		i;

	if (invalid_input(ac, av, &p))
		return (1);
	init_pipex(&p);
	args_parse(&p, ac, av);
	handle_fdpid(&p, ac, av);
	i = 0;
	while (i < p.cmd_num)
	{
		cmd_exec(&p, i, envp);
		i++;
	}
	wait_process(&p);
	free_pipex(&p);
	return (0);
}
