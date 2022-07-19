/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 16:31:36 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/16 18:46:53 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

t_ping_data	g_data;

int	main(int ac, char **av)
{
	if (ac == 1)
		fatal_error(EP_NODATA, NULL, 0);
	g_data = init_ping_data();
	parser(av + 1);
	setup_address();
	setup_socket();
	signal(SIGINT, handle_signal);
	signal(SIGALRM, handle_signal);
	ping();
	return (0);
}
