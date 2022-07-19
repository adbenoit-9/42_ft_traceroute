/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 14:42:56 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/16 20:02:26 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static int	get_flag_value(char flag)
{
	int		values[] = {F_HELP, F_VERBOSE, F_COUNT, F_QUIET, F_TTL};

	for (size_t i = 0; FLAGS[i]; i++)
		if (FLAGS[i] == flag)
			return (values[i]);
	return (-1);
}

static void	set_option_value(const char *str, int flag)
{
	long long int opt;

	opt = atoll(str);
	if (opt == LLONG_MAX &&
			ft_strcmp(str, "9223372036854775807"))
		fatal_error(EP_RESOOR, str, 0);
	if (!ft_isnumber(str))
		fatal_error(EP_BADARG, str, 0);
	if (flag == F_COUNT) {
		g_data.flag.count = opt;
		if (g_data.flag.count <= 0)
			fatal_error(EP_ARGOOR, str, 'c');
	}
	else if (flag == F_TTL) {
		g_data.flag.ttl = opt;
		if (g_data.flag.ttl < 0 || g_data.flag.ttl > 255)
			fatal_error(EP_ARGOOR, str, 't');
	}
	return ;
}

static int	set_flag(char *flags)
{
	int	new_flag;

	new_flag = -1;
	for (int i = 0; flags[i]; i++)
	{
		new_flag = get_flag_value(flags[i]);
		if (new_flag == -1)
			fatal_error(EP_BADOPT, NULL, flags[i]);
		g_data.flag.isset |= new_flag;
		if (new_flag == F_HELP)
		{
			print_usage();
			exit(SUCCESS);
		}
		else if ((new_flag == F_COUNT || new_flag == F_TTL) && flags[i + 1]) {
			set_option_value(flags + i + 1, new_flag);
			return (-1);
		}
	}
	return (new_flag);
}

static void	check_missing_arg(char *host)
{
	if (FLAG_ISSET(F_COUNT) && g_data.flag.count == -1)
		fatal_error(EP_NOARG, NULL, 'c');
	else if (FLAG_ISSET(F_TTL) && g_data.flag.ttl == -1)
		fatal_error(EP_NOARG, NULL, 't');
	else if (!host)
		fatal_error(EP_NODATA, NULL, 0);
}

bool	parser(char **arg)
{
	char	*host;
	int		flag;

	host = NULL;
	flag = -1;
	for (size_t i = 0; arg[i]; i++)
	{
		if (flag == F_COUNT || flag == F_TTL) {
			set_option_value(arg[i], flag);
			flag = -1;
		}
		else if (arg[i][0] == '-' && arg[i][1]) {
			flag = set_flag(arg[i] + 1);
		}
		else if (!host) {
			host = arg[i];
			flag = -1;
		}
		else
			fatal_error(EP_MULHOST, NULL, 0);
	}
	check_missing_arg(host);
	g_data.host = ft_strdup(host);
	if (!g_data.host)
		fatal_error(ENOMEM, NULL, 0);
	return (SUCCESS);
}
