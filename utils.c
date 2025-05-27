/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:19:35 by donheo            #+#    #+#             */
/*   Updated: 2025/05/27 11:12:26 by donheo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

	replace_space_within_quotes(raw_cmd);
	argv = ft_split(raw_cmd, ' ');
	if (!argv)
		return (ft_putstr_fd("tokenization failed"\
			, STDERR_FILENO), exit(EXIT_FAILURE));
	cleanup_quotes_and_restore(argv);
	path = parse_cmd_path(argv[0], pipex->envp);
	if (!path)
		return (ft_free_split(argv), ft_putstr_fd("command not found", \
			STDERR_FILENO), exit(EXIT_FAILURE));
	execve(path, argv, pipex->envp);
	ft_putstr_fd("execve failed", STDERR_FILENO);
	free(path);
	ft_free_split(argv);
	if (errno == EISDIR)
		exit(126);
	else if (errno == ENOENT)
		exit(127);
	else
		exit(1);
}
