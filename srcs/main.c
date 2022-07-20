/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 15:43:07 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/20 10:52:30 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int		print_usage(void)
{
	printf("\nUsage:\n ./ft_traceroute [option] host [packetlen]\n");
	printf("\nOptions:\n");
	printf("--help              print help and exit\n");
	printf("\n");
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
	setup_socket(&data); // il faut 2, une pour send et une pour recv (pas meme protocol)
	traceroute(&data);
	return (0);
}
