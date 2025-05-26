/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:00:42 by donheo            #+#    #+#             */
/*   Updated: 2025/05/26 14:50:45 by donheo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
	{
		ft_putstr_fd("invalid argument number", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	validate_quotes(argv);
	init_pipex(&pipex, argc, argv, envp);
	if (pipe(pipex.pipe) < 0)
	{
		ft_putstr_fd("failed to pipe\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	process_first_cmd(&pipex);
	process_second_cmd(&pipex);
	waitpid(pipex.pid[0], NULL, 0);
	waitpid(pipex.pid[1], &pipex.wait_status, 0);
	if (WIFEXITED(pipex.wait_status))
		return (WEXITSTATUS(pipex.wait_status));
	return (EXIT_SUCCESS);
}

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
			return (ft_putstr_fd("unmatched quote \n", STDERR_FILENO), exit(1));
		in_single = 0;
		in_double = 0;
		i = 0;
		argv++;
	}
}

void	init_pipex(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->argc = argc;
	pipex->argv = argv;
	pipex->envp = envp;
}

void	process_first_cmd(t_pipex *pipex)
{
	pipex->pid[0] = fork();
	if (pipex->pid[0] < 0)
	{
		ft_putstr_fd("first fork failed", STDERR_FILENO);
		return (close(pipex->pipe[0]), close(pipex->pipe[1]), exit(EXIT_FAILURE));
	}
	else if (pipex->pid == 0)
	{
		close(pipex->pipe[0]);
		pipex->fd[0] = open(pipex->argv[1], O_RDONLY);
		if (pipex->fd[0] < 0)
		{
			ft_putstr_fd("failed to open first file", STDERR_FILENO);
			close(pipex->pipe[1]);
			exit(STDERR_FILENO);
		}
		redirect(pipex->fd[0], pipex->pipe[1]);
		if (!execute_cmd(pipex, pipex->argv[2]))
		{
			close(pipex->pipe[1]);
			exit(STDERR_FILENO);
		}
	}
	close(pipex->pipe[1]);
}

void	process_second_cmd(t_pipex *pipex)
{
	pipex->pid[1] = fork();
	if (pipex->pid[1] < 0)
	{
		ft_putstr_fd("second fork failed", STDERR_FILENO);
		return (close(pipex->pipe[0]), close(pipex->pipe[1]), exit(EXIT_FAILURE));
	}
	if (pipex->pid[1] == 0)
	{
		close(pipex->pipe[1]);
		pipex->fd[1] = open(pipex->argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0777);
		if (pipex->fd[1] < 0)
		{
			ft_putstr_fd("failed to open last file", STDERR_FILENO);
			close(pipex->pipe[0]);
			exit(STDERR_FILENO);
		}
		redirect(pipex->pipe[0], pipex->fd[1]);
		execute_cmd(pipex, pipex->argv[3]);
	}
	close(pipex->pipe[0]);
}
