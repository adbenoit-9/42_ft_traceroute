/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 17:27:25 by adbenoit          #+#    #+#             */
/*   Updated: 2022/07/24 17:15:26 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

#ifdef OS

char	*ft_strerror(int error)
{
	char	*msg_err[] = {EPERM_MSG, ENOENT_MSG, ESRCH_MSG, EINTR_MSG,
		ET_FAMILY_MSG, ENXIO_MSG, E2BIG_MSG, NONAME_MSG, EBADF_MSG, ECHILD_MSG,
		EDEADLK_MSG, ENOMEM_MSG, EACCES_MSG, EFAULT_MSG, EBUSY_MSG, EEXIST_MSG,
		EXDEV_MSG, ENODEV_MSG, ENOTDIR_MSG, EISDIR_MSG, EINVAL_MSG, ENFILE_MSG,
		EMFILE_MSG, ENOTTY_MSG, ETXTBSY_MSG, EFBIG_MSG, ENOSPC_MSG, ESPIPE_MSG,
		EROFS_MSG, EMLINK_MSG, EPIPE_MSG, EDOM_MSG, ERANGE_MSG, EAGAIN_MSG,
		EWOULDBLOCK_MSG, EINPROGRESS_MSG, EALREADY_MSG, ENOTSOCK_MSG,
		EDESTADDRREQ_MSG, EMSGSIZE_MSG, EPROTOTYPE_MSG, ENOPROTOOPT_MSG,
		EPROTONOSUPPORT_MSG, ESOCKTNOSUPPORT_MSG, ENOTSUP_MSG,
		EPFNOSUPPORT_MSG, EAFNOSUPPORT_MSG, EADDRINUSE_MSG, EADDRNOTAVAIL_MSG,
		ENETDOWN_MSG, ENETUNREACH_MSG, ENETRESET_MSG, ECONNABORTED_MSG,
		ECONNRESET_MSG, ENOBUFS_MSG, EISCONN_MSG, ENOTCONN_MSG, ESHUTDOWN_MSG,
		ETOOMANYREFS_MSG, ETIMEDOUT_MSG, ECONNREFUSED_MSG, ELOOP_MSG,
		ENAMETOOLONG_MSG, EHOSTDOWN_MSG, EHOSTUNREACH_MSG, ENOTEMPTY_MSG,
		EPROCLIM_MSG, EUSERS_MSG, EDQUOT_MSG, ESTALE_MSG, EREMOTE_MSG,
		EBADRPC_MSG, ERPCMISMATCH_MSG, EPROGUNAVAIL_MSG, EPROGMISMATCH_MSG,
		EPROCUNAVAIL_MSG, ENOLCK_MSG, ENOSYS_MSG, EFTYPE_MSG, EAUTH_MSG,
		ENEEDAUTH_MSG, EPWROFF_MSG, EDEVERR_MSG, EOVERFLOW_MSG, EBADEXEC_MSG,
		EBADARCH_MSG, ESHLIBVERS_MSG, EBADMACHO_MSG, ECANCELED_MSG, EIDRM_MSG,
		ENOMSG_MSG, EILSEQ_MSG, ENOATTR_MSG, EBADMSG_MSG, EMULTIHOP_MSG,
		ENODATA_MSG, ENOLINK_MSG, ENOSR_MSG, ENOSTR_MSG, EPROTO_MSG, ETIME_MSG,
		EOPNOTSUPP_MSG, ENOPOLICY_MSG, ENOTRECOVERABLE_MSG, EOWNERDEAD_MSG,
		EQFULL_MSG};

	if (error > 0 && error < ELAST)
		return (msg_err[error - 1]);
	return (NULL);
}

#else

char	*ft_strerror(int error)
{
	char	*msg_err[] = {EPERM_MSG, ENOENT_MSG, ESRCH_MSG, EINTR_MSG,
		ET_FAMILY_MSG, ENXIO_MSG, E2BIG_MSG, NONAME_MSG, EBADF_MSG, ECHILD_MSG,
		EAGAIN_MSG, ENOMEM_MSG, EACCES_MSG, EFAULT_MSG, ENOTBLK_MSG, EBUSY_MSG,
		EEXIST_MSG, EXDEV_MSG, ENODEV_MSG, ENOTDIR_MSG, EISDIR_MSG, EINVAL_MSG,
		ENFILE_MSG, EMFILE_MSG, ENOTTY_MSG, ETXTBSY_MSG, EFBIG_MSG, ENOSPC_MSG,
		ESPIPE_MSG, EROFS_MSG, EMLINK_MSG, EPIPE_MSG, EDOM_MSG, ERANGE_MSG,
		EDEADLK_MSG, ENAMETOOLONG_MSG, ENOLCK_MSG, ENOSYS_MSG, ENOTEMPTY_MSG,
		ELOOP_MSG, EWOULDBLOCK_MSG, ENOMSG_MSG, EIDRM_MSG, ECHRNG_MSG,
		EL2NSYNC_MSG, EL3HLT_MSG, EL3RST_MSG, ELNRNG_MSG, EUNATCH_MSG,
		ENOCSI_MSG, EL2HLT_MSG, EBADE_MSG, EBADR_MSG, EXFULL_MSG, ENOANO_MSG,
		EBADRQC_MSG, EBADSLT_MSG, EDEADLOCK_MSG, EBFONT_MSG, ENOSTR_MSG,
		ENODATA_MSG, ETIME_MSG, ENOSR_MSG, ENONET_MSG, ENOPKG_MSG, EREMOTE_MSG,
		ENOLINK_MSG, EADV_MSG, ESRMNT_MSG, ECOMM_MSG, EPROTO_MSG, EMULTIHOP_MSG,
		EDOTDOT_MSG, EBADMSG_MSG, EOVERFLOW_MSG, ENOTUNIQ_MSG, EBADFD_MSG,
		EREMCHG_MSG, ELIBACC_MSG, ELIBBAD_MSG, ELIBSCN_MSG, ELIBMAX_MSG,
		ELIBEXEC_MSG, EILSEQ_MSG, ERESTART_MSG, ESTRPIPE_MSG, EUSERS_MSG,
		ENOTSOCK_MSG, EDESTADDRREQ_MSG, EMSGSIZE_MSG, EPROTOTYPE_MSG,
		ENOPROTOOPT_MSG, EPROTONOSUPPORT_MSG, ESOCKTNOSUPPORT_MSG,
		EOPNOTSUPP_MSG, EPFNOSUPPORT_MSG, EAFNOSUPPORT_MSG, EADDRINUSE_MSG,
		EADDRNOTAVAIL_MSG, ENETDOWN_MSG, ENETUNREACH_MSG, ENETRESET_MSG,
		ECONNABORTED_MSG, ECONNRESET_MSG, ENOBUFS_MSG, EISCONN_MSG,
		ENOTCONN_MSG, ESHUTDOWN_MSG, ETOOMANYREFS_MSG, ETIMEDOUT_MSG,
		ECONNREFUSED_MSG, EHOSTDOWN_MSG, EHOSTUNREACH_MSG, EALREADY_MSG,
		EINPROGRESS_MSG, ESTALE_MSG, EUCLEAN_MSG, ENOTNAM_MSG, ENAVAIL_MSG,
		EISNAM_MSG, EREMOTEIO_MSG};

	if (error > 0 && error < ELAST)
		return (msg_err[error - 1]);
	return (NULL);
}
#endif

int	ft_perror(const char *error, const char *fct)
{
	if (!error && fct)
		dprintf(STDERR_FILENO, "ft_traceroute: %s: unknown error\n", fct);
	else if (!error)
		dprintf(STDERR_FILENO, "ft_traceroute: unknown error\n");
	else if (fct)
		dprintf(STDERR_FILENO, "ft_traceroute: %s: %s\n", fct, error);
	else
		dprintf(STDERR_FILENO, "ft_traceroute: %s\n", error);
	return (0);
}

void	exit_error(char *str, t_data *data, int type)
{
	dprintf(STDERR_FILENO, "%s\n", str);
	clear_data(data);
	exit(type);
}

int	fatal_error(int error, const char *arg, const int argc, t_data *data)
{
	int			status;

	switch (error)
	{
		case ET_BADVAL:
			dprintf(STDERR_FILENO, ET_BADVAL_MSG, arg, argc);
			break ;
		case ET_NOHOST:
			dprintf(STDERR_FILENO, ET_NOHOST_MSG);
			break ;
		case ET_BADOPT:
			dprintf(STDERR_FILENO, ET_BADOPT_MSG, arg, argc);
			break ;
		case ET_MULHOST:
			print_usage();
			break ;
		default :
			ft_perror(ft_strerror(error), arg);
			break ;
	}
	if (data)
		clear_data(data);
	status = (error >= ET_NOHOST) ? USAGE_ERR : ERROR;
	exit(status);
}

char	*icmp_strerror(int error)
{
	if (error == ICMP_UNREACH)
		return ("Dest unreachable");
	else if (error == ICMP_SOURCEQUENCH)
		return ("Packet lost, slow down");
	else if (error == ICMP_REDIRECT)
		return ("Shorter route");
	else if (error == ICMP_TIMXCEED)
		return ("Time to live exceeded");
	else if (error == ICMP_PARAMPROB)
		return ("IP header bad");
	return (NULL);
}
