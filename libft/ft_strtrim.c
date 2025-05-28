/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:28:01 by donheo            #+#    #+#             */
/*   Updated: 2025/04/16 16:57:03 by donheo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	find_start(char const *s1, char const *set)
{
	int	s1_i;
	int	set_i;

	s1_i = 0;
	set_i = 0;
	while (set[set_i])
	{
		if (s1[s1_i] == set[set_i])
		{
			s1_i++;
			set_i = 0;
		}
		else
			set_i++;
	}
	return (s1_i);
}

static int	find_end(char const *s1, char const *set)
{
	int	s1_i;
	int	set_i;

	s1_i = ft_strlen(s1) - 1;
	set_i = 0;
	while (set[set_i])
	{
		if (s1[s1_i] == set[set_i] && s1_i != 0)
		{
			s1_i--;
			set_i = 0;
		}
		else
			set_i++;
	}
	return (s1_i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start_i;
	int		end_i;
	int		nstr_i;
	char	*trimstr;

	start_i = find_start(s1, set);
	end_i = find_end(s1, set);
	if (end_i < start_i)
		return (ft_strdup(""));
	trimstr = malloc((end_i - start_i + 2) * sizeof(char));
	if (!trimstr)
		return (NULL);
	nstr_i = 0;
	while (end_i + 1 > start_i)
	{
		trimstr[nstr_i] = s1[start_i];
		start_i++;
		nstr_i++;
	}
	trimstr[nstr_i] = '\0';
	return (trimstr);
}
