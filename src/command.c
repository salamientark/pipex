/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:14:45 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/05 22:50:56 by madlab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	This file split the command into correct execve format
		(adding executable dir to command)
	Basically a reworked split with *add_executable_dir* func for first arg
*/

#include "../includes/pipex.h"

/*
	free already malloc block on error
*/
static void	free_all(char ***split_cmd_ptr, unsigned int size)
{
	while (size > 0)
	{
		free(*(split_cmd_ptr[size]));
		*(split_cmd_ptr[size--]) = 0;
	}
	free(*split_cmd_ptr);
	*split_cmd_ptr = 0;
}

/*
	Count Word
*/
static unsigned int	count_word(const char *str)
{
	int			word_count;
	int			new_word;
	const char	*str_p;

	str_p = str;
	word_count = 0;
	new_word = 1;
	while (*str_p)
	{
		if (*str_p != ' ')
		{
			if (new_word == 1)
				word_count++;
			new_word = 0;
		}
		else
			new_word = 1;
		str_p++;
	}
	return (word_count);
}

/*
	Add correct executable dir from env to cmd
	Then free(cmd) if necessary
*/
static char	*add_executable_dir(char **cmd_ptr, char **env)
{
	char	**path;
	char	*final_cmd;
	int		index;

	if (access(*cmd_ptr, F_OK | X_OK) != -1)
		return (*cmd_ptr);
	path = get_path(env);
	if (!path)
		return (free(*cmd_ptr), (char *) NULL);
	index = 0;
	while (path[index])
	{
		final_cmd = ft_strjoin(path[index], *cmd_ptr);
		if (!final_cmd)
			return (print_error("add_executable_dir: ", strerror(errno)),
				free_str_tab(&path), free(*cmd_ptr), (char *) NULL);
		if (access(final_cmd, F_OK | X_OK) != -1)
			return (free_str_tab(&path), free(*cmd_ptr), final_cmd);
		free(final_cmd);
		index++;
	}
	print_error_cmd("pipex: ", *cmd_ptr, "Command not found");
	free_str_tab(&path);
	free(*cmd_ptr);
	return ((char *) NULL);
}

/*
	Malloc and copy word
	Add correct dir prefix from path to cmd if first_arg
*/
static char	*extract_word(char **cmd_ptr, int pos, char **env)
{
	int		i;
	int		size;
	char	*word;

	while (**cmd_ptr == ' ')
		(*cmd_ptr)++;
	size = 0;
	while ((*cmd_ptr)[size] && (*cmd_ptr)[size] != ' ')
		size++;
	if (size == 0)
		return (NULL);
	word = (char *)malloc(size + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < size)
	{
		word[i] = (*cmd_ptr)[i];
		i++;
	}
	word[i] = '\0';
	*cmd_ptr += size;
	if (pos == 0)
		return (add_executable_dir(&word, env));
	return (word);
}

char	**parse_command(char *cmd, char **env)
{
	char			**splited_cmd;
	unsigned int	word_nb;
	unsigned int	index;

	if (!cmd)
		return (NULL);
	word_nb = count_word(cmd);
	splited_cmd = (char **)malloc(sizeof(char *) * (word_nb + 1));
	if (!splited_cmd)
		return (NULL);
	index = 0;
	while (index < word_nb)
	{
		splited_cmd[index] = extract_word(&cmd, index, env);
		if (!splited_cmd[index])
			return (free_all(&splited_cmd, index), (char **) NULL);
		index++;
	}
	splited_cmd[index] = (char *) NULL;
	return (splited_cmd);
}

// /*
// 	Add correct binary from env to cmd
// */
// char	**parse_command(char *cmd, char **env)
// {
// 	char	**parse_cmd;
// 	int		path_pos;

// 	parse_cmd = ft_split(cmd, ' ');
// 	if (!parse_cmd)
// 	{
// 		print_error("parse_command: ", "ft_split error");
// 		return (NULL);
// 	}
// 	path_pos = 0;
// 	while (env[path_pos] && ft_strncmp(env[path_pos], "PATH=", 5) != 0)
// 		path_pos++;
// 	if (!env[path_pos])
// 	{
// 		free_str_tab(&parse_cmd);
// 		print_error_cmd("parse_command: ", cmd, "Command not found");
// 		return (NULL);
// 	}
// 	add_executable_dir(&parse_cmd, env[path_pos]);
// 	if (!parse_cmd)
// 	{
// 		// free_cmd(&parse_cmd);
// 		return (NULL);
// 	}
// 	return (parse_cmd);
// }
