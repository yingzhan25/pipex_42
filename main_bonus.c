/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:40:50 by yingzhan          #+#    #+#             */
/*   Updated: 2025/08/11 16:10:35 by yingzhan         ###   ########.fr       */
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
}

void	args_parse(t_pipex *p, int ac, char **av)
{
	int	i;
	int	j;

	p->cmd_num = ac - 3;
	p->cmd = ft_calloc(p->cmd_num + 1, sizeof(char**));
	if (!p->cmd)
	{
		ft_putstr_fd("Failed memory allocation", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	p->path = ft_calloc(p->cmd_num + 1, sizeof(char*));
	if (!p->path)
	{
		free_pipex(p);
		ft_putstr_fd("Failed memory allocation", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	i = 0;
	j = 2;
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

void	check_params(t_pipex *p, int ac, char **av)
{
	p->pids = malloc(sizeof(int) * p->cmd_num);
	if (!p->pids)
	{
		ft_putstr_fd("Failed memory allocation", STDERR_FILENO);
		free_pipex(p);
		exit(EXIT_FAILURE);
	}
	p->in_fd = open(av[1], O_RDONLY);
	if (p->in_fd == -1)
	{
		free_pipex(p);
		error_exit("Open infile");
	}
	p->out_fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (p->out_fd == -1)
	{
		free_pipex(p);
		error_exit("Open outfile");
	}
}

void	child_process(int *pfd, int i, t_pipex *p, char **ep)
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
	execve(p->path[i], p->cmd[i], ep);
	error_exit("execve cmd");
}

void	cmd_exec(t_pipex *p, int i, char **ep)
{
	int		pfd[2];

	if (i < p->cmd_num - 1)
	{
		if (pipe(pfd) == -1)
			error_exit("Pipe");
	}
	p->pids[i] = fork();
	if (p->pids[i] == -1)
		error_exit("fork");
	else if (p->pids[i] == 0)
		child_process(pfd, i, p, ep);
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

int	main(int ac, char **av, char *envp[])
{
	t_pipex	p;
	int		i;
	int		status;

	if (ac < 5)
	{
		ft_putstr_fd("Input invalid", STDERR_FILENO);
		return (1);
	}
	init_pipex(&p);
	args_parse(&p, ac, av);
	check_params(&p, ac, av);
	i = -1;
	while (++i < p.cmd_num)
		cmd_exec(&p, i, envp);
	i = -1;
	while (++i < p.cmd_num)
	{
		waitpid(p.pids[i], &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			free_pipex(&p);
			exit(WEXITSTATUS(status));
		}
	}
	free_pipex(&p);
	return (0);
}