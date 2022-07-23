/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 15:39:49 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/23 15:17:00 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	send_probe(t_data *data, char *packet, int seq, int ttl)
{
	((t_probe_packet *)packet)->ttl = ttl;
	((t_probe_packet *)packet)->seq = ++seq;
	gettimeofday(&((t_probe_packet *)packet)->tv, NULL);
	((struct sockaddr_in *)data->addrinfo->ai_addr)->sin_port = htons(UDP_PORT + seq);
	if (sendto(data->sndsock, packet, sizeof(packet), 0,
			data->addrinfo->ai_addr, data->addrinfo->ai_addrlen) == -1)
		ft_perror(ft_strerror(errno), "sendto");
}

int	traceroute_output(void	*packet, int ttl, int probe, t_data *data,
		struct timeval send_time)
{
	char			src[INET_ADDRSTRLEN];
	static char 	tmp[INET_ADDRSTRLEN];
	struct timeval	tv;
	double			time;

	gettimeofday(&tv, NULL);
	time = tv_to_ms(tv) - tv_to_ms(send_time);
	if (!(data->status & RTIMEDOUT)) {
		if (!inet_ntop(AF_INET, &((t_header *)packet)->ip.ip_src, src, INET_ADDRSTRLEN))
			ft_perror(ft_strerror(errno), "inet_ntop");
		if (probe == 0)
			dprintf(STDOUT_FILENO, "%2d  %s (%s)  %.3f ms ", ttl, src, src, time);
		else if (strncmp(src, tmp, INET_ADDRSTRLEN) != 0)
			dprintf(STDOUT_FILENO, "\n    %s (%s) %.3f ms ", src, tmp, time);
		else
			dprintf(STDOUT_FILENO, " %.3f ms ", time);
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
	char	*probe_packet;
	char	rcv_packet[data->packetlen];
	int		seq;

	printf("ft_traceroute to %s (%s), %d hops max, %d bytes packets\n",
	data->host, data->ip, NHOPS_MAX, data->packetlen);
	probe_packet = calloc(1, data->packetlen);
	if (!probe_packet)
		fatal_error(ENOMEM, NULL, 0, data);
	seq = 0;
	for (int ttl = START_TLL; ttl <= NHOPS_MAX && !(data->status & END); ttl++)
	{
		if (setsockopt(data->sndsock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1)
			fatal_error(errno, "setsockopt", 0, data);
		for (int probe = 0; probe < NPROBES; probe++)
		{
			send_probe(data, probe_packet, seq, ttl);
			data->status = RWAIT;
			while (data->status & RWAIT) {
				recv_reply(data, rcv_packet);
				check_reply(data, rcv_packet, seq);
			}
			if (traceroute_output(rcv_packet, ttl, probe, data,
					((t_probe_packet *)(probe_packet))->tv))
				data->status |= END;
		}
		printf("\n");
	}
	free(probe_packet);
}
