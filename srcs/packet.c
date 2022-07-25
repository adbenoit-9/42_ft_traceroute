/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 14:21:29 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/25 19:59:33 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	send_probe(t_data *data, char *packet, int seq)
{
	int ret;

	if (seq % SQUERIES == 0)
		data->status &= ~PSENDING;
	((struct udphdr *)packet)->uh_sport = htons(data->id);
	((struct udphdr *)packet)->uh_dport = htons(UDP_PORT + seq);
	((struct udphdr *)packet)->uh_ulen = htons(data->packetlen - sizeof(struct ip));
	((struct sockaddr_in *)data->addrinfo->ai_addr)->sin_port = htons(UDP_PORT + seq);
	ret = sendto(data->sndsock, packet, SENDLEN, 0,
			data->addrinfo->ai_addr, data->addrinfo->ai_addrlen);
	if (ret == -1)
		ft_perror(ft_strerror(errno), "sendto");
#ifdef DEBUG
	printf("\n%s[packet %d sent]%s %d / %ld bytes", S_GREEN, seq, S_NONE, ret, data->packetlen - sizeof(struct ip));
	if (DEBUG_LVL > 1)
		debug_udp((*(struct udphdr *)packet));
#endif
}

int	recv_packet(t_data *data, char *dest)
{
	fd_set			fds;
	int				ret;
	socklen_t		addrlen;
	struct timeval	timeout;
	
	ret = 0;
	timeout.tv_sec = data->waittime;
	timeout.tv_usec = (data->waittime / timeout.tv_sec) * 1000;
	addrlen = sizeof(data->sockaddr);
	FD_ZERO(&fds);
	FD_SET(data->rcvsock, &fds);
	ret = select(data->rcvsock + 1, &fds, NULL, NULL, &timeout);
	if (ret) {
		recvfrom(data->rcvsock, dest, RECVLEN, 0, &data->sockaddr, &addrlen);
#ifdef DEBUG
		printf("\n%s[packet received]%s\n", S_GREEN, S_NONE);
		if (DEBUG_LVL > 1) {
			debug_ip(((t_header *)dest)->ip);
			debug_icmp(((t_header *)dest)->icmp);
			debug_udp(((t_header *)dest)->udp);
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

int	parse_packet(t_data *data, void *packet, int seq)
{
	t_header	*hdr;

	if (data->status & RTIMEDOUT) {
		data->status &= ~RWAIT;
		return (seq);	
	}
	hdr = (t_header *)packet;
#ifndef DEBUG
	if (ntohs(hdr->udp.uh_sport) != data->id)
		return (false);
#else
	if (ntohs(hdr->udp.uh_sport) != data->id) {
		printf("packet [%sKO%s]\n", S_RED, S_NONE);
		if (ntohs(hdr->udp.uh_dport) != data->id)
			printf("%s%d(to dport) != %d(from sport)%s\n", S_RED, ntohs(hdr->udp.uh_dport), data->id, S_NONE);
		if (ntohs(hdr->udp.uh_sport) != UDP_PORT + seq)
			printf("%s%d(to sport) != %d(from dport)%s\n", S_RED, ntohs(hdr->udp.uh_sport), UDP_PORT + seq, S_NONE);
		return (false);
	}
	printf("packet %d [%sOK%s]\n", ntohs(hdr->udp.uh_dport) - UDP_PORT, S_GREEN, S_NONE);
#endif
	if (ntohs(hdr->udp.uh_dport) - UDP_PORT == seq)
		data->status &= ~RWAIT;
	return (ntohs(hdr->udp.uh_dport) - UDP_PORT);	
}
