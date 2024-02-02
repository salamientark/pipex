/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:27:16 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/01 15:57:57 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// int pipex(int size, char **arg)
// {
//     int     index;
//     int     pipe[2][2];
//     pid_t   pid; 

//     index = 0;
//     while (index < size)
//     {
//         pid = fork();
//         if (pid == -1)
//             return (1);
//         if (pid == 0)
//         {
            
//         }
//         else
//         {
            
//         }
//         index++;
//     }
// }

// int open_infile(char **av)
// {
//     int fd;

//     if (ft_strcmp(av[1], "here_doc") == 0)
//         fd = 
// }

int main(int ac, char **av)
{
    // int infile;

    if (ac <= 4)
        return (0);
    if (ft_strcmp(av[1], "here_doc") == 0)
    {
        ft_printf("here_doc : %d\n", pipe_here_doc(ac, av));
    }

    return (0);
}