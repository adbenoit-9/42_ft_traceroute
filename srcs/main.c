/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 15:43:07 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/21 10:32:13 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int		print_usage(void)
{
	printf("Usage:\n  ./ft_traceroute [option] host [packetlen]\n");
	printf("Options:\n");
	printf("  -f first_tt           Start from the first_ttl hop (instead from 1)\n");
	printf("  -m max_ttl            Set the max number of hops (max TTL to be\n");
    printf("                        reached). Default is 30\n");
	printf("  -q nqueries           Set the number of probes per each hop. Default is 3\n");
	printf("  --help                Read this help and exit\n");
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
		fatal_error(EP_NODATA, NULL, 0, NULL);
	data = init_data();
	parser(av + 1, &data);
	setup_address(&data);
	setup_socket(&data);
	traceroute(&data);
	clear_data(&data);
	return (0);
}
