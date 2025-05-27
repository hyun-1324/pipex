/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:00:42 by donheo            #+#    #+#             */
/*   Updated: 2025/05/27 13:06:03 by donheo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	validate_quotes(char **argv)
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

static void	init_pipex(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->argc = argc;
	pipex->argv = argv;
	pipex->envp = envp;
}

static void	run_first_child(t_pipex *pipex)
{
	pipex->pid[0] = fork();
	if (pipex->pid[0] < 0)
	{
		ft_putstr_fd("first fork failed\n", STDERR_FILENO);
		close(pipex->pipe[0]);
		close(pipex->pipe[1]);
		exit(EXIT_FAILURE);
	}
	else if (pipex->pid[0] == 0)
	{
		close(pipex->pipe[0]);
		pipex->infile_fd = open(pipex->argv[INFILE], O_RDONLY);
		if (pipex->infile_fd < 0)
		{
			ft_putstr_fd("failed to open first file\n", STDERR_FILENO);
			close(pipex->pipe[1]);
			exit(STDERR_FILENO);
		}
		redirect_io(pipex->infile_fd, pipex->pipe[1]);
		execute_cmd(pipex, pipex->argv[CMD1]);
	}
	close(pipex->pipe[1]);
}

static void	run_second_child(t_pipex *pipex)
{
	pipex->pid[1] = fork();
	if (pipex->pid[1] < 0)
	{
		ft_putstr_fd("second fork failed\n", STDERR_FILENO);
		close(pipex->pipe[0]);
		close(pipex->pipe[1]);
		exit(EXIT_FAILURE);
	}
	if (pipex->pid[1] == 0)
	{
		close(pipex->pipe[1]);
		pipex->outfile_fd = open(pipex->argv[OUTFILE], \
			O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (pipex->outfile_fd < 0)
		{
			ft_putstr_fd("failed to open last file\n", STDERR_FILENO);
			close(pipex->pipe[0]);
			exit(STDERR_FILENO);
		}
		redirect_io(pipex->pipe[0], pipex->outfile_fd);
		execute_cmd(pipex, pipex->argv[CMD2]);
	}
	close(pipex->pipe[0]);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
	{
		ft_putstr_fd("invalid argument number\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	validate_quotes(argv);
	init_pipex(&pipex, argc, argv, envp);
	if (pipe(pipex.pipe) < 0)
	{
		ft_putstr_fd("failed to pipe\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	run_first_child(&pipex);
	run_second_child(&pipex);
	waitpid(pipex.pid[0], NULL, 0);
	waitpid(pipex.pid[1], &pipex.wait_status, 0);
	if (WIFEXITED(pipex.wait_status))
		return (WEXITSTATUS(pipex.wait_status));
	return (EXIT_SUCCESS);
}
