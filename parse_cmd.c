/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 09:04:55 by donheo            #+#    #+#             */
/*   Updated: 2025/05/27 14:06:09 by donheo           ###   ########.fr       */
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

void	cleanup_quotes_and_restore_space(char **argv)
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

void	replace_space_within_quotes(char *raw_cmd)
{
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	while (*raw_cmd)
	{
		if (*raw_cmd == '\'' && !in_double)
			in_single = !in_single;
		else if (*raw_cmd == '"' && !in_single)
			in_double = !in_double;
		else if (*raw_cmd == ' ' && (in_single || in_double))
			*raw_cmd = 127;
		raw_cmd++;
	}
}
