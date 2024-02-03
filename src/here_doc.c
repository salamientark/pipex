/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 10:13:07 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/03 16:51:25 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

/*
    Create here_doc
*/
int	create_here_doc(char *limiter)
{
	int		fd;
	int		limiter_len;
	char	*line;

	fd = open("./here_doc", O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (fd == -1)
		return (print_error("create_here_doc: ", strerror(errno)), -1);
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
		return (print_error("Ctrl + D", ""), EXIT_SUCCESS);
	return (free(line), 0);
}

/*
    first child exec
*/
static void	first_command(int pipe[2], char *cmd, char **env)
{
	char	**parsed_command;
	int		fd;

	fd = open("./here_doc", O_RDONLY);
	if (fd < 0)
		exit_error_msg("first_command: ", strerror(errno));
	close(pipe[0]);
	if (dup2(fd, STDIN_FILENO) < 0)
		exit_error_msg("first_command: ", strerror(errno));
	if (dup2(pipe[1], STDOUT_FILENO) < 0)
		exit_error_msg("first_command: ", strerror(errno));
	close(pipe[1]);
	close(fd);
	parsed_command = parse_command(cmd, env);
	if (!parsed_command)
		exit(1);
	execve(parsed_command[0], parsed_command, env);
	free_str_tab(&parsed_command);
	exit_error_msg("first_command: ", strerror(errno));
}

/*
    Second child exec
*/
static void	second_command(char *outfile, int pipe[2], char *cmd, char **env)
{
	char	**parsed_command;
	int		fd;

	fd = open(outfile, O_CREAT | O_APPEND | O_WRONLY, 0664);
	if (fd < 0)
		exit_error_cmd("second_command: ", outfile, strerror(errno));
	close(pipe[1]);
	if (dup2(fd, STDOUT_FILENO) < 0)
		exit_error_msg("second_command: ", strerror(errno));
	if (dup2(pipe[0], STDIN_FILENO) < 0)
		exit_error_msg("second_command: ", strerror(errno));
	close(fd);
	close(pipe[0]);
	parsed_command = parse_command(cmd, env);
	if (!parsed_command)
		exit(1);
	execve(parsed_command[0], parsed_command, env);
	free_str_tab(&parsed_command);
	exit_error_msg("Second_command: ", strerror(errno));
}

/*
    pipex whith here_doc
*/
int	pipe_here_doc(int ac, char **av, char **env)
{
	int		pipefd[2];
	pid_t	pid;

	(void) ac;
	if (pipe(pipefd) == -1)
		return (print_error("pipe_here_doc: ", strerror(errno)), -1);
	if (create_here_doc(av[2]) < 0)
		return (EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		return (print_error("pipex_here_doc: ", strerror(errno)), -1);
	if (pid == 0)
		first_command(pipefd, av[3], env);
	pid = fork();
	if (pid < 0)
		return (print_error("pipex_here_doc: ", strerror(errno)), -1);
	if (pid == 0)
		second_command(av[5], pipefd, av[4], env);
	wait(NULL);
	unlink("./here_doc");
	return (0);
}
