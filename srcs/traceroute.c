/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 15:39:49 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/20 15:22:18 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

t_packet	*setup_packet(t_data *data, t_packet *packet, int seq, int ttl)
{
	packet->udp.uh_dport = htons(UDP_PORT + seq);
	packet->udp.uh_sport = htons((data->pid | 0xffff) & 0x80);
	packet->udp.uh_ulen = htons(data->packetlen - sizeof(struct ip));
	packet->udp.uh_sum = 0;
	
	packet->ttl = ttl;
	packet->seq = seq;
	gettimeofday(&packet->tv, NULL);
	
	packet->ip.ip_id = htons((data->pid | 0xffff) & 0x80 + seq);
	packet->ip.ip_hl = sizeof(packet->ip) >> 2;
	packet->ip.ip_v = IPVERSION;
	packet->ip.ip_len = htons(data->packetlen);
	packet->ip.ip_p = IPPROTO_UDP;
	packet->ip.ip_ttl = ttl;
	return (packet);
}

void	send_probe(t_data *data, t_packet *packet, int seq, int ttl)
{
	int				ret;

	setup_packet(data, packet, seq, ttl);
	ret = sendto(data->sndsock, packet, data->packetlen, 0,
		&data->sockaddr, sizeof(data->sockaddr));
	if (ret == -1)
		ft_perror(ft_strerror(errno), "sendto");
#ifdef DEBUG
	if (ret == -1)
		printf("%s[transmission failed]%s seq %d ttl %d\n", S_RED, S_NONE, seq, ttl);
	else
		printf("%s[transmission succeed]%s seq %d ttl %d len %d\n", S_GREEN, S_NONE, seq, ttl, ret);
	if (DEBUG_LVL > 1)
		debug_packet(*packet);
#endif
}

int	recv_reply(t_data *data)
{
	fd_set	fds;
	fd_set	err;
	int		ret;
	char	packet[data->packetlen];
	socklen_t	addrlen;
	struct timeval timeout;
	
	ret = 0;
	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;
	addrlen = sizeof(data->sockaddr);
	FD_ZERO(&fds);
	FD_ZERO(&err);
	FD_SET(data->rcvsock, &fds);
	FD_SET(data->rcvsock, &err);
	ret = select(data->rcvsock + 1, &fds, NULL, &err, &timeout);
	if (ret == 1) {
		if (FD_ISSET(data->rcvsock, &err))
			printf("in err fd\n");
		else
			recvfrom(data->rcvsock, packet, sizeof(packet), 0, &data->sockaddr, &addrlen);
#ifdef DEBUG
		printf("%s[packet received]%s\n", S_GREEN, S_NONE);
		if (DEBUG_LVL > 1)
			debug_icmp(*(struct icmp *)packet);
#endif
		FD_CLR(data->rcvsock, &fds);
		FD_CLR(data->rcvsock, &err);
		return (SUCCESS);
	}
	else {
#ifdef DEBUG
		printf("%s[timeout]%s\n", S_RED, S_NONE);
#endif
		FD_CLR(data->rcvsock, &fds);
		FD_CLR(data->rcvsock, &err);
		return (ETIMEDOUT);
	}
}


void    traceroute(t_data *data)
{
	t_packet		*packet;
	int				seq;

	printf("ft_traceroute to %s (%s), %d hops max, %d bytes packets\n",
	data->host, data->ip, NHOPS_MAX, PACKET_LEN);
	packet = (t_packet *)calloc(1, data->packetlen);
	if (!packet)
		fatal_error(ENOMEM, NULL, 0, data);
	seq = 0;
	for (int ttl = START_TLL; ttl <= NHOPS_MAX; ttl++)
	{
		for (int probe = 0; probe < NPROBES; probe++)
		{
			send_probe(data, packet, seq, ttl);
			recv_reply(data);
			++seq;
		}
		// printf("\n");
		// si je mets IP_HDRINCL estce que je dois remplir iphdr a la mano ? non faut appeler bind
		// if (setsockopt(data->sockfd, IPPROTO_IP, IP_HDRINCL, &ttl, sizeof(ttl)) == -1)
		// 	fatal_error(errno, "setsockopt", 0, data);
		// printf("%d\t\n", ttl);
	}
	free(packet);
}
