/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 10:03:43 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/01 10:47:36 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void    print_error(char *msg, int error)
{
    ft_putstr_fd(msg, 2);
    ft_putstr_fd(":", 2);
    ft_putendl_fd(strerror(error), 2);
}