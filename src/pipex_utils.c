/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 08:04:31 by dbaladro          #+#    #+#             */
/*   Updated: 2024/04/29 01:31:23 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

/*
	free char ** and set every value to NULL
*/
void	free_str_tab(char ***str_tab_ptr)
{
	int	index;

	index = 0;
	while ((*str_tab_ptr)[index] != NULL)
	{
		free((*str_tab_ptr)[index]);
		(*str_tab_ptr)[index] = NULL;
		index++;
	}
	free(*str_tab_ptr);
	*str_tab_ptr = NULL;
}

/*
	Write here_doc into pipe
*/
static void	write_here_doc(int pipe_fd[2], char *limiter)
{
	int		limiter_len;
	char	*line;

	limiter_len = ft_strlen(limiter);
	close(pipe_fd[0]);
	write(STDOUT_FILENO, "here_doc > ", 11);
	line = get_next_line(STDIN_FILENO);
	while (line && !(ft_strncmp(limiter, line, limiter_len) == 0
			&& line[limiter_len] == '\n'))
	{
		write(pipe_fd[1], line, ft_strlen(line));
		free(line);
		write(STDOUT_FILENO, "here_doc > ", 11);
		line = get_next_line(STDIN_FILENO);
	}
	if (!line)
		print_error("pipe_here_doc: ", "Ctrl+d Nan Nan Nan.");
	else
		free(line);
	close(pipe_fd[1]);
	exit(EXIT_SUCCESS);
}

/*
	Create child process for here_doc piping
*/
void	pipe_here_doc(char *limiter)
{
	pid_t	pid;
	int		pipe_fd[2];

	if (pipe(pipe_fd) < 0)
		exit_error_msg("pipe_here_doc: ", strerror(errno));
	pid = fork();
	if (pid < 0)
		exit_error_msg("pipe_here_doc: ", strerror(errno));
	if (pid == 0)
		write_here_doc(pipe_fd, limiter);
	waitpid(pid, NULL, 0);
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		exit_error_msg("pipe_here_doc: ", strerror(errno));
	close(pipe_fd[0]);
}

/*
	Redirect from file or to file.
*/
static void	file_redirect(t_pipex data, int redirect_flag)
{
	int	fd;

	fd = 0;
	if (redirect_flag == READ_FROM_FILE)
		fd = open(data.infile, O_RDONLY);
	if (fd < 0)
		exit_error_cmd("pipex: ", data.infile, strerror(errno));
	if (redirect_flag == WRITE_TO_FILE_TRUNC)
		fd = open(data.outfile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (redirect_flag == WRITE_TO_FILE_APPEND)
		fd = open(data.outfile, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd < 0)
		exit_error_cmd("pipex: ", data.outfile, strerror(errno));
	if (redirect_flag == 0 && dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		exit_error_msg("pipex: ", strerror(errno));
	}
	if (redirect_flag > 2 && dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		exit_error_msg("pipex: ", strerror(errno));
	}
	close(fd);
}

/*
	FLAGS :
	0 : redirect from. Same as '<'
	1 : here_doc
	3 : normal pipe (Also work for here_doc ?)
	4 : Redirect to (TRUNCATE). Same as '>'
	5 : Redirect to (APPEND). Same as '>>' 
*/
void	redirect_io(int pipe_fd[2], t_pipex data, int redirect_flag)
{
	close(pipe_fd[0]);
	if (redirect_flag == READ_FROM_FILE || redirect_flag == PIPE)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
			exit_error_msg("pipex: ", strerror(errno));
		close(pipe_fd[1]);
		if (redirect_flag == PIPE)
			return ;
		file_redirect(data, redirect_flag);
	}
	else
	{
		close(pipe_fd[1]);
		file_redirect(data, redirect_flag);
	}
}
