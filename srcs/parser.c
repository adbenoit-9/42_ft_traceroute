/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 14:42:56 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/19 14:50:25 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static int	get_packetlen(const char *str)
{
	int packetlen;

	if (!ft_isnumber(str))
		fatal_error(EP_BADVAL, str, 0);
	packetlen = atoi(str);
	if (packetlen > 32768 || packetlen <= 51)
		fatal_error(EP_BADLEN, NULL, packetlen);
	return (packetlen);
}

t_data	parser(char **arg)
{
	char	*host;
	int		packetlen;
	t_data	data;

	host = NULL;
	packetlen = -1;
	for (size_t i = 0; arg[i]; i++)
	{
		if (!host && ft_strcmp(arg[i], "--help") == 0)
			exit(print_usage());
		else if (!host && arg[i][0] == '-')
			fatal_error(EP_BADOPT, arg[i] + 1, 0);
		else if (!host)
			host = arg[i];
		else if (host && packetlen == -1)
			packetlen = get_packetlen(arg[i]);
		else
			fatal_error(EP_MULHOST, NULL, 0);
	}
	if (!host)
		fatal_error(EP_NODATA, NULL, 0);
	data.packetlen = packetlen == -1 ? PACKET_SIZE : packetlen;
	data.host = ft_strdup(host);
	if (!data.host)
		fatal_error(ENOMEM, NULL, 0);
	return (data);
}
