/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:27:16 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/06 13:33:39 by madlab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

extern char	**environ;

/*
	Create Defautl environment if not found
*/
char	**get_env(void)
{
	char	**env;

	if (environ != NULL)
		return (environ);
	env = (char **)malloc(sizeof(char *) * 2);
	if (!env)
		return (print_error("get_env: ", strerror(errno)), (char **) NULL);
	env[0] = ft_strdup("PATH=/usr/bin:/bin");
	if (!env[0])
	{
		free(env);
		print_error("get_env: ", strerror(errno));
		return ((char **) NULL);
	}
	env[1] = NULL;
	return (env);
}

/*
	Open infile or here_doc + redirect it to STD_IN + set index
*/
int	init_pipex(int ac, char **av, int *index_ptr)
{
	int	fd;

	if (av[1] && ft_strcmp(av[1], "here_doc") == 0 && ac < 6)
		exit_error_msg(HERE_DOC_MISUSE, "");
	if (ac < 5)
		exit_error_msg(PIPEX_MISUSE, "");
	*index_ptr = 2;
	if (ft_strcmp(av[1], "here_doc") == 0)
	{
		fd = here_doc(av[2]);
		*index_ptr += 1;
	}
	else
		fd = ft_open(av[1], 1);
	if (fd < 0)
		return (-1);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (print_error("pipex: ", strerror(errno)), close(fd), -1);
	// return (fd);
	// close(fd);
	return (fd);
}

/*
	pipe + fork + exec
*/
void	pipex(char *cmd, char **env)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) < 0)
		return (print_error("pipex: ", strerror(errno)));
	pid = fork();
	if (pid < 0)
		return (print_error("pipex: ", strerror(errno)));
	if (pid == 0)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
			exit_error_msg("pipex: ", strerror(errno));
		ft_exec(cmd, env);
		close(pipe_fd[1]);
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO))
		exit_error_msg("pipex: ", strerror(errno));
	close(pipe_fd[0]);
}

// // /*
// //	 Redirect command output to file opened in open_mode
// // */
// void	ft_redirect_from(char *filename, int open_mode, char *cmd, char **env)
// {
//	 pid_t   pid;
//	 int	 pipe_fd[2];
//	 int	 fd;

//	 fd = ft_open(filename, open_mode);
//	 if (fd < 0)
//		 exit(EXIT_FAILURE);
//	 if (dup2(fd, STDIN_FILENO) < 0)
//		 exit_error_msg("pipex: ", strerror(errno));
//	 if (pipe(pipe_fd) < 0)
//		 return (print_error("ft_redirect_to: ", strerror(errno)));
//	 pid = fork();
//	 if (pid < 0)
//		 return (print_error("pipex: ", strerror(errno)));
//	 if (pid == 0)
//	 {
//		 close(pipe_fd[0]);
//		 if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
//			 exit_error_msg("pipex: ", strerror(errno));
//		 ft_exec(cmd, env);
//		 close(pipe_fd[1]);
//		 exit(EXIT_FAILURE);
//	 }
//	 close(pipe_fd[1]);
//	 if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
//		 print_error("pipex: ", strerror(errno));
//	 close(pipe_fd[0]);
// }

int	main(int ac, char **av)
{
	char	**env;
	int		index;

	env = get_env();
	if (!env)
		return (EXIT_FAILURE);
	if (init_pipex(ac, av, &index) < 0)
		return (0);
	while (index < ac - 2)
	{
		pipex(av[index], env);
		index++;
	}
	if (ft_strcmp(av[1], "here_doc") == 0)
		ft_redirect_to(av[index + 1], 2, av[index], env);
	else
		ft_redirect_to(av[index + 1], 3, av[index], env);
	wait(NULL);
	if (!environ)
	{
		free(env[1]);
		free(env);
	}
	return (0);
}
