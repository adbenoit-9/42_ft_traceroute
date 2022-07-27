/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 11:54:29 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/27 13:34:15 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	debug_ip(struct ip iphdr)
{
	printf("-- IP HEADER (size %ld) --\n", sizeof(iphdr));
	printf("ip_hl: %d\n", iphdr.ip_hl);
	printf("ip_v: %d\n", iphdr.ip_v);
	printf("tos: %d\n", iphdr.ip_tos);
	printf("len: %d\n", iphdr.ip_len);
	printf("id: %d\n", iphdr.ip_id);
	printf("off: %d\n", iphdr.ip_off);
	printf("ttl: %d\n", iphdr.ip_ttl);
	printf("protocol: %d\n", iphdr.ip_p);
	printf("sum: %d\n", iphdr.ip_sum);
}

void	debug_icmp(struct icmp icmphdr)
{
	printf("-- ICMP HEADER (size %ld) --\n", sizeof(icmphdr));
	printf("type: %d\n", icmphdr.icmp_type);
	printf("code: %d\n", icmphdr.icmp_code);
	printf("identifier: %d\n", icmphdr.icmp_id);
	printf("sequence: %d\n", icmphdr.icmp_seq);
	printf("checksum: %d\n", icmphdr.icmp_cksum);
}

void	debug_udp(struct udphdr udp)
{
	printf("-- UDP HEADER (size %ld) --\n", sizeof(struct udphdr));
	printf("port dest: %d\n", udp.uh_dport);
	printf("port src: %d\n", udp.uh_sport);
	printf("len: %d\n", udp.uh_ulen);
	printf("sum: %d\n", udp.uh_sum);
}
