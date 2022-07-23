/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 14:42:56 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/23 17:20:54 by adbenoit         ###   ########.fr       */
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

static int	get_flag_value(char flag)
{
	int		values[] = {FLAG_F, FLAG_M, FLAG_Q, FLAG_W};

	for (size_t i = 0; i < NB_FLAGS; i++)
		if (FLAGS[i] == flag)
			return (values[i]);
	return (-1);
}

static void	set_option_value(const char *str, int flag, t_data *data)
{
	long long int opt;

	opt = atoll(str);
	if (flag == 'f')
		data->flag.first_ttl = opt;		
	if (flag == 'm')
		data->flag.max_ttl = opt;		
	if (flag == 'q')
		data->flag.nprobes = opt;		
	if (flag == 'w')
		data->flag.waittime = opt;		
	// if (opt == LLONG_MAX &&
	// 		ft_strcmp(str, "9223372036854775807"))
	// 	fatal_error(EP_RESOOR, str, 0);
	// if (!ft_isnumber(str))
	// 	fatal_error(EP_BADARG, str, 0);
	// if (flag == F_COUNT) {
	// 	g_data.flag.count = opt;
	// 	if (g_data.flag.count <= 0)
	// 		fatal_error(EP_ARGOOR, str, 'c');
	// }
	// else if (flag == F_TTL) {
	// 	g_data.flag.ttl = opt;
	// 	if (g_data.flag.ttl < 0 || g_data.flag.ttl > 255)
	// 		fatal_error(EP_ARGOOR, str, 't');
	// }
	return ;
}

static int	set_flag(char *flags, t_data *data)
{
	int new_flag;

	new_flag = -1;
	for (int i = 0; flags[i]; i++)
	{
		new_flag = get_flag_value(flags[i]);
		if (new_flag == -1)
			fatal_error(EP_BADOPT, flags + i, 0, data);
		data->flag.isset |= new_flag;
		if (flags[i + 1]) {
			set_option_value(flags + i + 1, new_flag, data);
			return (-1);
		}
	}
	return (new_flag);
}

t_data	*parser(char **arg, t_data *data)
{
	int		packetlen;

	packetlen = -1;
	for (size_t i = 0; arg[i]; i++)
	{
		if (!data->host && ft_strcmp(arg[i], "--help") == 0)
			exit(print_usage());
		else if (!data->host && arg[i][0] == '-')
			set_flag( arg[i] + 1, data);
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
