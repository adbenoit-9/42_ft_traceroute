/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 18:45:13 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/20 10:55:13 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

t_data	init_data(void)
{
	t_data	data;

	data.host = NULL;
	data.packetlen = -1;
	bzero(data.ip, INET_ADDRSTRLEN);
	data.sndsock = -1;
	data.rcvsock = -1;
	data.pid = getpid();
	bzero(&data.sockaddr, sizeof(data.sockaddr));
	return (data);
}

void	clear_data(t_data *data)
{
	free(data->host);
	if (data->sndsock != -1)
		close(data->sndsock);
	if (data->rcvsock != -1)
		close(data->rcvsock);
}

t_data	*setup_address(t_data *data)
{
	struct addrinfo	hints;
	struct addrinfo	*res;
	struct in_addr	src;
	int				ret;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = 0;
	ret = getaddrinfo(data->host, NULL, &hints, &res);
	if (ret == EAI_FAMILY)
		fatal_error(EP_FAMILY, data->host, 0, data);
	else if (ret == EAI_NODATA)
		fatal_error(EP_NODATA, data->host, 0, data);
	else if (ret != 0)
		fatal_error(EP_NONAME, data->host, 0, data);
	memcpy(&data->sockaddr, res->ai_addr, sizeof(struct sockaddr));
	src = ((struct sockaddr_in *)res->ai_addr)->sin_addr;
	if (!inet_ntop(AF_INET, &src, data->ip, INET_ADDRSTRLEN))
		fatal_error(errno, data->host, 0, data);
	freeaddrinfo(res);
	return (data);
}

t_data	*setup_socket(t_data *data)
{
	data->sndsock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (data->sndsock == -1)
		fatal_error(errno, "socket", 0, data);
	data->rcvsock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (data->rcvsock == -1)
		fatal_error(errno, "socket", 0, data);
	return (data);
}