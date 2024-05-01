/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 20:28:36 by dbaladro          #+#    #+#             */
/*   Updated: 2024/04/29 13:49:07 by dbaladro         ###   ########.fr       */
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
