/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:40:50 by yingzhan          #+#    #+#             */
/*   Updated: 2025/08/06 14:12:26 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex_bonus.h>

void	pipe_init(t_pipex *p,char **av)
{
	p->in_fd = open(av[1], O_RDONLY);
	if (p->in_fd == -1)
	{
		perror("Open infile");
		exit(EXIT_FAILURE);
	}
	p->out_fd = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (p->out_fd == -1)
	{
		perror("Open outfile");
		exit(EXIT_FAILURE);
	}
}

int	args_parse(t_pipex *p, char **av, char **ev)
{
	int	i;
	int	j;

	p->cmd = malloc(sizeof(char**) * p->cmd_num);
	if (!p->cmd)
		return (1);
	i = 0;
	j = 2;
	while (i < p->cmd_num)
	{
		p->cmd[i] = parse_cmd(av[j]);
		p->path[i] = parse_path(p->cmd[i], getent("PATH"));
		i++;
		p->path[i] = parse_path(p->cmd[i], getent("PATH"));
		i++;
		j++;
	}
	p->cmd[i] = NULL;
	p->path[i] = NULL;
	return (0);
}

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

int	main(int ac, char **av, char *envp[])
{
	t_pipex	*p;
	int		pfd[2];
	int		i;
	int		status;

	if (ac != 5)
	{
		ft_putstr_fd("Input invalid", STDERR_FILENO);
		return (1);
	}
	p->cmd_num = ac - 3;
	p->pids = malloc(sizeof(int) * p->cmd_num);
	pipe_init(p, av);
	args_parse(p, av, envp);
	i = 0;
	while (i < p->cmd_num - 1)
	{
		if (i < p->cmd_num - 1)
		{
			if (pipe(pfd) == -1)
			{
				perror("Pipe");
				exit(EXIT_FAILURE);
			}
		}
		p->pids[i] = fork();
		if (p->pids[i] == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (p->pids[i] == 0)
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
			execve(p->path[i], p->cmd[i], envp);
			perror("execve cmd");
			exit(EXIT_FAILURE);
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
		i++;
	}
	i = 0;
	while (i < p->cmd_num)
	{
		waitpid(p->pids[i], &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
				exit(WEXITSTATUS(status));
		i++;
	}
}