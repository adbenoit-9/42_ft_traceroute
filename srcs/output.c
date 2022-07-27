/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 12:14:41 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/27 13:26:27 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static double	tv_to_ms(struct timeval t)
{
	return (t.tv_sec * 1000. + t.tv_usec / 1000.);
}

static const char	*dns_resolution(struct ip ip)
{
	struct hostent	*host;
	char			name[4096];
	char			src[INET_ADDRSTRLEN];
	char			*ptr;

	gethostname(name, 4096);
	host = gethostbyaddr((char *)&ip.ip_src, INET_ADDRSTRLEN, AF_INET);
	if (host && host->h_name)
		return (host->h_name);
	if (!inet_ntop(AF_INET, &ip.ip_src, src, INET_ADDRSTRLEN))
		ft_perror(ft_strerror(errno), "inet_ntop");
	if (ft_strcmp(src, "127.0.0.1") == 0)
		return ("localhost");
	ptr = src;	
	return (ptr);
}

static int	print_trace(void *packet, int seq, t_data *data, double rtt)
{
	char			src[INET_ADDRSTRLEN];
	static char 	tmp[INET_ADDRSTRLEN];
	t_probe			probe;
	struct ip		ip;

	probe = get_probe_data(seq, data);
	if (!(data->status & RTIMEDOUT) && rtt > -0.99) {
		ip = ((t_header *)packet)->ip;
		if (!inet_ntop(AF_INET, &ip.ip_src, src, INET_ADDRSTRLEN))
			ft_perror(ft_strerror(errno), "inet_ntop");
		if (probe.id == 0) {
			dprintf(STDOUT_FILENO, "\n%2d  %s (%s)  %.3f ms ", probe.ttl,
				dns_resolution(ip), src, rtt);
		}
		else if (ft_strncmp(src, tmp, INET_ADDRSTRLEN) != 0) {
			dprintf(STDOUT_FILENO, " %s (%s) %.3f ms ",
				dns_resolution(ip), src, rtt);
		}
		else
			dprintf(STDOUT_FILENO, " %.3f ms ", rtt);
		ft_strncpy(tmp,src, INET_ADDRSTRLEN);
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
