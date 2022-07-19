/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 17:45:31 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/18 14:12:19 by adbenoit         ###   ########.fr       */
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
# include <stdbool.h>
# include <sys/time.h>
# include <math.h>
# include "error.h"

# define NONE 0

/*
** -- Flags --
*/
# define NB_FLAGS 5
# define FLAGS "hvcqt"
# define F_HELP 0x80
# define F_VERBOSE 0x40
# define F_COUNT 0x20
# define F_QUIET 0x10
# define F_TTL 0x8
# define FLAG_ISSET(flag_code) (g_data.flag.isset & flag_code)

/*
** -- Transmission status --
*/
# define WAIT_REPLY 0b0001
# define RTIMEDOUT 0b0010
# define STOP_SENDING 0b0100
# define STATUS_ISSET(status_code) (g_data.status & status_code)

/*
** -- Request infos --
*/
# define TIMEOUT 1
# define TIME_INTERVAL 1
# define HEADER_SIZE 28
# define PACKET_SIZE 56
# define DEFAULT_TTL 64
# define VERSION 4

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
# define S_PACKET g_data.request_packet.echo.request
# define R_PACKET g_data.reply_packet.echo.reply

#endif
