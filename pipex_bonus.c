/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:24:33 by shechong          #+#    #+#             */
/*   Updated: 2023/07/31 11:51:13 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	gnl(char *str, int pipefd[2])
{
	char	*line;

	close(pipefd[0]);
	dup2(pipefd[1], 1);
	write(0, "> ", 2);
	line = get_next_line(0);
	while (1)
	{
		if (!line)
			exit(0);
		if (ft_strncmp(line, str, ft_strlen(str)) == 0
			&& ft_strlen(line) - 1 == ft_strlen(str))
			break ;
		write(0, "> ", 2);
		ft_putstr_fd(line, 1);
		free(line);
		line = get_next_line(0);
	}
	free(line);
	return (1);
}

void	run_cmd_redir2(char *cmd, char **env, char p, char **av)
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
		exit(0);
	pid = fork();
	if (pid == -1)
		exit(0);
	if (pid != 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], 0);
	}
	else
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
	}
	return ;
}

void	last_cmd(char *cmd, char **env, int infile, int outfile)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		if (outfile >= 0)
			run_cmd(cmd, env);
		exit(0);
	}
	else
	{
		wait(0);
		close(infile);
		close(outfile);
	}
}

int	here_doc(int ac, char **av, char **env)
{
	int	outfile;
	int	i;
	int	start;
	int	pipefd[2];
	int	pid;

	if (ac < 6)
		return (write(2, "Invalid argument count\n", 24));
	pipe(pipefd);
	pid = fork();
	if (pid == 0)
		exit(gnl(av[2], pipefd));
	close(pipefd[1]);
	wait(0);
	outfile = open_files(av[ac - 1], 'a');
	start = scan_cmd(ac, av, env, 2);
	i = start + (start == 1);
	if (start != 1)
		pipefd[0] = open("/dev/null", 0);
	dup2(pipefd[0], 0);
	while (++i < ac - 2 && ac - start >= 4)
		run_cmd_redir(av[i], env);
	dup2(outfile, 1);
	last_cmd(av[ac - 2], env, outfile, outfile);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	int	infile;
	int	outfile;
	int	transfer;
	int	i;
	int	start;

	if (ac < 5)
		return (write(2, "Invalid argument count\n", 24));
	if (ft_strncmp("here_doc", av[1], 8) == 0 && ft_strlen(av[1]) == 8)
		return (here_doc(ac, av, env));
	start = scan_cmd(ac, av, env, 1);
	i = start;
	infile = open_files(av[1], 'i');
	outfile = open_files(av[ac - 1], 'o');
	dup2(open("/dev/null", 0), 0);
	if (start == 1)
		dup2(infile, 0);
	while (++i < ac - 2 && ac - start >= 4)
		run_cmd_redir(av[i], env);
	dup2(outfile, 1);
	last_cmd(av[ac - 2], env, infile, outfile);
}
