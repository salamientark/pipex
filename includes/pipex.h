/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 10:07:15 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/16 21:52:04 by dbaladro         ###   ########.fr       */
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

# define HERE_DOC_MISUSE "pipex here_doc [limiter] [cmd 1]...[cmd n] [outfile]"
# define PIPEX_MISUSE "pipex [infile] [cmd 1] .... [cmd n] [outifle]"
# define HERE_DOC "/tmp/.here_doc"

typedef struct s_pipex
{
    int     here_doc;
    char    *limiter;
    char    *infile;
    char    *outfile;
    char    **env;
}               t_pipex;


// COMMAND
char	**parse_command(char *cmd, char **env);

// ERROR
void	print_error(char *msg, char *error);
void	print_error_cmd(char *msg, char *arg, char *error);
void	exit_error_msg(char *msg, char *msg_2);
void	exit_error_cmd(char *msg, char *arg, char *error);

// PATH
char	**get_path(char **env);

// PIPEX_UTILS
void	free_str_tab(char ***str_tab_ptr);
int		ft_open(char *filename, int open_mode);
int		here_doc(char *limiter);
void	ft_exec(char *cmd, char **env);
void	ft_redirect_to(char *filename, int open_mode, char *cmd, char **env);

// PIPEX
char	**get_env(void);

#endif
