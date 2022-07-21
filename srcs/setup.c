/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 18:45:13 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/21 15:02:45 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

t_data	init_data(void)
{
	t_data	data;

	data.host = NULL;
	data.addrinfo = NULL;
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
	if (data->addrinfo)
		freeaddrinfo(data->addrinfo);
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
	data->addrinfo = res;
	return (data);
}

t_data	*setup_socket(t_data *data)
{
	struct sockaddr_in	from;
	
	bzero(&from, sizeof(from));
	from.sin_family = AF_INET;
	from.sin_port = htons((data->pid & 0xffff) | 0x8000);
	data->sndsock = socket(AF_INET, SOCK_DGRAM, 0);
	if (data->sndsock == -1)
		fatal_error(errno, "socket", 0, data);
	if (bind(data->sndsock, (struct sockaddr *)&from, sizeof(from)) == -1)
		fatal_error(errno, NULL, 0, data);
	data->rcvsock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (data->rcvsock == -1)
		fatal_error(errno, "socket", 0, data);
	return (data);
}
