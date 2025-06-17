/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 10:54:36 by donheo            #+#    #+#             */
/*   Updated: 2025/03/22 22:51:09 by donheo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*save_char;

	save_char = NULL;
	while (*s)
	{
		if (*s == (char)c)
			save_char = (char *)s;
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (save_char);
}
