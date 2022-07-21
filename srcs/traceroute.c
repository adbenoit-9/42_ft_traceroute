/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 15:39:49 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/21 12:47:25 by adbenoit         ###   ########.fr       */
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
			debug_icmp(*(struct icmp *)(dest + sizeof(struct ip)));
		}
		if (((t_packet *)dest)->hdr.reply.icmp.icmp_type != 0) {
			printf("%s[error] %s%s\n", S_RED, S_NONE,
				icmp_strerror(((struct icmp *)(dest + sizeof(struct ip)))->icmp_type));
		}
#endif
		FD_CLR(data->rcvsock, &fds);
		FD_CLR(data->rcvsock, &err);
		return (SUCCESS);
	}
	else {
		FD_CLR(data->rcvsock, &fds);
		FD_CLR(data->rcvsock, &err);
		return (ETIMEDOUT);
	}
}

int	traceroute_output(t_packet	*packet, int ttl, int probe, int status, t_data *data)
{
	char	src[INET_ADDRSTRLEN];
	char	dst[INET_ADDRSTRLEN];

	if (status == SUCCESS) {
		if (!inet_ntop(AF_INET, &packet->hdr.reply.ip.ip_src, src, INET_ADDRSTRLEN))
			ft_perror(ft_strerror(errno), "inet_ntop");
		if (!inet_ntop(AF_INET, &packet->hdr.reply.ip.ip_dst, dst, INET_ADDRSTRLEN))
			ft_perror(ft_strerror(errno), "inet_ntop");
		if (probe == 0)
			dprintf(STDOUT_FILENO, "%2d  %s (%s)  %s", ttl, src, src, "ok ");
		else
			dprintf(STDOUT_FILENO, " ok ");
		if (probe == 2 && !ft_strcmp(src, data->ip))
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
	t_packet		*packet;
	char			rcv_packet[data->packetlen];
	struct timeval	tv;
	int				seq;
	int				ret;

	printf("ft_traceroute to %s (%s), %d hops max, %d bytes packets\n",
	data->host, data->ip, NHOPS_MAX, PACKET_LEN);
	packet = (t_packet *)calloc(1, data->packetlen);
	if (!packet)
		fatal_error(ENOMEM, NULL, 0, data);
	seq = 1;
	for (int ttl = START_TLL; ttl <= NHOPS_MAX; ttl++)
	{

		if (setsockopt(data->sndsock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1)
			fatal_error(errno, "setsockopt", 0, data);
		for (int probe = 0; probe < NPROBES; probe++)
		{
			gettimeofday(&tv, NULL);
			send_probe(data, packet, seq, ttl);
			ret = recv_reply(data, rcv_packet);
#ifndef DEBUG
			if (traceroute_output((t_packet *)rcv_packet, ttl, probe, ret, data))
			{
				printf("\n");
				free(packet);
				return ;
			}
#else
			if (ret != SUCCESS)
				printf("%s[timeout]%s\n", S_RED, S_NONE);
#endif
			++seq;
		}
		printf("\n");
	}
	free(packet);
}
