/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 01:03:24 by dbaladro          #+#    #+#             */
/*   Updated: 2024/04/29 01:34:59 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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
