/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 17:45:31 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/21 12:36:37 by adbenoit         ###   ########.fr       */
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

# define DEBUG_LVL 2
# define NONE 0
# define PACKET_LEN 52 // Mac: 52
# define MAXPACKET 32768
# define NPROBES 3
# define NHOPS_MAX 64 // Mac: net.inet.ip.ttl | linux: 30 ?
# define TIMEOUT 5 // ou 3 ?
# define UDP_PORT 33434
# define START_TLL 1

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
