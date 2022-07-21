/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 13:44:42 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/21 10:39:56 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_FT
# define FT_TRACEROUTE_FT

# include "defs.h"

typedef struct s_packet
{
	struct ip	ip;
	union u_head
	{
		struct udphdr   udp;
		struct icmp		icmp;
	}	hdr;
	u_char          seq;
	u_char          ttl;
	struct timeval  tv;
}	t_packet;

typedef struct s_data
{
    char            *host;
    char            ip[INET_ADDRSTRLEN];
    int             packetlen;
	struct sockaddr	sockaddr;
	int				sndsock;
	int				rcvsock;
	uint8_t			pid;
}   t_data;

char	*ft_strerror(int error);
int		ft_perror(const char *error, const char *fct);
int		fatal_error(int error, const char *arg, const int len, t_data *data);
t_data	*parser(char **arg, t_data *data);
char	*icmp_strerror(int error);

int		ft_strcmp(const char *s1, const char *s2);
int		ft_isnumber(const char *str);
char	*ft_strdup(const char *s1);
size_t	ft_strlen(const char *str);

void	clear_data(t_data *data);
int		print_usage(void);
void    traceroute(t_data *data);

t_data		init_data(void);
t_data		*setup_address(t_data *data);
t_data		*setup_socket(t_data *data);
t_packet	*setup_packet(t_data *data, t_packet *packet, int seq, int ttl);
int			ft_wait(struct timeval start_time, size_t nb_sec);

void	debug_icmp(struct icmp icmphdr);
void	debug_packet(t_packet packet);
void	debug_ip(struct ip iphdr);

#endif
