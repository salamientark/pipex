/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 20:28:36 by dbaladro          #+#    #+#             */
/*   Updated: 2024/03/29 12:07:31 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

/*
	Create Default environment if not found
*/
char	**get_env(char **envp)
{
	char	**env;

	if (envp != NULL)
		return (envp);
	env = (char **)malloc(sizeof(char *) * 2);
	if (!env)
		exit_error_msg("get_env: ", strerror(errno));
	env[0] = ft_strdup("PATH=/usr/bin:/bin");
	if (!env[0])
	{
		free(env);
		exit_error_msg("get_env: ", strerror(errno));
	}
	env[1] = NULL;
	return (env);
}

/*
	Init t_pipex struct
*/
t_pipex	init_pipex(int ac, char **av, char **envp)
{
	t_pipex	data;

	if (av[1] && ft_strcmp(av[1], "here_doc") == 0 && ac < 6)
		exit_error_msg(HERE_DOC_MISUSE, "");
	if (ac < 5)
		exit_error_msg(PIPEX_MISUSE, "");
	// data.env = envp;
	data.env = get_env(envp);
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

pid_t	pipex(char *cmd, t_pipex data, int redirect_flag)
{
	pid_t	pid;
	int		pipe_fd[2];
	char	**final_cmd;

	if (pipe(pipe_fd) < 0)
		exit_error_msg("pipex: ", strerror(errno));
	pid = fork();
	if (pid < 0)
		exit_error_msg("pipex: ", strerror(errno));
	if (pid == 0)
	{
		redirect_io(pipe_fd, data, redirect_flag);
		final_cmd = parse_command(cmd, data.env);
		if (!final_cmd)
			exit(EXIT_FAILURE);
		execve(final_cmd[0], final_cmd, data.env);
		print_error_cmd("pipex: ", final_cmd[0], strerror(errno));
		free_str_tab(&final_cmd);
		exit(126);
	}
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		exit_error_msg("pipex: ", strerror(errno));
	close(pipe_fd[0]);
	return (pid);
}

int	wait_for_children(pid_t last_pid)
{
	int		status;
	int		return_value;

	while (ECHILD != errno)
	{
		if (waitpid(0, &status, 0) == last_pid)
		{
			if (WIFEXITED(status))
				return_value = WEXITSTATUS(status);
			else
				return_value = WSTOPSIG(status);
		}
	}
	return (return_value);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	data;
	int		index;
	int		last_pid;

	data = init_pipex(ac, av, envp);
	index = 2 + data.here_doc;
	last_pid = pipex(av[index++], data, data.here_doc);
	while (index < ac - 2)
	{
		last_pid = pipex(av[index], data, 3);
		index++;
	}
	last_pid = pipex(av[index], data, 4 + data.here_doc);
	if (!envp)
		free_str_tab(&(data.env));
	return (wait_for_children(last_pid));
}
