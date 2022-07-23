/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 14:21:29 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/23 14:48:36 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

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
	}
    else {
        data->status |= RTIMEDOUT;
    }
	FD_CLR(data->rcvsock, &fds);
    return (0);
}

bool	check_reply(t_data *data, void *packet)
{
	t_header	*hdr;

	if (data->status & RTIMEDOUT) {
		data->status &= ~RWAIT;
		return (true);	
	}
	hdr = (t_header *)packet;
	if (hdr->ip.ip_id != data->id)
		return (false);
	data->status &= ~RWAIT;
	return (true);	
}
