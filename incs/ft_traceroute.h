/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 13:44:42 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/24 18:43:35 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_FT
# define FT_TRACEROUTE_FT

# include "defs.h"

typedef	struct s_header
{
	struct ip		ip;
	struct icmp		icmp;
	struct udphdr   udp;
}				t_header;

typedef struct s_datagram
{
	u_char          seq;
	u_char          ttl;
	struct timeval  tv;
}	t_datagram;

typedef struct s_data
{
    char            *host;
    char            ip[INET_ADDRSTRLEN];
    int             packetlen;
	struct addrinfo	*addrinfo;
	struct sockaddr	sockaddr;
	int				sndsock;
	int				rcvsock;
	uint8_t			id;
	short			status;
	int				first_ttl;
	int				max_ttl;
	int				nprobes;
}   t_data;

char	*ft_strerror(int error);
int		ft_perror(const char *error, const char *fct);
int		fatal_error(int error, const char *arg, const int argc, t_data *data);
t_data	*parser(char **arg, t_data *data);
char	*icmp_strerror(int error);
void	exit_error(char *str, t_data *data, int type);

int		ft_strcmp(const char *s1, const char *s2);
int		ft_isnumber(const char *str);
char	*ft_strdup(const char *s1);
size_t	ft_strlen(const char *str);

void	clear_data(t_data *data);
int		print_usage(void);
void    traceroute(t_data *data);
int		recv_packet(t_data *data, char *dest);
bool	check_packet(t_data *data, void *packet, int seq);
void	send_probe(t_data *data, char *packet, int seq, int ttl);

t_data		init_data(void);
int			setup_host(char **arg, int i, t_data *data);
t_data		*setup_socket(t_data *data);
int			ft_wait(struct timeval start_time, size_t nb_sec);
double		tv_to_ms(struct timeval t);

void	debug_icmp(struct icmp icmphdr);
void	debug_packet(t_datagram packet);
void	debug_ip(struct ip iphdr);
void	debug_udp(struct udphdr udp);

#endif
