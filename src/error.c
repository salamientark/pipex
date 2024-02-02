/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 10:03:43 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/02 09:27:40 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void    print_error(char *msg, int error)
{
    ft_putstr_fd(msg, 2);
    if (error >= 0)
        ft_putstr_fd(strerror(error), 2);
    ft_putendl_fd("", 2);
}

void    exit_error_msg(char *msg, int error)
{
    ft_putstr_fd(msg, 2);
    if (errno >= 0)
        ft_putstr_fd(strerror(error), 2);
    ft_putendl_fd("", 2);
    exit(1);
}