/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 14:42:56 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/19 16:25:11 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static int	get_packetlen(const char *str, t_data *data)
{
	int packetlen;

	if (!ft_isnumber(str))
		fatal_error(EP_BADVAL, str, 0, data);
	packetlen = atoi(str);
	if (packetlen > 32768 || packetlen <= 51)
		fatal_error(EP_BADLEN, NULL, packetlen, data);
	return (packetlen);
}

t_data	*parser(char **arg, t_data *data)
{
	int		packetlen;

	for (size_t i = 0; arg[i]; i++)
	{
		if (!data->host && ft_strcmp(arg[i], "--help") == 0)
			exit(print_usage());
		else if (!data->host && arg[i][0] == '-')
			fatal_error(EP_BADOPT, arg[i] + 1, 0, data);
		else if (!data->host) {
			data->host = ft_strdup(arg[i]);
			if (!data->host)
				fatal_error(ENOMEM, NULL, 0, data);
		}
		else if (data->host && packetlen == -1)
			packetlen = get_packetlen(arg[i], data);
		else
			fatal_error(EP_MULHOST, NULL, 0, data);
	}
	if (!data->host)
		fatal_error(EP_NODATA, NULL, 0, data);
	data->packetlen = packetlen == -1 ? PACKET_LEN : packetlen;
	return (data);
}
