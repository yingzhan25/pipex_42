/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yingzhan <yingzhan@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:41:12 by yingzhan          #+#    #+#             */
/*   Updated: 2025/08/11 16:09:52 by yingzhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <libft/includes/libft.h>
# include <libft/includes/ft_printf.h>
# include <libft/includes/get_next_line.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>


typedef struct s_pipex
{
	int		in_fd;
	int		out_fd;
	int 	prev;
	int		cmd_num;
	int		*pids;
	char	**path;
	char	***cmd;
}	t_pipex;

char	*path_join(char *dir, char *cmd);
void	clean_array(char **arr);
char	*check_access(char *cmd);
void	error_exit(char *s);
void	free_pipex(t_pipex *p);
char	**parse_cmd(char *str, t_pipex *p);
char	*parse_path(char *cmd, char *path_env);

#endif