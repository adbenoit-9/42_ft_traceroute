/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 14:42:56 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/23 19:39:15 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static int	get_packetlen(const char *str, t_data *data)
{
	int 	packetlen;
	char	error[4096];

	if (!ft_isnumber(str))
		fatal_error(EP_BADVAL, str, 0, data);
	packetlen = atoi(str);
	if (packetlen > 65000 || (packetlen < 0 && str[0] != '-')) {
		sprintf(error, "too big packetlen %s specified", str);
		exit_error(error, data, USAGE_ERR);
	}
	if (packetlen < 28)
		packetlen = 28;
	return (packetlen);
}

static int	get_flag_value(char flag)
{
	int		values[] = {FLAG_F, FLAG_M, FLAG_Q};

	for (size_t i = 0; i < NB_FLAGS; i++)
		if (FLAGS[i] == flag)
			return (values[i]);
	return (-1);
}

static char	reverse_flag_value(int flag)
{
	int		values[] = {FLAG_F, FLAG_M, FLAG_Q};

	for (size_t i = 0; i < NB_FLAGS; i++)
		if (values[i] == flag)
			return (FLAGS[i]);
	return (0);
}

static void	set_option_value(const char *str, int flag, int i, t_data *data)
{
	int		opt;
	char	error[4096];

	if (!ft_isnumber(str)) {
		sprintf(error, EP_OPTARG_MSG, reverse_flag_value(flag), str, i + 1);
		exit_error(error, data, USAGE_ERR);
	}
	opt = atoi(str);
	switch (flag)
	{
	case FLAG_F:
		if (opt < 1 || opt > 64)
			exit_error("first hop out of range", data, USAGE_ERR);
		data->flag.first_ttl = opt;
		break;
	case FLAG_M:
		if (opt < 1 || opt > 255)
			exit_error("max hops cannot be more than 255", data, USAGE_ERR);
		data->flag.max_ttl = opt;
		break;
	case FLAG_Q:
		if (opt < 1 || opt > 10)
			exit_error("no more than 10 probes per hop", data, USAGE_ERR);
		data->flag.nprobes = opt;
		break;
	default:
		break;
	}
	return ;
}

static int	set_flag(char *flags, t_data *data, int index)
{
	int new_flag;

	new_flag = -1;
	for (int i = 0; flags[i]; i++)
	{
		new_flag = get_flag_value(flags[i]);
		if (new_flag == -1)
			fatal_error(EP_BADOPT, flags + i, index + 1, data);
		data->flag.isset |= new_flag;
		if (flags[i + 1]) {
			set_option_value(flags + i + 1, new_flag, index, data);
			return (-1);
		}
	}
	return (new_flag);
}

t_data	*parser(char **arg, t_data *data)
{
	int		packetlen;
	int		flag;

	packetlen = -1;
	flag = -1;
	for (size_t i = 0; arg[i]; i++)
	{
		if (flag != -1)
			set_option_value(arg[i], flag, i, data);
		else if (ft_strcmp(arg[i], "--help") == 0)
			exit(print_usage());
		else if (arg[i][0] == '-') {
			flag = set_flag( arg[i] + 1, data, i);
			continue ;
		}
		else if (!data->host) {
			data->host = ft_strdup(arg[i]);
			if (!data->host)
				fatal_error(ENOMEM, NULL, 0, data);
		}
		else if (data->host && packetlen == -1)
			packetlen = get_packetlen(arg[i], data);
		else
			fatal_error(EP_MULHOST, NULL, 0, data);
		flag = -1;
	}
	if (!data->host)
		fatal_error(EP_NODATA, NULL, 0, data);
	data->packetlen = packetlen == -1 ? PACKET_LEN : packetlen;
	return (data);
}
