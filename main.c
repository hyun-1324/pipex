/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:00:42 by donheo            #+#    #+#             */
/*   Updated: 2025/05/25 22:59:09 by donheo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
	{
		ft_putstr_fd("Invalid argument number", STDERR_FILENO);
		return (1);
	}
	validate_quotes(argv, 0, 0, 0);
	init_pipex(&pipex, argc, argv, envp);


}

void	validate_quotes(char **argv, int in_single, int in_double, int i)
{
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
		if (in_single)
			ft_putstr_fd("Error: unmatched single quote (')\n", STDERR_FILENO);
		else if (in_double)
			ft_putstr_fd("Error: unmatched double quote (\")\n", STDERR_FILENO);
		if (in_single || in_double)
			exit(1);
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
