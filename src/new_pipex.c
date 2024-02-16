/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_pipex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 20:28:36 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/16 22:15:56 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	pipe_here_doc(char *limiter)
{
	pid_t   pid;
    int     pipe_fd[2];
	int		limiter_len;
	char	*line;

    if (pipe(pipe_fd) < 0)
        exit_error_msg("pipe_here_doc: ", strerror(errno));
    pid = fork();
    if (pid < 0)
        exit_error_msg("pipe_here_doc: ", strerror(errno));
    if (pid == 0)
	{
		limiter_len = ft_strlen(limiter);
		close(pipe_fd[0]);
		// if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		// 	exit_error_msg("pipe_here_doc: ", strerror(errno));
		// close(pipe_fd[1]);
		write(STDOUT_FILENO, "here_doc > ", 11);
		line = get_next_line(STDIN_FILENO);
		while (line && ft_strncmp(limiter, line, limiter_len) != 0)
		{
			write(STDOUT_FILENO, line, ft_strlen(line));
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
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		exit_error_msg("pipe_here_doc: ", strerror(errno));
	close(pipe_fd[0]);
	waitpid(pid, NULL, 0);
}

t_pipex	new_init_pipex(int ac, char **av)
{
	t_pipex	data;

	if (av[1] && ft_strcmp(av[1], "here_doc") == 0 && ac < 6)
		exit_error_msg(HERE_DOC_MISUSE, "");
	if (ac < 5)
		exit_error_msg(PIPEX_MISUSE, "");
	data.env = get_env();
	data.infile = av[1];
	data.outfile = av[ac - 1];
	data.limiter = NULL;
	data.here_doc = 0;
	if (ft_strcmp(av[1], "here_doc") == 0)
	{
		data.here_doc = 1;
		data.limiter = av[2];
	}
	else
		data.infile = av[1];
	return (data);
}

void	first_cmd(char *cmd, t_pipex data)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		fd;

	if (pipe(pipe_fd) < 0)
		exit_error_msg("pipex: ", strerror(errno));
	pid = fork();
	if (pid < 0)
		exit_error_msg("pipex: ", strerror(errno));
	if (pid == 0)
	{
		close(pipe_fd[0]);
		if (data.here_doc)
			pipe_here_doc(data.limiter);
		else
		{
			fd = open(data.infile, O_RDONLY);
			if (fd < 0)
				exit_error_cmd("pipex: ", data.infile, strerror(errno));
			if (dup2(fd, STDIN_FILENO) < 0)
			{
				close(fd);
				exit_error_msg("pipex: ", strerror(errno));
			}
            close(fd);
        }
        if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
            exit_error_msg("pipex: ", strerror(errno));
        ft_exec(cmd, data.env);
        exit_error_msg("pipex: ", strerror(errno));
	}
    close(pipe_fd[1]);
    if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
        exit_error_msg("pipex: ", strerror(errno));
    close(pipe_fd[0]);
}


/*
	FLAGS :
	0 : redirect from. Same as '<'
	1 : here_doc
	3 : normal pipe (Also work for here_doc ?)
	2 : Redirect to (TRUNCATE). Same as '>'
	5 : Redirect to (APPEND). Same as '>>' 
*/
void	redirect_io(int pipe_fd[2], t_pipex data, int redirect_flag)
{
	int	fd;

	if (redirect_flag <= 3)
	{
		close(pipe_fd[0]);
		if(dup2(pipe_fd[1], STDOUT_FILENO) < 0)
			exit_error_msg("pipex: ", strerror(errno));
		close(pipe_fd[1]);
		if (redirect_flag == 1)
			pipe_here_doc(data.limiter);
		if (redirect_flag == 0)
		{
			fd = open(data.infile, O_RDONLY);
			if (fd < 0)
				exit_error_cmd("pipex: ", data.infile, strerror(errno));
			if (dup2(fd, STDIN_FILENO) < 0)
			{
				close(fd);
				exit_error_msg("pipex: ", strerror(errno));
			}
        	close(fd);
		}
		
	}
	else
	{
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		if (redirect_flag == 4)
			fd = open(data.outfile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		else
			fd = open(data.outfile, O_CREAT | O_APPEND | O_WRONLY, 0644);
		if (fd < 0)
			exit_error_cmd("pipex: ", data.outfile, strerror(errno));
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			exit_error_msg("pipex: ", strerror(errno));
		}
		close(fd);
	}
	
}

void	new_pipex(char *cmd, t_pipex data, int redirect_flag)
{
	pid_t	pid;
	int		pipe_fd[2];

	if (pipe(pipe_fd) < 0)
		exit_error_msg("pipex: ", strerror(errno));
	pid = fork();	
	if (pid < 0)
		exit_error_msg("pipex: ", strerror(errno));
	if (pid == 0)
	{
		// close(pipe_fd[0]);
		// if(dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		// 	exit_error_msg("pipex: ", strerror(errno));
		// close(pipe_fd[1]);
		redirect_io(pipe_fd, data, redirect_flag);
		ft_exec(cmd, data.env);
		exit_error_msg("pipex: ", strerror(errno));
	}
	close(pipe_fd[1]);
	if(dup2(pipe_fd[0], STDIN_FILENO) < 0)
		exit_error_msg("pipex: ", strerror(errno));
	close(pipe_fd[0]);
}


int	main(int ac, char **av)
{
	t_pipex	data;
	int		index;

	data = new_init_pipex(ac, av);
	index = 2 + data.here_doc;
	new_pipex(av[index++], data, data.here_doc);
	while (index < ac - 2)
	{
		new_pipex(av[index], data, 3);
		index++;
	}
	new_pipex(av[index], data, 4 + data.here_doc);
	while (wait(NULL) != -1)
		;
	return (0);
}
