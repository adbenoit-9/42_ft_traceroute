/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 13:44:42 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/19 14:35:59 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_FT
# define FT_TRACEROUTE_FT

# include "defs.h"

typedef struct s_data
{
    char    *host;
    int     packetlen;
}   t_data;

extern t_data g_data;

char	*ft_strerror(int error);
int		ft_perror(const char *error, const char *fct);
int		fatal_error(int error, const char *arg, const int len);
t_data	parser(char **arg);

int		ft_strcmp(const char *s1, const char *s2);
int		ft_isnumber(const char *str);
char	*ft_strdup(const char *s1);
size_t	ft_strlen(const char *str);

void	clear_data(t_data *data);
int		print_usage(void);

#endif
