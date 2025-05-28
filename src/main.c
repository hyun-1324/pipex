/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:00:42 by donheo            #+#    #+#             */
/*   Updated: 2025/05/28 07:59:12 by donheo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	init_pipex(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->argc = argc;
	pipex->argv = argv;
	pipex->envp = envp;
	pipex->total_forks = 0;
	pipex->exit_status = 0;
}

static void	run_first_child(t_pipex *pipex)
{
	pipex->first_pid = fork();
	if (pipex->first_pid < 0)
	{
		ft_putstr_fd("first fork failed\n", STDERR_FILENO);
		close(pipex->pipe[0]);
		close(pipex->pipe[1]);
		exit(EXIT_FAILURE);
	}
	else if (pipex->first_pid == 0)
	{
		close(pipex->pipe[0]);
		pipex->infile_fd = open(pipex->argv[INFILE], O_RDONLY);
		if (pipex->infile_fd < 0)
		{
			perror("pipex");
			close(pipex->pipe[1]);
			exit(errno);
		}
		redirect_io(pipex->infile_fd, pipex->pipe[1]);
		execute_cmd(pipex, pipex->argv[CMD1]);
	}
	pipex->total_forks++;
	close(pipex->pipe[1]);
}

static void	run_second_child(t_pipex *pipex)
{
	pipex->second_pid = fork();
	if (pipex->second_pid < 0)
	{
		ft_putstr_fd("second fork failed\n", STDERR_FILENO);
		close(pipex->pipe[0]);
		close(pipex->pipe[1]);
		exit(EXIT_FAILURE);
	}
	if (pipex->second_pid == 0)
	{
		close(pipex->pipe[1]);
		pipex->outfile_fd = open(pipex->argv[OUTFILE], \
			O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (pipex->outfile_fd < 0)
		{
			perror("pipex");
			close(pipex->pipe[0]);
			exit(EXIT_FAILURE);
		}
		redirect_io(pipex->pipe[0], pipex->outfile_fd);
		execute_cmd(pipex, pipex->argv[CMD2]);
	}
	pipex->total_forks++;
	close(pipex->pipe[0]);
}

static void	wait_children(t_pipex *pipex)
{
	pid_t	wpid;
	int		status;

	while (pipex->total_forks--)
	{
		wpid = waitpid(-1, &status, 0);
		if (WIFEXITED(status))
		{
			if (wpid == pipex->second_pid)
				pipex->exit_status = WEXITSTATUS(status);
		}
		else if (WIFSIGNALED(status))
		{
			if (wpid == pipex->second_pid)
				pipex->exit_status = 128 + WTERMSIG(status);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
	{
		ft_putstr_fd("invalid argument number\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	validate_quotes(argv[CMD1]);
	validate_quotes(argv[CMD2]);
	init_pipex(&pipex, argc, argv, envp);
	if (pipe(pipex.pipe) < 0)
	{
		ft_putstr_fd("failed to pipe\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	run_first_child(&pipex);
	run_second_child(&pipex);
	wait_children(&pipex);
	return (pipex.exit_status);
}
