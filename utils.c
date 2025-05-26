/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:19:35 by donheo            #+#    #+#             */
/*   Updated: 2025/05/26 14:51:20 by donheo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	redirect(int in, int out)
{
	if (dup2(in, STDIN_FILENO) < 0)
	{
		close(in);
		close(out);
		ft_putstr_fd("failed to dup2 (stdin)", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (dup2(out, STDOUT_FILENO) < 0)
	{
		close(in);
		close(out);
		ft_putstr_fd("failed to dup2 (stdout)", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	close(in);
	close(out);
}

void	replace_space_within_quotes(char *cmd)
{
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	while (*cmd)
	{
		if (*cmd == '\'' && !in_double)
			in_single = !in_single;
		else if (*cmd == '"' && !in_single)
			in_double = !in_double;
		else if (*cmd == ' ' && (in_single || in_double))
			*cmd = 127;
	}
}

int	execute_cmd(t_pipex *pipex, char *cmd)
{
	char	**splited_cmd;

	replace_space_within_quotes(cmd);
	splited_cmd = ft_split(cmd, ' ');
	if (!splited_cmd)
	{
		ft_putstr_fd("failed memory allocation for splited_cmd", STDERR_FILENO);
		return (0);
	}


	return (1);
}
