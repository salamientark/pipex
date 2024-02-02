/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:14:45 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/02 09:29:27 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/pipex.h"

void    free_cmd(char ***cmd_ptr)
{
    int index;

    index = 0;
    while ((*cmd_ptr)[index])
    {
        free((*cmd_ptr)[index]);
        (*cmd_ptr)[index] = NULL;
        index++;
    }
    free(*cmd_ptr);
    *cmd_ptr = NULL;
}

char    *add_bin_dir(char *cmd)
{
    char    *final_cmd;

    if (access(cmd, F_OK | X_OK) == 0)
        return (cmd);
    final_cmd = ft_strjoin("/usr/bin/", cmd);
    if (access(final_cmd, F_OK | X_OK) == 0)
        return (free(cmd), final_cmd);
    free(final_cmd);
    final_cmd = ft_strjoin("/bin/", cmd);
    if (access(final_cmd, F_OK | X_OK) == 0)
        return (free(cmd), final_cmd);
    print_error(cmd, errno);
    free(cmd);
    return (NULL);
}

char    **parse_command(char *cmd)
{
    char    **parse_cmd;

    parse_cmd = ft_split(cmd, ' ');
    if (!parse_cmd)
    {
        print_error("parse_command: ft_split error", -1);
        return (NULL);
    }
    parse_cmd[0] = add_bin_dir(parse_cmd[0]);
    if (!parse_cmd[0])
    {
        print_error("parse_command: command not found", -1);
        return (NULL);
    }
    return (parse_cmd);
}