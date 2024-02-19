/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 10:03:43 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/19 08:46:18 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

/*
    Error msg are bufferised to avoid mixed output
*/

void	print_error(char *msg, char *error)
{
	int		msg_len;
	int		error_len;
	char	buffer[512];

	msg_len = ft_strlen(msg);
	error_len = ft_strlen(error);
	ft_memcpy((void *)buffer, msg, msg_len);
	ft_memcpy((void *)buffer + msg_len, error, error_len);
	buffer[msg_len + error_len] = '\n';
	write(2, buffer, msg_len + error_len + 1);
}

void	print_error_cmd(char *msg, char *arg, char *error)
{
	int		msg_len;
	int		arg_len;
	int		error_len;
	char	buffer[512];

	msg_len = ft_strlen(msg);
	arg_len = ft_strlen(arg);
	error_len = ft_strlen(error);
	ft_memcpy((void *)buffer, msg, msg_len);
	ft_memcpy((void *)buffer + msg_len, arg, arg_len);
	ft_memcpy((void *)buffer + msg_len + arg_len, ": ", 2);
	ft_memcpy((void *)buffer + msg_len + arg_len + 2, error, error_len);
	buffer[msg_len + arg_len + error_len + 2] = '\n';
	write(2, buffer, msg_len + error_len + arg_len + 3);
}

void	exit_error_cmd(char *msg, char *arg, char *error)
{
	int		msg_len;
	int		arg_len;
	int		error_len;
	char	buffer[512];

	msg_len = ft_strlen(msg);
	arg_len = ft_strlen(arg);
	error_len = ft_strlen(error);
	ft_memcpy((void *)buffer, msg, msg_len);
	ft_memcpy((void *)buffer + msg_len, arg, arg_len);
	ft_memcpy((void *)buffer + msg_len + arg_len, ": ", 2);
	ft_memcpy((void *)buffer + msg_len + arg_len + 2, error, error_len);
	buffer[msg_len + arg_len + error_len + 2] = '\n';
	write(2, buffer, msg_len + error_len + arg_len + 3);
	exit(1);
}

void	exit_error_msg(char *msg, char *error)
{
	int		msg_len;
	int		error_len;
	char	buffer[512];

	msg_len = ft_strlen(msg);
	error_len = ft_strlen(error);
	ft_memcpy((void *)buffer, msg, msg_len);
	ft_memcpy((void *)buffer + msg_len, error, error_len);
	buffer[msg_len + error_len] = '\n';
	write(2, buffer, msg_len + error_len + 1);
	exit(1);
}

// void	print_error_cmd(char *msg, char *arg, char *error)
// {
//	 ft_putstr_fd(msg, 2);
//	 ft_putstr_fd(arg, 2);
//	 ft_putstr_fd(": ", 2);
//	 ft_putendl_fd(error, 2);
// }

// void	exit_error_cmd(char *msg, char *arg, char *error)
// {
//	 ft_putstr_fd(msg, 2);
//	 if (arg)
//	 {
//		 ft_putstr_fd(arg, 2);
//		 ft_putstr_fd(": ", 2);
//	 }
//	 if (error)
//		 ft_putstr_fd(error, 2);
//	 ft_putendl_fd("", 2);
//	 exit(1);
// }

// void	exit_error_msg(char *msg, char *msg_2)
// {
//	 ft_putstr_fd(msg, 2);
//	 if (msg_2)
//		 ft_putstr_fd(msg_2, 2);
//	 ft_putendl_fd("", 2);
//	 exit(1);
// }
