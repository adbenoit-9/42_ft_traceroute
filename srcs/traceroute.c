/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 15:39:49 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/20 18:08:24 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

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

int	recv_reply(t_data *data, char *dest)
{
	fd_set	fds;
	fd_set	err;
	int		ret;
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
			recvfrom(data->rcvsock, dest, 52, 0, &data->sockaddr, &addrlen);
#ifdef DEBUG
		printf("%s[packet received]%s\n", S_GREEN, S_NONE);
		if (DEBUG_LVL > 1) {
			struct ip ip;
			ip = *(struct ip *)dest;
			debug_ip(ip);
			printf("len : %d\n", (int)(ip.ip_hl << 2));
			debug_icmp(*(struct icmp *)(dest + sizeof(struct ip)));
		}
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

void	traceroute_output(char *packet)
{
	(void)packet;
}

void    traceroute(t_data *data)
{
	t_packet		*packet;
	char			rcv_packet[data->packetlen];
	struct timeval	tv;
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
			gettimeofday(&tv, NULL);
			send_probe(data, packet, seq, ttl);
			if (recv_reply(data, rcv_packet) == SUCCESS)
				traceroute_output(rcv_packet);
			else
				printf(" *");
			if (!ft_wait(tv, 1))
				fatal_error(errno, "gettimeofday", 0, data);
			;
			++seq;
		}
		printf("\n");
		// si je mets IP_HDRINCL estce que je dois remplir iphdr a la mano ? non faut appeler bind
		// if (setsockopt(data->sockfd, IPPROTO_IP, IP_HDRINCL, &ttl, sizeof(ttl)) == -1)
		// 	fatal_error(errno, "setsockopt", 0, data);
		// printf("%d\t\n", ttl);
	}
	free(packet);
}
