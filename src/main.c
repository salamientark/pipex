/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 01:03:24 by dbaladro          #+#    #+#             */
/*   Updated: 2024/04/29 13:52:15 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

/*
	Init t_pipex struct
*/
t_pipex	init_pipex(int ac, char **av, char **envp)
{
	t_pipex	data;

	if (ac != 5)
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
	index = 2;
	last_pid = pipex(av[index++], data, READ_FROM_FILE);
	while (index < ac - 2)
	{
		last_pid = pipex(av[index], data, 3);
		index++;
	}
	last_pid = pipex(av[index], data, 4);
	if (!envp)
		free_str_tab(&(data.env));
	return (wait_for_children(last_pid));
}
