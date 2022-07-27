/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 15:39:49 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/27 14:18:37 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

t_probe	get_probe_data(int seq, t_data *data)
{
	t_probe	probe_data;

	probe_data.seq = seq;
	probe_data.id = (seq - 1) % data->nqueries;
	probe_data.ttl = (seq - 1) / data->nqueries + data->first_ttl;
	return (probe_data);
}

char	*dns_resolution(struct in_addr ipaddr, char *ipstr)
{
	struct hostent	*host;
	char			src[INET_ADDRSTRLEN];

	host = gethostbyaddr((char *)&ipaddr, INET_ADDRSTRLEN, AF_INET);
	if (host && host->h_name)
		return (host->h_name);
	if (!inet_ntop(AF_INET, &ipaddr, src, INET_ADDRSTRLEN))
		ft_perror(ft_strerror(errno), "inet_ntop");
	if (ft_strcmp(ipstr, "127.0.0.1") == 0)
		return ("localhost");
	return (ipstr);
}

static void	probe_transmission(t_probe probe, t_data *data)
{
	char			packet[RECVLEN];
	char			udp_packet[SENDLEN];
	struct timeval	tv;
	int				rcv_seq;

	ft_bzero(udp_packet, SENDLEN);
	if (!(data->status & PSENDING))
		gettimeofday(&tv, NULL);
	data->status = PSENDING;
	send_probe(data, udp_packet, probe.seq);
	if (probe.ttl == data->max_ttl && probe.id == data->nqueries - 1)
		data->status &= ~PSENDING;
	if (!(data->status & PSENDING)) {
		data->status = RWAIT;
		while (data->status & RWAIT) {
			recv_packet(data, packet);
			rcv_seq = parse_packet(data, packet, probe.seq);
			if (rcv_seq && handle_output(packet, rcv_seq, data, tv))
					data->status |= END;
		}
	}
}

void	traceroute(t_data *data)
{
	t_probe			probe;

	dprintf(STDOUT_FILENO,
		"ft_traceroute to %s (%s), %d hops max, %d bytes packets",
		dns_resolution(((struct sockaddr_in *)data->addrinfo->ai_addr)->sin_addr, data->host),
		data->host, MAX_TTL, data->packetlen);
	probe.seq = 1;
	probe.id = 0;
	probe.ttl = data->first_ttl;
	for (; probe.ttl <= data->max_ttl && !(data->status & END); probe.ttl++)
	{
		if (setsockopt(data->sndsock, IPPROTO_IP, IP_TTL,
				&probe.ttl, sizeof(probe.ttl)) == -1)
			fatal_error(errno, "setsockopt", 0, data);
		probe.id = 0;
		for (; probe.id < data->nqueries; probe.id++) {
			probe_transmission(probe, data);
			++probe.seq;
		}
	}
	printf("\n");
}
