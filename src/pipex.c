/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:27:16 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/05 21:01:31 by dbaladro         ###   ########.fr       */
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

int    first_command(char *infile, char *cmd, int pipe_fd[2], char **env)
{
    char    **splited_cmd;
    int     fd;

    fd = open(infile, O_RDONLY);
    if (fd < 0)
        exit_error_cmd("pipex: ", infile, strerror(errno));
    close(pipe_fd[0]);
    if (dup2(fd, STDIN_FILENO) < 0 || dup2(pipe_fd[1], STDOUT_FILENO) < 0)
        return (close(fd), close(pipe_fd[1]), EXIT_FAILURE);
    close(fd);    
    close(pipe_fd[1]);
    splited_cmd = parse_command(cmd, env);  
    if (!splited_cmd)
        return (EXIT_FAILURE);
    execve(splited_cmd[0], splited_cmd, env);
    free_str_tab(&splited_cmd);
    print_error("pipex: ", strerror(errno));
    return (EXIT_FAILURE);  
}

int    last_command(char *outfile, char *cmd, int pipe_fd[2], char **env)
{
    ft_printf("outfile: %s | cmd %s\n", outfile, cmd);
    char    **splited_cmd;
    int     fd;

    fd = open(outfile, O_CREAT | O_TRUNC | O_WRONLY, 0664);
    if (fd < 0)
        exit_error_cmd("pipex: ", outfile, strerror(errno));
    close(pipe_fd[1]);
    if (dup2(pipe_fd[0], STDIN_FILENO) < 0 || dup2(fd, STDOUT_FILENO) < 0)
        return (close(fd), EXIT_FAILURE);
    close(fd);    
    close(pipe_fd[0]);
    splited_cmd = parse_command(cmd, env);  
    if (!splited_cmd)
        return (EXIT_FAILURE);
    execve(splited_cmd[0], splited_cmd, env);
    free_str_tab(&splited_cmd);
    print_error("pipex: ", strerror(errno));
    return (EXIT_FAILURE);  
}

/*
    MAYBE ADD A SECOND PIPE TO LINK MIDDLE PROG WITH PREVIOUS + MIDDLE PROG WITH NEXT
*/

int    middle_command(char *cmd, int pipe_from[2], int pipe_to[2], char **env)
{
    char    **splited_cmd;

    close(pipe_from[1]);
    close(pipe_to[0]);
    if (dup2(pipe_from[0], STDIN_FILENO) < 0 || dup2(pipe_to[1], STDOUT_FILENO) < 0)
    {
        close(pipe_from[0]);
        close(pipe_to[1]);
        exit_error_msg("Middle_command: ", strerror(errno));
    }
    close(pipe_from[0]);
    close(pipe_to[1]);
    splited_cmd = parse_command(cmd, env);  
    if (!splited_cmd)
        return (EXIT_FAILURE);
    execve(splited_cmd[0], splited_cmd, env);
    free_str_tab(&splited_cmd);
    print_error("pipex: ", strerror(errno));
    return (EXIT_FAILURE);  
}


void    pipex(int ac, char **av, char **env)
{
    int     index;
    int     pipe_fd[2][2];
    pid_t   pid;

    if (pipe(pipe_fd[0]) < 0 || pipe(pipe_fd[1]) < 0)
        exit_error_msg("pipex: ", strerror(errno));
    index = 2;
    while (index < ac - 1)
    {
        pid = fork();
        if (pid < 0)
            exit_error_msg("pipex: ", strerror(errno));
        if (pid == 0)
        {
            if (index == 2)
                exit(first_command(av[1], av[2], pipe_fd[0], env));
            else if (index > 2 && index < ac - 2)
                exit(middle_command(av[index], pipe_fd[(index + 1) % 2], pipe_fd[index % 2], env));
            else
                exit(last_command(av[index + 1], av[index], pipe_fd[(index + 1) % 2], env));
        }
        index++;
    }
    close(pipe_fd[0][0]);
    close(pipe_fd[0][1]);
    close(pipe_fd[1][0]);
    close(pipe_fd[1][1]);
    wait(NULL);
}

int main(int ac, char **av)
{
    char    **env;

    if (ac <= 4 || (!ft_strcmp(av[1], "here_doc") && ac != 6))
        return (0);
    if (!environ)
        env = create_env();
    else
        env = environ;
    if (ft_strcmp(av[1], "here_doc") == 0)
        ft_printf("here_doc return : %d\n", pipe_here_doc(ac, av, env));
    else
    {
        pipex(ac, av, env);
    }
    if (!environ)
        free_str_tab(&env);
    return (0);
}