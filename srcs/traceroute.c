/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traceroute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 15:39:49 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/21 23:50:06 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	send_probe(t_data *data, char *packet, int seq, int ttl)
{
	((t_probe_packet *)packet)->ttl = ttl;
	((t_probe_packet *)packet)->seq = ++seq;
	gettimeofday(&((t_probe_packet *)packet)->tv, NULL);
	((struct sockaddr_in *)data->addrinfo->ai_addr)->sin_port = htons(UDP_PORT + seq);
	sendto(data->sndsock, packet, sizeof(packet), 0, data->addrinfo->ai_addr,
		data->addrinfo->ai_addrlen);
}

int	recv_reply(t_data *data, char *dest)
{
	fd_set			fds;
	int				ret;
	socklen_t		addrlen;
	struct timeval	timeout;
	
	ret = 0;
	timeout.tv_sec = TIMEOUT;
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
		FD_CLR(data->rcvsock, &fds);
		return (SUCCESS);
	}
	FD_CLR(data->rcvsock, &fds);
	return (ETIMEDOUT);
}

int	traceroute_output(t_header	*packet, int ttl, int probe, t_data *data, double time)
{
	char		src[INET_ADDRSTRLEN];
	static char tmp[INET_ADDRSTRLEN];

	if (data->status == SUCCESS) {
		if (!inet_ntop(AF_INET, &packet->ip.ip_src, src, INET_ADDRSTRLEN))
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
	char			*probe_packet;
	char			rcv_packet[data->packetlen];
	struct timeval	after;
	int				seq;

	printf("ft_traceroute to %s (%s), %d hops max, %d bytes packets\n",
	data->host, data->ip, NHOPS_MAX, PACKET_LEN);
	probe_packet = calloc(1, data->packetlen);
	if (!probe_packet)
		fatal_error(ENOMEM, NULL, 0, data);
	seq = 0;
	for (int ttl = START_TLL; ttl <= NHOPS_MAX; ttl++)
	{
		bzero(data->lastip, INET_ADDRSTRLEN);
		if (setsockopt(data->sndsock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1)
			fatal_error(errno, "setsockopt", 0, data);
		for (int probe = 0; probe < NPROBES; probe++)
		{
			send_probe(data, probe_packet, seq, ttl);
			data->status = recv_reply(data, rcv_packet);
			gettimeofday(&after, NULL);
			if (traceroute_output((t_header *)rcv_packet, ttl, probe, data,
					tv_to_ms(after) - tv_to_ms(((t_probe_packet *)probe_packet)->tv)))
			{
				printf("\n");
				free(probe_packet);
				return ;
			}
		}
		printf("\n");
	}
	free(probe_packet);
}
