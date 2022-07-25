/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 18:45:13 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/25 18:37:25 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

t_data	init_data(void)
{
	t_data	data;

	data.host = NULL;
	data.addrinfo = NULL;
	data.packetlen = PACKET_LEN;
	bzero(data.ip, INET_ADDRSTRLEN);
	data.sndsock = -1;
	data.rcvsock = -1;
	data.status = NONE;
	data.first_ttl = FIRST_TTL;
	data.max_ttl = MAX_TTL;
	data.nqueries = NQUERIES;
	data.id = (getpid() & 0xffff) | 0x8000;
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

int	setup_host(char **arg, int i, t_data *data)
{
	struct addrinfo	hints;
	struct addrinfo	*res;
	struct in_addr	src;
	int				ret;

	data->host = ft_strdup(arg[i]);
	if (!data->host)
		fatal_error(ENOMEM, NULL, 0, data);
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = 0;
	ret = getaddrinfo(data->host, NULL, &hints, &res);
	if (ret == EAI_FAMILY)
		fatal_error(ET_FAMILY, arg[i], i + 1, data);
	else if (ret == EAI_NODATA)
		fatal_error(ET_NOHOST, arg[i], i + 1, data);
	else if (ret != 0)
		fatal_error(ET_NONAME, arg[i], i + 1, data);
	memcpy(&data->sockaddr, res->ai_addr, sizeof(struct sockaddr));
	src = ((struct sockaddr_in *)res->ai_addr)->sin_addr;
	if (!inet_ntop(AF_INET, &src, data->ip, INET_ADDRSTRLEN))
		fatal_error(errno, data->host, 0, data);
	data->addrinfo = res;
	return (SUCCESS);
}

t_data	*setup_socket(t_data *data)
{
	struct sockaddr_in	from;
	
	bzero(&from, sizeof(from));
	from.sin_family = AF_INET;
	from.sin_port = htons(data->id);
	data->sndsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (data->sndsock == -1)
		fatal_error(errno, "socket", 0, data);
	if (bind(data->sndsock, (struct sockaddr *)&from, sizeof(from)) == -1)
		fatal_error(errno, NULL, 0, data);
	data->rcvsock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (data->rcvsock == -1)
		fatal_error(errno, "socket", 0, data);
	return (data);
}
