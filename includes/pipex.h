/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 10:07:15 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/01 10:47:25 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <fcntl.h>
# include "../libft/includes/libft.h"

// HERE_DOC
int     create_here_doc(char *limiter);
int     pipe_here_doc(int size, char **av);

// COMMAND
char    **parse_command(char *cmd);

// ERROR
void    print_error(char *msg, int error);

#endif