/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:26:46 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/03 16:30:51 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	Point of get path is making a "correctly" splitted PATH array.
	Basically a reworked ft_split addind '/' at every path's end.
	EXEMPLE :
		{								 \		{
		...,						|-----\		"/usr/bin/",
		"PATH=/usr/bin:/bin:...",   |-----/		"/bin/",
		...,							 /		NULL,
		}										}
*/

#include "../includes/pipex.h"

extern char	**environ;

static void	free_all(char ***str_tab_ptr, unsigned int size)
{
	while (size > 0 || (*str_tab_ptr)[size] != NULL)
	{
		free(*(str_tab_ptr[size]));
		*(str_tab_ptr[size--]) = 0;
	}
	free(*str_tab_ptr);
	*str_tab_ptr = 0;
}

static unsigned int	count_path(const char *str)
{
	int			word_count;
	int			new_word;
	const char	*str_p;

	str_p = str;
	word_count = 0;
	new_word = 1;
	while (*str_p)
	{
		if (*str_p != ':')
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

static char	*extract_path(const char **str)
{
	int		i;
	int		size;
	char	*word;

	while (**str == ':')
		(*str)++;
	i = 0;
	while ((*str)[i] && (*str)[i] != ':')
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
		word[i] = (*str)[i];
		i++;
	}
	word[i++] = '/';
	word[i] = '\0';
	*str += size;
	return (word);
}

char	**split_path(const char *path)
{
	char			**strs_tab;
	unsigned int	word_nb;
	unsigned int	index;

	if (!path)
		exit_error_msg("make_path: ", "No PATH found... Nowhere");
	word_nb = count_path(path);
	strs_tab = (char **)malloc(sizeof(char *) * (word_nb + 1));
	if (!strs_tab)
		exit_error_msg("make_path: ", strerror(errno));
	index = 0;
	while (index < word_nb)
	{
		strs_tab[index] = extract_path(&path);
		if (!strs_tab[index])
		{
			free_all(&strs_tab, index);
			print_error("make_path: ", strerror(errno));
			return (NULL);
		}
		index++;
	}
	strs_tab[index] = NULL;
	return (strs_tab);
}
