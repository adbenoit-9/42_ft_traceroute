/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 17:45:31 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/25 20:33:54 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFS_H
# define DEFS_H

/*
** -- Includes --
*/
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sysexits.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <signal.h>
# include <arpa/inet.h>
# include <ctype.h>
# include <netinet/ip_icmp.h>
# include <netinet/udp.h>
# include <stdbool.h>
# include <sys/time.h>
# include <math.h>
# include <sys/select.h>
# include "error.h"

# define DEBUG_LVL 1
# define NONE 0
# define PACKET_LEN sizeof(struct ip) + 40
# define MAXPACKET 32768
# define UDP_PORT 33434
# define FIRST_TTL 1
# define MAX_TTL 30
# define WAITTIME 5.0
# define NQUERIES 3
# define SQUERIES 1
# define RECVLEN sizeof(t_header)
# define SENDLEN data->packetlen - sizeof(struct ip)

/*
** -- flags --
*/
# define NB_FLAGS 3
# define FLAG_LST {"f", "m", "q", "-first", "-max-hops", "-queries"}
# define FLAG_F 0
# define FLAG_M 1
# define FLAG_Q 2

/*
** -- Tarnsmission status --
*/
# define RWAIT 0b1000
# define RTIMEDOUT 0b0100
# define END 0b0010
# define PSENDING 0b0001

/*
** -- Exit codes --
*/
# define SUCCESS 0
# define USAGE_ERR 1
# define ERROR 2

/*
** -- Color codes --
*/
# define S_NONE    "\033[0m"
# define S_RED     "\033[1;31m"
# define S_GREEN   "\033[1;32m"
# define S_YELLOW  "\033[1;33m"
# define S_BLUE    "\033[1;34m"
# define S_PURPLE  "\033[1;35m"
# define S_CYAN    "\033[1;36m"

# define LLONG_MAX 9223372036854775807

#endif
