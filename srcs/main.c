/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 17:38:13 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/24 18:43:27 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int		print_usage(void)
{
	printf("Usage:\n  ./ft_traceroute [option] host [packetlen]\n");
	printf("Options:\n");
	printf("  -f first_ttl  --first=first_ttl\n");
	printf("                              Start from the first_ttl hop (instead from 1)\n");
	printf("  -m max_ttl  --max-hops=max_ttl\n");
	printf("                              Set the max number of hops (max TTL to be\n");
    printf("                              reached). Default is 30\n");
	printf("  -q nqueries  --queries=nqueries\n");
	printf("                              Set the number of probes per each hop. Default is 3\n");
	printf("  --help                      Read this help and exit\n");
	printf("\nAguments:\n");
	printf("+     host          The host to traceroute to\n");
	printf("      packetlen     The full packet length (default is the length of an IP\n");
	printf("                    header plus 40). Can be ignored or increased to a minimal\n");
	printf("                    allowed value\n");
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac == 1)
		exit(print_usage());
	data = init_data();
	parser(av + 1, &data);
	setup_socket(&data);
	traceroute(&data);
	clear_data(&data);
	return (0);
}
