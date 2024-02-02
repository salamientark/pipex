/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 10:13:07 by dbaladro          #+#    #+#             */
/*   Updated: 2024/02/02 08:14:29 by dbaladro         ###   ########.fr       */
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
        return (print_error("create_here_doc", errno), -1);
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
        return (ft_printf("Error"), 0);
    ft_printf("SuCess\n");
    return (free(line), 0);
}

int first_command(int fd, int pipe[2], char *cmd)
{
    char    **parsed_command;

    close(pipe[0]);
    if (dup2(fd, STDIN_FILENO) < 0)
        return (print_error("first_command", errno), EXIT_FAILURE);
    if (dup2(pipe[1], STDOUT_FILENO) < 0)
        return (print_error("first_command", errno), EXIT_FAILURE);
    parsed_command = parse_command(cmd);
    if (!parsed_command)
        return (EXIT_FAILURE);
    close(pipe[1]);
    close(fd);
    execve(parsed_command[0], parsed_command, env);
    if (unlink("here_doc") < 0)
        print_error("first_command: unlink error", errno);
    print_error("first_command", errno);
    return (EXIT_SUCCESS);
}

int second_command(int fd, int pipe[2], char *cmd)
{
    char    **parsed_command;
    int     exec_result;

    close(pipe[1]);
    if (dup2(fd, STDOUT_FILENO) < 0)
        return (print_error("second_command", errno), EXIT_FAILURE);
    if (dup2(pipe[0], STDIN_FILENO) < 0)
        return (print_error("second_command", errno), EXIT_FAILURE);
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
    int     fd_here_doc;
    int     fd_outfile;
    int     pipefd[2];
    pid_t   pid;

    (void) ac;
    if (pipe(pipefd) == -1)
        return (print_error("pipe_here_doc", errno), -1);
    if (create_here_doc(av[2]) < 0)
        return (EXIT_FAILURE);
    fd_here_doc = open("./here_doc", O_RDONLY);
    if (fd_here_doc < 0)
        return (print_error("pipe_error: Nor opening here_doc", errno), 1);
    fd_outfile = open(av[5], O_CREAT | O_APPEND | O_WRONLY, 00664);
    if (fd_outfile < 0)
        // print_error("pipe_here_doc", errno);
        return (print_error("pipe_here_doc", errno), -1);
    pid = fork();
    if (pid == -1)
        return (print_error("pipex_here_doc", errno),-1);
    if (pid == 0)
        first_command(fd_here_doc, pipefd, av[3]);  
    else
    {
        pid = fork();
        if (pid < 0)
             return (print_error("pipex_here_doc", errno),-1);
        if (pid == 0)
            second_command(fd_outfile, pipefd, av[4]);
        else
            wait(NULL);
    }
    return (0);
}