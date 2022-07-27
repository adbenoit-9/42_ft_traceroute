/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 16:35:48 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/27 12:44:39 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		++len;
	return (len);
}

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

int	ft_isnumber(const char *str)
{
	int	i;

	i = 0;
	if (!str[0])
		return (false);
	if (str[0] == '-' || str[0] == '+')
		++i;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		++i;
	}
	return (true);
}

int	ft_isrealnumber(const char *str)
{
	int	i;
	int count;

	i = 0;
	count = 0;
	if (!str[0] || str[0] == '.')
		return (false);
	if (str[0] == '-' || str[0] == '+')
		++i;
	while (str[i])
	{
		if (str[i] == '.')
			++count;
		else if (!ft_isdigit(str[i]))
			return (false);
		if (count > 1)
			return (false);
		++i;
	}
	return (true);
}

char	*ft_strdup(const char *s1)
{
	char	*copy;
	int		i;

	copy = malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!copy)
		return (0);
	i = 0;
	while (s1[i])
	{
		copy[i] = s1[i];
		++i;
	}
	copy[i] = '\0';
	return (copy);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	if (dest == NULL || src == NULL)
		return (dest);
	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		++i;
	}
	dest[i] = 0;
	return (dest);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] && s2[i] && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		++i;
	}
	if (i != n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = 0;
		++i;
	}
}

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		((unsigned char *)b)[i] = (unsigned char)c;
		++i;
	}
	return (b);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (src == NULL && dst == NULL)
		return (NULL);
	while (i < n)
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		++i;
	}
	return (dst);
}
