/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donheo <donheo@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 11:35:44 by donheo            #+#    #+#             */
/*   Updated: 2025/04/17 09:29:17 by donheo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t	i;
	size_t	dlen;
	size_t	slen;

	dlen = 0;
	i = 0;
	while (dlen < dsize && dst[dlen] != '\0')
		dlen++;
	slen = ft_strlen(src);
	if (dlen == dsize)
		return (dlen + slen);
	i = 0;
	while (dlen + i < dsize - 1 && src[i] != '\0')
	{
		dst[dlen + i] = src[i];
		i++;
	}
	dst[dlen + i] = '\0';
	return (dlen + slen);
}
