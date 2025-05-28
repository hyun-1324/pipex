/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:19:35 by donheo            #+#    #+#             */
/*   Updated: 2025/05/28 07:25:03 by donheo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	validate_quotes(char **argv)
{
	int	in_single;
	int	in_double;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (*argv)
	{
		while ((*argv)[i])
		{
			if ((*argv)[i] == '\'' && !in_double)
				in_single = !in_single;
			else if ((*argv)[i] == '"' && !in_single)
				in_double = !in_double;
			i++;
		}
		if (in_single || in_double)
			return (ft_putstr_fd("unmatched quote\n", STDERR_FILENO), exit(1));
		in_single = 0;
		in_double = 0;
		i = 0;
		argv++;
	}
}

void	ft_free_split(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	redirect_io(int in_fd, int out_fd)
{
	if (dup2(in_fd, STDIN_FILENO) < 0 || dup2(out_fd, STDOUT_FILENO) < 0)
	{
		ft_putstr_fd("failed to dup2\n", STDERR_FILENO);
		close(in_fd);
		close(out_fd);
		exit(EXIT_FAILURE);
	}
	close(in_fd);
	close(out_fd);
}

void	execute_cmd(t_pipex *pipex, char *raw_cmd)
{
	char	**argv;
	char	*path;

	argv = parse_cmd(raw_cmd);
	path = parse_cmd_path(argv, pipex->envp);
	if (!path)
		return (ft_free_split(argv), ft_putstr_fd("command not found\n"\
			, STDERR_FILENO), exit(127));
	execve(path, argv, pipex->envp);
	ft_putstr_fd("execve failed\n", STDERR_FILENO);
	free(path);
	ft_free_split(argv);
	if (errno == EISDIR || errno == EACCES)
		exit(E_CMD_NOT_EXECUTABLE);
	else if (errno == ENOENT)
		exit(E_CMD_NOT_FOUND);
	else
		exit(EXIT_FAILURE);
}
