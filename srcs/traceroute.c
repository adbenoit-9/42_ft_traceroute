/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 15:39:49 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/20 12:29:23 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	send_probe(t_data *data, t_packet *packet)
{
	// struct timeval	tv;
	int				ret;
	// double      prob_time;

	ret = sendto(data->sndsock, packet, data->packetlen, 0,
		&data->sockaddr, sizeof(data->sockaddr));
	if (ret == -1)
		ft_perror(ft_strerror(errno), "sendto");
}

void	recv_reply(t_data *data)
{
	fd_set	fds;
	int		ret;
	char	packet[data->packetlen];
	socklen_t	addrlen;
	
	ret = 0;
	addrlen = sizeof(data->sockaddr);
	while (!ret)
	{
		FD_SET(data->rcvsock, &fds);
		ret = select(data->rcvsock + 1, &fds, NULL, NULL, NULL);
		if (ret == 1) {
			recvfrom(data->rcvsock, packet, sizeof(packet), 0, &data->sockaddr, &addrlen);
#ifdef DEBUG
			debug_icmp(*(struct icmp *)packet);
#endif
		}
	}
}

t_packet	*setup_packet(t_packet *packet, int seq, t_data *data)
{
	packet->udp.uh_dport = htons(UDP_PORT + seq);
	packet->udp.uh_sport = htons((data->pid | 0xffff) & 0x80);
	packet->udp.uh_ulen = htons(data->packetlen - sizeof(struct ip));
	packet->udp.uh_sum = 0;
	packet->ttl = htons(seq);
	packet->ip.ip_id = htons(data->pid);
	// packet->ip.ip_dst = 
	// udp dest port : 33434(traceroute port)+seq (pk + seq ? port : 33434-33534,
	//		give the position of the device ?)
	// udp src port :  pid with first bit set (pk ?
	// 		je crois que c'est pck on ne veut pas de rep alors pk pas a 0 ???
	//		+ seq (pk + seq ?)
	// udp len : (- iphdr)
	// udp sum: 0
	return (packet);
}

void    traceroute(t_data *data)
{
	t_packet		*packet;
	
	printf("ft_traceroute to %s (%s), %d hops max, %d bytes packets\n",
	data->host, data->ip, NHOPS_MAX, PACKET_LEN);
	packet = (t_packet *)calloc(1, data->packetlen);
	if (!packet)
		fatal_error(ENOMEM, NULL, 0, data);
	for (int ttl = START_TLL; ttl < NHOPS_MAX; ttl++)
	{
		setup_packet(packet, ttl - START_TLL, data);
#ifdef DEBUG
		debug_packet(*packet);
#endif
		for (int probe = 0; probe < NPROBES; probe++)
		{
			send_probe(data, packet);
			recv_reply(data);
		}
		// si je mets IP_HDRINCL estce que je dois remplir iphdr a la mano ? non faut appeler bind
		// if (setsockopt(data->sockfd, IPPROTO_IP, IP_HDRINCL, &ttl, sizeof(ttl)) == -1)
		// 	fatal_error(errno, "setsockopt", 0, data);
		printf("%d\t\n", ttl);
	}
	free(packet);
}
