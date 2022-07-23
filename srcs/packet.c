/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 14:21:29 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/23 16:47:44 by adbenoit         ###   ########.fr       */
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

int	recv_packet(t_data *data, char *dest)
{
	fd_set			fds;
	int				ret;
	socklen_t		addrlen;
	struct timeval	timeout;
	
	ret = 0;
	timeout.tv_sec = WAITTIME;
	timeout.tv_usec = 0;
	addrlen = sizeof(data->sockaddr);
	FD_ZERO(&fds);
	FD_SET(data->rcvsock, &fds);
	ret = select(data->rcvsock + 1, &fds, NULL, NULL, &timeout);
	if (ret) {
		recvfrom(data->rcvsock, dest, 52, 0, &data->sockaddr, &addrlen);
#ifdef DEBUG
		printf("%s[packet received]%s\n", S_GREEN, S_NONE);
		if (DEBUG_LVL > 1) {
			debug_ip(((t_header *)dest)->ip);
			debug_icmp(((t_header *)dest)->icmp);
			debug_udp(((t_header *)dest)->udp);
			debug_packet(*(t_probe_packet *)(dest + sizeof(t_header)));
		}
		if (((t_header *)dest)->icmp.icmp_type != 0) {
			printf("%s[error] %s%s\n", S_RED, S_NONE,
				icmp_strerror(((t_header *)dest)->icmp.icmp_type));
		}
#endif
	}
    else {
        data->status |= RTIMEDOUT;
    }
	FD_CLR(data->rcvsock, &fds);
    return (0);
}

bool	check_packet(t_data *data, void *packet, int seq)
{
	t_header	*hdr;

	if (data->status & RTIMEDOUT) {
		data->status &= ~RWAIT;
		return (true);	
	}
	hdr = (t_header *)packet;
	if (ntohs(hdr->udp.uh_sport) != data->id ||
			ntohs(hdr->udp.uh_dport) != UDP_PORT + seq + 1)
		return (false);
	data->status &= ~RWAIT;
	return (true);	
}
