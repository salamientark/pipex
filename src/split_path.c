/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:26:46 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/05 22:53:40 by madlab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	Point of get path is making a "correctly" split PATH array from environment
	Basically a reworked ft_split addind '/' at every path's end.
	EXEMPLE :
		ENV :
		{								 \		{
		...,						|-----\		"/usr/bin/",
		"PATH=/usr/bin:/bin:...",   |-----/		"/bin/",
		...,							 /		NULL,
		}										}
*/

#include "../includes/pipex.h"

extern char	**environ;

/*
	Free the split_path if malloc failed during filling
	Set every pointer to NULL
*/
static void	free_all(char ***split_path_ptr, unsigned int size)
{
	while (size > 0 || (*split_path_ptr)[size] != NULL)
	{
		free(*(split_path_ptr[size]));
		*(split_path_ptr[size--]) = 0;
	}
	free(*split_path_ptr);
	*split_path_ptr = 0;
}

/*
	count PATH entrys from environment PATH variable
*/
static unsigned int	count_path(const char *path)
{
	int			word_count;
	int			new_word;
	const char	*path_p;

	path_p = path;
	word_count = 0;
	new_word = 1;
	while (*path_p)
	{
		if (*path_p != ':')
		{
			if (new_word == 1)
				word_count++;
			new_word = 0;
		}
		else
			new_word = 1;
		path_p++;
	}
	return (word_count);
}

/*
	Malloc and extract one path from the PATH variable (extracted from env)
*/
static char	*extract_path(char **path)
{
	int		i;
	int		size;
	char	*word;

	while (**path == ':')
		(*path)++;
	i = 0;
	while ((*path)[i] && (*path)[i] != ':')
		i++;
	if (i == 0)
		return (NULL);
	word = (char *)malloc(i + 2);
	if (!word)
		return (NULL);
	size = i;
	i = 0;
	while (i < size)
	{
		word[i] = (*path)[i];
		i++;
	}
	word[i++] = '/';
	word[i] = '\0';
	*path += size;
	return (word);
}

/*
	Get the PATH vaiable from env
	Return NULL if not found
*/
static char	*find_path_in_env(char **env)
{
	int	index;

	index = 0;
	while (env[index] && ft_strncmp(env[index], "PATH=", 5) != 0)
		index++;
	if (!env[index])
		return (print_error("pipex: ", "No PATH"), (char *) NULL);
	return (env[index] + 5);
}

/*
	split the PATH variable from env
*/
char	**get_path(char **env)
{
	char			**splited_path;
	char			*env_path;
	unsigned int	path_nbr;
	unsigned int	index;

	if (!env)
		return (print_error("get_path: ", "No ENV"), (char **) NULL);
	env_path = find_path_in_env(env);
	if (!env_path)
		return ((char **) NULL);
	path_nbr = count_path(env_path);
	splited_path = (char **)malloc(sizeof(char *) * (path_nbr + 1));
	if (!splited_path)
		return (print_error("get_path: ", strerror(errno)), (char **) NULL);
	index = 0;
	while (index < path_nbr)
	{
		splited_path[index] = extract_path(&env_path);
		if (!splited_path[index])
			return (free_all(&splited_path, index), print_error("make_path: ",
					strerror(errno)), (char **) NULL);
		index++;
	}
	splited_path[index] = NULL;
	return (splited_path);
}
