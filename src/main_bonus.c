/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 01:23:27 by dbaladro          #+#    #+#             */
/*   Updated: 2024/05/01 09:54:04 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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

int	main(int ac, char **av, char **envp)
{
	t_pipex	data;
	int		index;
	int		last_pid;

	data = init_pipex(ac, av, envp);
	index = 2 + data.here_doc;
	if (data.here_doc)
		pipe_here_doc(data.limiter);
	else
		last_pid = pipex(av[index++], data, READ_FROM_FILE);
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
