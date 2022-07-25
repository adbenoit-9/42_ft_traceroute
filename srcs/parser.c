/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 14:42:56 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/26 01:41:13 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static void	flag_error(int error, int flag, int index, const char *arg, t_data *data) {
	char	*syntax[] = {"-f first_ttl", "-m max_ttl", "-q nqueries", "-w waittime",
		"--first=first_ttl", "--max-hops=max_ttl", "--queries=nqueries"};
	char	*flag_lst[] = FLAG_LST;
	char	str[4096];

	if (error == ET_BADARG)
		sprintf(str, ET_BADARG_MSG, flag_lst[flag], arg, index + 1);
	else if (error == ET_NOARG)
		sprintf(str, ET_NOARG_MSG, flag_lst[flag], index + 1, syntax[flag]);
	else if (error == ET_NOARG)
		sprintf(str, ET_NOARG_MSG, flag_lst[flag], index + 1, syntax[flag]);
	else if (error == ET_TTLARG)
		sprintf(str, ET_TTLARG_MSG, data->first_ttl, data->max_ttl);
	exit_error(str, data, USAGE_ERR);
}

static int	get_packetlen(const char *str, t_data *data, int index)
{
	int 	packetlen;
	char	error[4096];

	if (!ft_isnumber(str))
		fatal_error(ET_BADVAL, str, index + 1, data);
	packetlen = atoi(str);
	if (packetlen > 65000 || (packetlen < 0 && str[0] != '-')) {
		sprintf(error, "too big packetlen %s specified", str);
		exit_error(error, data, USAGE_ERR);
	}
	if (packetlen < 28)
		packetlen = 28;
	return (packetlen);
}

static void	set_option_value(const char *str, int flag, int i, t_data *data)
{
	int		opt;
	double	wt;

	if (!ft_isnumber(str) && (flag != FLAG_W || !ft_isrealnumber(str)))
		flag_error(ET_BADARG, flag, i, str, data);
	opt = atoi(str);
	switch (flag % NB_FLAGS)
	{
	case FLAG_W:
		wt = atof(str);
		if (wt > 86400. || wt <= 0.)
			exit_error("wait time cannot be more than 86400", data, USAGE_ERR);
		data->waittime = wt;
		break ;
	case FLAG_F:
		if (opt < 1 || opt > 64)
			exit_error("first hop out of range", data, USAGE_ERR);
		data->first_ttl = opt;
		break;
	case FLAG_M:
		if (opt < 1 || opt > 255)
			exit_error("max hops cannot be more than 255", data, USAGE_ERR);
		data->max_ttl = opt;
		break;
	case FLAG_Q:
		if (opt < 1 || opt > 10)
			exit_error("no more than 10 probes per hop", data, USAGE_ERR);
		data->nqueries = opt;
		break;
	default:
		break;
	}
	return ;
}

static int	set_option(char *option, int index, t_data *data)
{
	int		len;
	char	*opt_lst[] = FLAG_LST;

	if (ft_strcmp(option, "-help") == 0)
		exit(print_usage());
	for (int i = 0; i < NB_FLAGS * 2 - 1; i++)
	{
		len  = strlen(opt_lst[i]);
		if (strncmp(option, opt_lst[i], len) == 0)
		{
			if (i >= NB_FLAGS && !option[len]) 
				flag_error(ET_NOARG, i, index, NULL, data);
			else if (i >= NB_FLAGS && option[len] != '=')
				fatal_error(ET_BADOPT, option, index + 1, data);
			else if (i >= NB_FLAGS)
				set_option_value(option + len + 1, i, index, data);
			else if (option[len])
				set_option_value(option + len, i, index, data);
			else
				return (i);
			return (-1);
		}
	}
	fatal_error(ET_BADOPT, option, index + 1, data);
	return (-1);
}

t_data	*parser(char **arg, t_data *data)
{
	int		ipcklen;
	int		ihost;
	int		flag;
	size_t	i;

	ipcklen = -1;
	ihost = -1;
	flag = -1;
	for (i = 0; arg[i]; i++)
	{
		if (flag != -1)
			set_option_value(arg[i], flag, i, data);
		else if (arg[i][0] == '-') {
			flag = set_option(arg[i] + 1, i, data);
			continue ;
		}
		else if (ihost == -1) {
			ihost = i;
		}
		else if (ihost != -1 && ipcklen == -1)
			ipcklen = i;
		else
			fatal_error(ET_EXTRAARG, arg[i], i + 1, data);
		flag = -1;
	}
	if (flag != -1)
		flag_error(ET_NOARG, flag, i, NULL, data);
	else if (ihost == -1)
		fatal_error(ET_NOHOST, NULL, 0, data);
	setup_host(arg, ihost, data);
	if (ipcklen != -1)
		data->packetlen = get_packetlen(arg[ipcklen], data, ipcklen);
	if (data->first_ttl > data->max_ttl)
		flag_error(ET_TTLARG, NONE, NONE, NULL, data);
	return (data);
}
