/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 09:22:42 by donheo            #+#    #+#             */
/*   Updated: 2025/05/27 14:17:16 by donheo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*get_path_value(char **envp)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}

static char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
	size_t	len1;
	size_t	len2;
	size_t	len3;
	size_t	total_len;
	char	*result;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	total_len = len1 + len2 + len3;
	result = malloc(total_len + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2);
	ft_memcpy(result + len1 + len2, s3, len3);
	result[total_len] = '\0';
	return (result);
}

static char	*check_possible_candidates(char **dirs, const char *cmd)
{
	char	*candidate;
	int		i;

	i = 0;
	while (dirs[i])
	{
		candidate = ft_strjoin_three(dirs[i], "/", cmd);
		if (!candidate)
		{
			ft_free_split(dirs);
			ft_putstr_fd("memory allocation failed while building command path\n"\
				, STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		if (access(candidate, X_OK) == 0)
			return (candidate);
		free(candidate);
		i++;
	}
	return (NULL);
}

static char	*get_path_from_envp(const char **cmd, char **envp)
{
	char	*path;
	char	*path_env;
	char	**dirs;

	path_env = get_path_value(envp);
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
	{
		ft_putstr_fd("memory allocation failed while splitting PATH\n", \
			STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	path = check_possible_candidates(dirs, *cmd);
	ft_free_split(dirs);
	return (path);
}

char	*parse_cmd_path(const char **cmd, char **envp)
{
	char	*path;

	if (ft_strchr(*cmd, '/'))
	{
		if (access(*cmd, X_OK) == 0)
		{
			path = ft_strdup(*cmd);
			if (!path)
				return (ft_putstr_fd("failed memory allocation for path\n", \
					STDERR_FILENO), exit(EXIT_FAILURE), NULL);
			return (path);
		}
		else
			return (NULL);
	}
	path = get_path_from_envp(cmd, envp);
	return (path);
}
