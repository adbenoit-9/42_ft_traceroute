/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 15:39:49 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/25 15:15:06 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int	output(void	*packet, int ttl, int probe, t_data *data,
		struct timeval send_time)
{
	char			src[INET_ADDRSTRLEN];
	static char 	tmp[INET_ADDRSTRLEN];
	struct timeval	tv;
	double			rtt;

	gettimeofday(&tv, NULL);
	rtt = tv_to_ms(tv) - tv_to_ms(send_time);
	if (!(data->status & RTIMEDOUT)) {
		data->waittime = 3. * rtt > WAITTIME ? 3. * rtt : WAITTIME;
		if (!inet_ntop(AF_INET, &((t_header *)packet)->ip.ip_src, src, INET_ADDRSTRLEN))
			ft_perror(ft_strerror(errno), "inet_ntop");
		if (probe == 0)
			dprintf(STDOUT_FILENO, "%2d  %s (%s)  %.3f ms ", ttl, src, src, rtt);
		else if (strncmp(src, tmp, INET_ADDRSTRLEN) != 0)
			dprintf(STDOUT_FILENO, " %s (%s) %.3f ms ", src, src, rtt);
		else
			dprintf(STDOUT_FILENO, " %.3f ms ", rtt);
		strncpy(tmp,src, INET_ADDRSTRLEN);
		if (probe == 2 && strncmp(src, data->ip, INET_ADDRSTRLEN) == 0)
			return (true);
	}
	else if (probe == 0) {
		dprintf(STDOUT_FILENO, "%2d  *", ttl);
	}
	else {
		dprintf(STDOUT_FILENO, " *");
	}
	return (false);
}

void    traceroute(t_data *data)
{
	char			*datagram;
	struct timeval	tv;
	char			packet[HDR_SIZE];
	int				seq;

	printf("ft_traceroute to %s (%s), %d hops max, %d bytes packets\n",
	data->host, data->ip, MAX_TTL, data->packetlen);
	datagram = calloc(1, data->packetlen - sizeof(struct ip));
	if (!datagram)
		fatal_error(ENOMEM, NULL, 0, data);
	seq = 0;
	for (int ttl = data->first_ttl; ttl <= data->max_ttl &&
			!(data->status & END); ttl++)
	{
		if (setsockopt(data->sndsock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1)
			fatal_error(errno, "setsockopt", 0, data);
		data->waittime = WAITTIME;
			gettimeofday(&tv, NULL);
		data->status = PSENDING;
		for (int probe = 0; probe < data->nqueries; probe++)
		{
			send_probe(data, datagram, ++seq);
			if (!(data->status & PSENDING)) {
				data->status = RWAIT;
				while (data->status & RWAIT) {
					recv_packet(data, packet);
					check_packet(data, packet, seq);
				}
				if (output(packet, ttl, probe, data, tv))
					data->status |= END;
				gettimeofday(&tv, NULL);
			}
		}
		printf("\n");
	}
	free(datagram);
}
