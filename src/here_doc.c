/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 10:13:07 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/02 09:35:20 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char *env[] = { "PATH=/bin:/usr/bin", "USER=me", NULL };

int create_here_doc(char *limiter)
{
    int     fd;
    int     limiter_len;
    char    *line;

    fd = open("./here_doc", O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd == -1)
        return (print_error("create_here_doc: ", errno), -1);
    limiter_len = ft_strlen(limiter);
    line = get_next_line(0);
    while (line && ft_strncmp(line, limiter, limiter_len) != 0)
    {
        write(fd, line, ft_strlen(line));
        free(line);
        line = get_next_line(0);
    }
    close(fd);
    if (!line)
        return (print_error("Ctrl + D", -1), 0);
    ft_printf("SuCess\n");
    return (free(line), 0);
}

void first_command(int pipe[2], char *cmd)
{
    char    **parsed_command;
    int     fd;

    fd = open("./here_doc", O_RDONLY);
    if (fd < 0)
        exit_error_msg("first_command: ", errno);
    close(pipe[0]);
    if (dup2(fd, STDIN_FILENO) < 0)
        exit_error_msg("first_command: ", errno);
    if (dup2(pipe[1], STDOUT_FILENO) < 0)
        exit_error_msg("first_command: ", errno);
    parsed_command = parse_command(cmd);
    if (!parsed_command)
        exit(1);
    close(pipe[1]);
    close(fd);
    execve(parsed_command[0], parsed_command, env);
    exit_error_msg("firs_command: ", errno);
}

int second_command(int fd, int pipe[2], char *cmd)
{
    char    **parsed_command;
    int     exec_result;

    close(pipe[1]);
    if (dup2(fd, STDOUT_FILENO) < 0)
        exit_error_msg("second_command: ", errno);
    if (dup2(pipe[0], STDIN_FILENO) < 0)
        exit_error_msg("second_command: ", errno);
    parsed_command = parse_command(cmd);
    if (!parsed_command)
        return (EXIT_FAILURE);
    exec_result = execve(parsed_command[0], parsed_command, env);
    close(fd);
    close(pipe[0]);
    if (exec_result < 0)
        return (print_error("Second_command", errno), EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

int pipe_here_doc(int ac, char **av)
{
    int     fd_outfile;
    int     pipefd[2];
    pid_t   pid;

    (void) ac;
    if (pipe(pipefd) == -1)
        return (print_error("pipe_here_doc: ", errno), -1);
    if (create_here_doc(av[2]) < 0)
        return (EXIT_FAILURE);
    pid = fork();
    if (pid == -1)
        return (print_error("pipex_here_doc: ", errno),-1);
    if (pid == 0)
        first_command(pipefd, av[3]); 
    wait(NULL);
    pid = fork();
    if (pid < 0)
        return (print_error("pipex_here_doc: ", errno),-1);
    if (pid == 0)
    {
        fd_outfile = open(av[5], O_CREAT | O_APPEND | O_WRONLY, 00664);
        if (fd_outfile < 0)
            exit_error_msg("pipe_here_doc: ", errno);
        second_command(fd_outfile, pipefd, av[4]);
    }
    wait(NULL);
    return (0);
}