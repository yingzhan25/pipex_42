/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:41:12 by yingzhan          #+#    #+#             */
/*   Updated: 2025/08/07 16:41:02 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./libft/includes/libft.h"
# include "./libft/includes/ft_printf.h"
# include "./libft/includes/get_next_line.h"
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	int		in_fd;
	int		out_fd;
	int		pid[2];
	int		pfd[2];
	char	**cmd[2];
	char	*path[2];
}	t_pipex;

char	*path_join(char *dir, char *cmd);
void	clean_array(char **arr);
char	*check_access(char *cmd);
void	error_exit(char *s);
char	*return_path(char *path);
void	parse_args(t_pipex *p, int i, char *av);

#endif