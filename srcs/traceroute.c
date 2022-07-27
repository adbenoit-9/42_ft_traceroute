/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 15:39:49 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/27 12:14:21 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void    traceroute(t_data *data)
{
	char			*udp_packet;
	struct timeval	tv;
	char			packet[RECVLEN];
	int				seq;
	int				rcv_seq;

	dprintf(STDOUT_FILENO, "ft_traceroute to %s (%s), %d hops max, %d bytes packets",
	data->host, data->ip, MAX_TTL, data->packetlen);
	udp_packet = ft_calloc(1, SENDLEN);
	if (!udp_packet)
		fatal_error(ENOMEM, NULL, 0, data);
	seq = 0;
	for (int ttl = data->first_ttl; ttl <= data->max_ttl &&
			!(data->status & END); ttl++)
	{
		if (setsockopt(data->sndsock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1)
			fatal_error(errno, "setsockopt", 0, data);
		gettimeofday(&tv, NULL);
		for (int probe = 0; probe < data->nqueries; probe++)
		{
			data->status = PSENDING;
			send_probe(data, udp_packet, ++seq);
			if (ttl == data->max_ttl && probe == data->nqueries - 1)
				data->status &= ~PSENDING;
			if (!(data->status & PSENDING)) {
				data->status = RWAIT;
				while (data->status & RWAIT) {
					recv_packet(data, packet);
					rcv_seq = parse_packet(data, packet, seq);
					if (rcv_seq)
						if (handle_output(packet, rcv_seq, data, tv))
							data->status |= END;
				}
				gettimeofday(&tv, NULL);
			}
		}
	}
	printf("\n");
	free(udp_packet);
}
