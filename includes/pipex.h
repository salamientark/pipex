/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 10:07:15 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/05 17:02:27 by dbaladro         ###   ########.fr       */
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

// Bash error code
// # define ENXIO "No such file or directory"

// HERE_DOC
int     create_here_doc(char *limiter);
int     pipe_here_doc(int size, char **av, char **env);

// COMMAND
char    **parse_command(char *cmd, char **env);

// ERROR
void    print_error(char *msg, char *error);
void    print_error_cmd(char *msg, char *arg, char *error);
void    exit_error_msg(char *msg, char *msg_2);
void    exit_error_cmd(char *msg, char *arg, char *error);

// PATH
char	**get_path(char **env);

void    free_str_tab(char ***str_tab_ptr);

#endif