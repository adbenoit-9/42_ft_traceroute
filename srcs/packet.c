/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 14:21:29 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/28 16:02:33 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

#ifndef DEBUG

void	send_probe(t_data *data, char *packet, int seq)
{
	int					ret;
	struct udphdr		*udp;
	struct sockaddr_in	*to_sockaddr;

	if (seq % SQUERIES == 0)
		data->status &= ~PSENDING;
	udp = (struct udphdr *)packet;
	to_sockaddr = (struct sockaddr_in *)data->addrinfo->ai_addr;
	udp->uh_sport = htons(data->id);
	udp->uh_dport = htons(UDP_PORT + seq);
	udp->uh_ulen = htons(data->packetlen - sizeof(struct ip));
	to_sockaddr->sin_port = htons(UDP_PORT + seq);
	ret = sendto(data->sndsock, packet, SENDLEN, 0,
			data->addrinfo->ai_addr, data->addrinfo->ai_addrlen);
	if (ret == -1)
		ft_perror(ft_strerror(errno), "sendto");
}

int	recv_packet(t_data *data, char *dest)
{
	fd_set			fds;
	int				ret;
	socklen_t		addrlen;
	struct timeval	timeout;

	ret = 0;
	timeout.tv_sec = data->waittime;
	timeout.tv_usec = (data->waittime - timeout.tv_sec) * 1000000;
	addrlen = sizeof(data->sockaddr);
	FD_ZERO(&fds);
	FD_SET(data->rcvsock, &fds);
	ret = select(data->rcvsock + 1, &fds, NULL, NULL, &timeout);
	if (ret)
		recvfrom(data->rcvsock, dest, RECVLEN, 0, &data->sockaddr, &addrlen);
	else
		data->status |= RTIMEDOUT;
	FD_CLR(data->rcvsock, &fds);
	return (0);
}

int	parse_packet(t_data *data, void *packet, int seq)
{
	t_header	*hdr;

	if (data->status & RTIMEDOUT)
	{
		data->status &= ~RWAIT;
		return (seq);
	}
	hdr = (t_header *)packet;
	if (ntohs(hdr->udp.uh_sport) != data->id)
		return (false);
	if (ntohs(hdr->udp.uh_dport) - UDP_PORT == seq)
		data->status &= ~RWAIT;
	if (hdr->icmp.icmp_type != ICMP_TIME_EXCEEDED
		&& get_probe_data(seq, data).id == data->nqueries - 1)
		data->status |= END;
	return (ntohs(hdr->udp.uh_dport) - UDP_PORT);
}

#else

void	send_probe(t_data *data, char *packet, int seq)
{
	int					ret;
	struct udphdr		*udp;
	struct sockaddr_in	*to_sockaddr;

	if (seq % SQUERIES == 0)
		data->status &= ~PSENDING;
	udp = (struct udphdr *)packet;
	to_sockaddr = (struct sockaddr_in *)data->addrinfo->ai_addr;
	udp->uh_sport = htons(data->id);
	udp->uh_dport = htons(UDP_PORT + seq);
	udp->uh_ulen = htons(data->packetlen - sizeof(struct ip));
	to_sockaddr->sin_port = htons(UDP_PORT + seq);
	ret = sendto(data->sndsock, packet, SENDLEN, 0,
			data->addrinfo->ai_addr, data->addrinfo->ai_addrlen);
	if (ret == -1)
		ft_perror(ft_strerror(errno), "sendto");
	printf("\n%s[packet %d sent]%s %d / %ld bytes", S_GREEN, seq,
		S_NONE, ret, data->packetlen - sizeof(struct ip));
	if (DEBUG_LVL > 1)
		debug_udp((*(struct udphdr *)packet));
}

int	recv_packet(t_data *data, char *dest)
{
	fd_set			fds;
	int				ret;
	socklen_t		addrlen;
	struct timeval	timeout;

	ret = 0;
	timeout.tv_sec = data->waittime;
	timeout.tv_usec = (data->waittime - timeout.tv_sec) * 1000;
	addrlen = sizeof(data->sockaddr);
	FD_ZERO(&fds);
	FD_SET(data->rcvsock, &fds);
	ret = select(data->rcvsock + 1, &fds, NULL, NULL, &timeout);
	if (ret) {
		recvfrom(data->rcvsock, dest, RECVLEN, 0, &data->sockaddr, &addrlen);
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
	if (ntohs(hdr->udp.uh_sport) != data->id) {
		printf("packet [%sKO%s]\n", S_RED, S_NONE);
		if (ntohs(hdr->udp.uh_sport) != UDP_PORT + seq) {
			printf("%s%d(to sport) != %d(from dport)%s\n", S_RED,
				ntohs(hdr->udp.uh_sport), UDP_PORT + seq, S_NONE);
		}
		return (false);
	}
	printf("packet %d [%sOK%s]\n", ntohs(hdr->udp.uh_dport) - UDP_PORT,
		S_GREEN, S_NONE);
	if (ntohs(hdr->udp.uh_dport) - UDP_PORT == seq)
		data->status &= ~RWAIT;
	if (hdr->icmp.icmp_type != ICMP_TIME_EXCEEDED
			&& get_probe_data(seq, data).id == 2)
		data->status |= END;
	return (ntohs(hdr->udp.uh_dport) - UDP_PORT);	
}

#endif
