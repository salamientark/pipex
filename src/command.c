/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:14:45 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/03 17:02:08 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void free_cmd(char ***cmd_ptr)
{
    int index;

    index = 1;
    while ((*cmd_ptr)[index] != NULL)
    {
        free((*cmd_ptr)[index]);
        (*cmd_ptr)[index] = NULL;
        index++;
    }
    free(*cmd_ptr);
    *cmd_ptr = NULL;
}

/*
    add correct dir prefix from path to cmd
*/
static char	*add_executable_dir(char *cmd, char *path)
{
	char	**splited_path;
	char	*final_cmd;
	int		index;

	if (access(cmd, F_OK | X_OK) == 0)
		return (cmd);
	splited_path = split_path(path);
	if (!splited_path)
		return (NULL);
	index = 0;
	while (splited_path[index])
	{
		final_cmd = ft_strjoin(splited_path[index], cmd);
		if (access(final_cmd, F_OK | X_OK) == 0)
			return (free_str_tab(&splited_path), final_cmd);
		free(final_cmd);
		index++;
	}
	free_str_tab(&splited_path);
	print_error(cmd, " : Command not found");
	return (NULL);
}

/*
    Add correct binary from env to cmd
*/
char	**parse_command(char *cmd, char **env)
{
	char	**parse_cmd;
	int		path_pos;

	parse_cmd = ft_split(cmd, ' ');
	if (!parse_cmd)
	{
		print_error("parse_command: ", "ft_split error");
		return (NULL);
	}
	path_pos = 0;
	while (env[path_pos] && ft_strncmp(env[path_pos], "PATH=", 5) != 0)
		path_pos++;
	if (!env[path_pos])
	{
        free_str_tab(&parse_cmd);
		print_error_cmd("parse_command: ", cmd, "Command not found");
		return (NULL);
	}
	parse_cmd[0] = add_executable_dir(parse_cmd[0], env[path_pos]);
	if (!parse_cmd[0])
	{
		free_cmd(&parse_cmd);
		return (NULL);
	}
	return (parse_cmd);
}
