/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 10:07:15 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/19 08:09:01 by madlab           ###   ########.fr       */
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
# define CMD_NOT_FOUND " : Command not found"

/*
	Redirect_flag
*/
# define READ_FROM_FILE 0
# define HERE_DOC 1
# define PIPE 3
# define WRITE_TO_FILE_TRUNC 4
# define WRITE_TO_FILE_APPEND 5

typedef struct s_pipex
{
	int		here_doc;
	char	*limiter;
	char	*infile;
	char	*outfile;
	char	**env;
}				t_pipex;

// COMMAND
char	**parse_command(char *cmd, char **env);

// PATH
char	**get_path(char **env);

// ERROR
void	print_error(char *msg, char *error);
void	print_error_cmd(char *msg, char *arg, char *error);
void	exit_error_msg(char *msg, char *msg_2);
void	exit_error_cmd(char *msg, char *arg, char *error);

// PIPEX_UTILS
void	free_str_tab(char ***str_tab_ptr);
void	pipe_here_doc(char *limiter);
void	redirect_io(int pipe_fd[2], t_pipex data, int redirect_flag);

#endif
