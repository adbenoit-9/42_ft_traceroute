/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 16:31:36 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/19 14:42:03 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

t_data	g_data;

void	clear_data(t_data *data)
{
	free(data->host);
}

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
		fatal_error(EP_NODATA, NULL, 0);
	data = parser(av + 1);
	g_data = data;
	return (0);
}
