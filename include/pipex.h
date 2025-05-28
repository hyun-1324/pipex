/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:03:28 by donheo            #+#    #+#             */
/*   Updated: 2025/05/28 06:28:20 by donheo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define INFILE 1
# define CMD1   2
# define CMD2   3
# define OUTFILE 4
# define E_CMD_NOT_FOUND 127

# include "../libft/libft.h"
# include <errno.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct s_pipex
{
	int		argc;
	char	**argv;
	char	**envp;
	int		infile_fd;
	int		outfile_fd;
	int		pipe[2];
	pid_t	first_pid;
	pid_t	second_pid;
	int		total_forks;
	int		exit_status;
}	t_pipex;

void	validate_quotes(char **argv);
int		main(int argc, char **argv, char **envp);

void	replace_space_within_quotes(char *raw);
void	cleanup_quotes_and_restore_space(char **tokens);
char	**parse_cmd(char *raw_cmd);
char	*parse_cmd_path(char **cmd, char **envp);
void	redirect_io(int in_fd, int out_fd);
void	execute_cmd(t_pipex *pipex, char *raw_cmd);

void	ft_free_split(char **arr);

#endif
