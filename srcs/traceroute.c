/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 15:39:49 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/25 16:52:03 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

t_probe	get_probe_data(int seq, t_data *data) {
	t_probe	probe_data;
	
	probe_data.seq = seq;
	probe_data.id = (seq - 1) % data->nqueries;
	probe_data.ttl = (seq - 1) / probe_data.id + data->first_ttl;
	return (probe_data);
}

int	print_trace(void *packet, t_probe probe, t_data *data, double rtt)
{
	char			src[INET_ADDRSTRLEN];
	static char 	tmp[INET_ADDRSTRLEN];

	if (!(data->status & RTIMEDOUT) && rtt > -0.99) {
		data->waittime = 3. * rtt > WAITTIME ? 3. * rtt : WAITTIME;
		if (!inet_ntop(AF_INET, &((t_header *)packet)->ip.ip_src, src, INET_ADDRSTRLEN))
			ft_perror(ft_strerror(errno), "inet_ntop");
		if (probe.id == 0)
			dprintf(STDOUT_FILENO, "%2d  %s (%s)  %.3f ms ", probe.ttl, src, src, rtt);
		else if (strncmp(src, tmp, INET_ADDRSTRLEN) != 0)
			dprintf(STDOUT_FILENO, " %s (%s) %.3f ms ", src, src, rtt);
		else
			dprintf(STDOUT_FILENO, " %.3f ms ", rtt);
		strncpy(tmp,src, INET_ADDRSTRLEN);
		if (probe.id == 2 && strncmp(src, data->ip, INET_ADDRSTRLEN) == 0)
			return (true);
	}
	else if (probe.id == 0) {
		dprintf(STDOUT_FILENO, "%2d  *", probe.ttl);
	}
	else {
		dprintf(STDOUT_FILENO, " *");
	}
	return (false);	
}

int	output(void	*packet, int seq, t_data *data, struct timeval send_time)
{
	struct timeval	tv;
	double			rtt;
	t_probe			current;
	t_probe			last = get_probe_data(seq, data);

	gettimeofday(&tv, NULL);
	rtt = tv_to_ms(tv) - tv_to_ms(send_time);
	current = get_probe_data(seq, data);
#ifdef DEBUG
	printf("current probe %d - ttl %d - seq %d\n", current.id, current.ttl, current.seq);
	printf("last probe %d - ttl %d - seq %d\n", last.id, last.ttl, last.seq);
#endif
	if (current.seq < last.seq)
		return (false);
	for (int i = last.seq; i < current.seq; i++) {
		print_trace(packet, get_probe_data(i, data), data, -1);
	}
	last = current;
	return (print_trace(packet, current, data, rtt));
}

void    traceroute(t_data *data)
{
	char			*udp_packet;
	struct timeval	tv;
	char			packet[HDR_SIZE];
	int				seq;
	int				rcv_seq;

	printf("ft_traceroute to %s (%s), %d hops max, %d bytes packets\n",
	data->host, data->ip, MAX_TTL, data->packetlen);
	udp_packet = calloc(1, data->packetlen - sizeof(struct ip));
	if (!udp_packet)
		fatal_error(ENOMEM, NULL, 0, data);
	seq = 0;
	for (int ttl = data->first_ttl; ttl <= data->max_ttl &&
			!(data->status & END); ttl++)
	{
		if (setsockopt(data->sndsock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1)
			fatal_error(errno, "setsockopt", 0, data);
		data->waittime = WAITTIME;
			gettimeofday(&tv, NULL);
		for (int probe = 0; probe < data->nqueries; probe++)
		{
			data->status = PSENDING;
			send_probe(data, udp_packet, ++seq);
			printf("seq = %d | %d\n", seq, data->status & PSENDING);
			if (!(data->status & PSENDING)) {
				data->status = RWAIT;
				while (data->status & RWAIT) {
					recv_packet(data, packet);
					rcv_seq = parse_packet(data, packet, seq);
					if (rcv_seq) {
						if (output(packet, rcv_seq, data, tv))
							data->status |= END;
						exit(3);
					}
				}
				gettimeofday(&tv, NULL);
			}
		}
		printf("\n");
	}
	free(udp_packet);
}
