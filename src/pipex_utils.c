/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 13:24:06 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/06 21:36:51 by dbaladro         ###   ########.fr       */
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
	Open file + print error if failed
	OPEN_MODE :
	0 - Create_here_doc
	1 - Read file
	2 - Append to file
	3 - Write to file
*/
int	ft_open(char *filename, int open_mode)
{
	int	fd;

	if (open_mode == 0)
		fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	else if (open_mode == 1)
		fd = open(filename, O_RDONLY);
	else if (open_mode == 2)
		fd = open(filename, O_CREAT | O_APPEND | O_WRONLY, 0664);
	else
		fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0664);
	if (fd < 0)
		print_error_cmd("pipex: ", filename, strerror(errno));
	return (fd);
}

/*
	Create here_doc and return fd descritor of it
*/
int	here_doc(char *limiter)
{
	int		fd;
	int		limiter_len;
	char	*line;

	fd = ft_open(HERE_DOC, 0);
	if (fd == -1)
		return (-1);
	limiter_len = ft_strlen(limiter);
	write(1, "> ", 2);
	line = get_next_line(0);
	while (line && ft_strncmp(line, limiter, limiter_len) != 0)
	{
		write(fd, line, ft_strlen(line));
		free(line);
		write(1, "> ", 2);
		line = get_next_line(0);
	}
	close(fd);
	if (!line)
		print_error("Ctrl + D", "");
	fd = ft_open(HERE_DOC, 1);
	return (free(line), fd);
}

/*
	execve + error printing
*/
void	ft_exec(char *cmd, char **env)
{
	char	**final_cmd;

	final_cmd = parse_command(cmd, env);
	if (final_cmd)
	{
		execve(final_cmd[0], final_cmd, env);
		print_error("pipex: ", strerror(errno));
		return ;
	}
	return ;
}

/*
	Redirect command output to file opened in open_mode
*/
void	ft_redirect_to(char *filename, int open_mode, char *cmd, char **env)
{
	pid_t	pid;
	int		fd;

	fd = ft_open(filename, open_mode);
	if (fd < 0)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid < 0)
		return (print_error("pipex: ", strerror(errno)));
	if (pid == 0)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
			exit_error_msg("pipex: ", strerror(errno));
		close(fd);
		ft_exec(cmd, env);
		exit(EXIT_FAILURE);
	}
	close(fd);
}
