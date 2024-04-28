/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 01:23:27 by dbaladro          #+#    #+#             */
/*   Updated: 2024/04/29 01:31:43 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int ac, char **av, char **envp)
{
	t_pipex	data;
	int		index;
	int		last_pid;

	data = init_pipex(ac, av, envp);
	index = 2 + data.here_doc;
	if (data.here_doc)
		pipe_here_doc(data.limiter);
	last_pid = pipex(av[index++], data, 3);
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
