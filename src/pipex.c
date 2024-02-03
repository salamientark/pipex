/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:27:16 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/03 17:01:57 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

extern char **environ;

void    free_str_tab(char ***str_tab_ptr)
{
    int index;

    index = 0;
    while ((*str_tab_ptr)[index] != NULL)
    {
        free((*str_tab_ptr)[index]);
        (*str_tab_ptr)[index] = NULL;
        index++;
    }
    free(*str_tab_ptr);
    *str_tab_ptr = NULL;
}

/*
    Create Defautl environment if not found
*/
char **create_env(void)
{
    char    **env;

    env = (char **)malloc(sizeof(char *) * 2);
    if (!env)
        exit_error_msg("create_env: ", "Unable to create_env");
    env[0] = "PATH=/usr/bin:/bin";
    env[1] = NULL;
    return (env);
}

int main(int ac, char **av)
{
    char    **env;

    if (ac <= 4)
        return (0);
    if (!environ)
        env = create_env();
    else
        env = environ;
    if (ft_strcmp(av[1], "here_doc") == 0)
    {
        ft_printf("here_doc return : %d\n", pipe_here_doc(ac, av, env));
    }
    // if (ft_strcmp(av[1], "here_doc") == 0)
    // {
    //     if (create_here_doc(av[2]) == -1)
    //         return (1);
    //     infile_fd = open("./here_doc", O_RDONLY);
    // }
    // else
    //     infile_fd = open(av[1], O_RDONLY);
    // if (infile_fd < 0)
    //     print_error("pipex: ", strerror(errno));
    if (!environ)
        free_str_tab(&env);
    return (0);
}