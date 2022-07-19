/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 15:39:49 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/20 00:16:24 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	send_probes(t_data *data, t_packet *packet)
{
	struct timeval	tv;
	int				ret;
	// double      prob_time;

	for (int probe = 0; probe < NPROBES; probe++)
	{
		ret = sendto(data->sockfd, packet, data->packetlen, 0,
			&data->sockaddr, sizeof(data->sockaddr));
		if (ret == -1)
			ft_perror(ft_strerror(errno), "sendto");
	}
}

t_packet	*setup_packet(t_packet *packet)
{
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
	packet = (t_packet *)malloc(data->packetlen);
	if (!packet)
		fatal_error(ENOMEM, NULL, 0, data);
	setup_packet(packet);
	for (int ttl = 1; ttl < NHOPS_MAX; ttl++)
	{
		packet->ttl = ttl;
		send_probes(data, ttl);
		// si je mets IP_HDRINCL estce que je dois remplir iphdr a la mano ? non faut appeler bind
		// if (setsockopt(data->sockfd, IPPROTO_IP, IP_HDRINCL, &ttl, sizeof(ttl)) == -1)
		// 	fatal_error(errno, "setsockopt", 0, data);
		printf("%d\t\n", ttl);
	}
	free(packet);
}
