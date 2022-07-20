/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 17:21:42 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/20 17:41:20 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

double	tv_to_ms(struct timeval t)
{
	return (t.tv_sec * 1000. + t.tv_usec / 1000.);
}


int	ft_wait(struct timeval start_time, size_t nb_sec)
{
	struct timeval	current_time;
	double			end_ms;
	double			current_ms;

	start_time.tv_sec = start_time.tv_sec + nb_sec;
	end_ms = tv_to_ms(start_time);
	if (gettimeofday(&current_time, NULL) == -1)
		return (false);
	current_ms = tv_to_ms(current_time);
	while (current_ms < end_ms)
	{
		if (gettimeofday(&current_time, NULL) == -1)
			return (false);
		current_ms = tv_to_ms(current_time);
	}
	return (true);
}
