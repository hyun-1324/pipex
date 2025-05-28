/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 09:04:55 by donheo            #+#    #+#             */
/*   Updated: 2025/05/28 07:28:31 by donheo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	remove_outer_quotes(char *str)
{
	size_t	len;

	if (!str)
		return ;
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'') \
	|| (str[0] == '"' && str[len - 1] == '"')))
	{
		ft_memmove(str, str + 1, len - 2);
		str[len - 2] = '\0';
	}
}

static void	replace_del_with_space(char *str)
{
	while (*str)
	{
		if ((unsigned char)*str == 127)
			*str = ' ';
		str++;
	}
}

static void	cleanup_quotes_and_restore_space(char **argv)
{
	size_t	i;

	i = 0;
	while (argv[i])
	{
		remove_outer_quotes(argv[i]);
		replace_del_with_space(argv[i]);
		i++;
	}
}

static void	replace_space_with_del(char *raw_cmd, int i)
{
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	while (raw_cmd[i])
	{
		if (raw_cmd[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (raw_cmd[i] == '"' && !in_single)
			in_double = !in_double;
		else if (raw_cmd[i] == '\\' && raw_cmd[i + 1] && raw_cmd[i + 1] == ' ')
		{
			raw_cmd[i] = 127;
			if (raw_cmd[i + 2])
				ft_memmove(&raw_cmd[i + 1], &raw_cmd[i + 2], \
					ft_strlen(&raw_cmd[i + 2]) + 1);
			else
				raw_cmd[i + 1] = '\0';
		}
		else if (raw_cmd[i] == ' ' && (in_single || in_double))
			raw_cmd[i] = 127;
		i++;
	}
}

char	**parse_cmd(char *raw_cmd)
{
	char	**argv;

	if (*raw_cmd == '\0')
		return (ft_putstr_fd("command not found\n"\
			, STDERR_FILENO), exit(126), NULL);
	replace_space_with_del(raw_cmd, 0);
	argv = ft_split(raw_cmd, ' ');
	if (!argv)
		return (ft_putstr_fd("tokenization failed\n"\
			, STDERR_FILENO), exit(EXIT_FAILURE), NULL);
	cleanup_quotes_and_restore_space(argv);
	return (argv);
}
