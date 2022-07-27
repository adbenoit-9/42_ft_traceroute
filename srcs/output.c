/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 12:14:41 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/27 12:15:36 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static t_probe	get_probe_data(int seq, t_data *data) {
	t_probe	probe_data;
	
	probe_data.seq = seq;
	probe_data.id = (seq - 1) % data->nqueries;
	probe_data.ttl = (seq - 1) / data->nqueries + data->first_ttl;
	return (probe_data);
}

static const char	*dns_resolution(struct in_addr addr, char *ip)
{
	struct hostent	*host;

	host = gethostbyaddr((char *)&addr, INET_ADDRSTRLEN, AF_INET);
	if (host && host->h_name)
		return (host->h_name);
	return (ip);
}

static int	print_trace(void *packet, int seq, t_data *data, double rtt)
{
	char			src[INET_ADDRSTRLEN];
	static char 	tmp[INET_ADDRSTRLEN];
	t_probe			probe;
	struct in_addr	addr;

	probe = get_probe_data(seq, data);
	if (!(data->status & RTIMEDOUT) && rtt > -0.99) {
		addr = ((t_header *)packet)->ip.ip_src;
		if (!inet_ntop(AF_INET, &addr, src, INET_ADDRSTRLEN))
			ft_perror(ft_strerror(errno), "inet_ntop");
		if (probe.id == 0) {
			dprintf(STDOUT_FILENO, "\n%2d  %s (%s)  %.3f ms ", probe.ttl,
				dns_resolution(addr, src), src, rtt);
		}
		else if (ft_strncmp(src, tmp, INET_ADDRSTRLEN) != 0) {
			dprintf(STDOUT_FILENO, " %s (%s) %.3f ms ",
				dns_resolution(addr, src), src, rtt);
		}
		else
			dprintf(STDOUT_FILENO, " %.3f ms ", rtt);
		ft_strncpy(tmp,src, INET_ADDRSTRLEN);
		if (probe.id == 2 && ft_strncmp(src, data->ip, INET_ADDRSTRLEN) == 0)
			return (true);
	}
	else if (probe.id == 0) {
		dprintf(STDOUT_FILENO, "\n%2d  *", probe.ttl);
	}
	else {
		dprintf(STDOUT_FILENO, " *");
	}
	return (false);	
}

int	handle_output(void	*packet, int seq, t_data *data, struct timeval sent_time)
{
	struct timeval	tv;
	double			rtt;
	static int		last_seq;

	gettimeofday(&tv, NULL);
	rtt = tv_to_ms(tv) - tv_to_ms(sent_time);
	if (seq < last_seq)
		return (false);
	for (int i = last_seq + 1; i < seq; i++)
		print_trace(packet, i, data, -1);
	last_seq = seq;
	return (print_trace(packet, seq, data, rtt));
}
