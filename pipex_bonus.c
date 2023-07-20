/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 16:09:16 by shechong          #+#    #+#             */
/*   Updated: 2023/07/20 17:05:53 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	gnl(int ac, char **av, char *str)
{
	char	*line;
	int		infile;

	infile = open("transfer", O_CREAT | O_WRONLY | O_TRUNC, 436);
	write(0, "> ", 2);
	line = get_next_line(0);
	while (1)
	{
		if (!line)
		{
			unlink("transfer");
			exit(0);
		}
		if (ft_strncmp(line, str, ft_strlen(str)) == 0
			&& ft_strlen(line) - 1 == ft_strlen(str))
			break ;
		write(0, "> ", 2);
		ft_putstr_fd(line, infile);
		free(line);
		line = get_next_line(0);
	}
	free(line);
	close(infile);
	infile = open("transfer", O_RDONLY, 436);
	return (infile);
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
		unlink("transfer");
		unlink("buffer");
		close(infile);
		close(outfile);
	}
}

int	here_doc(int ac, char **av, char **env)
{
	int	infile;
	int	outfile;
	int	buffer;
	int	i;
	int	start;

	if (ac < 6)
		return (write(2, "Invalid argument count\n", 24));
	infile = gnl(ac, av, av[2]);
	outfile = open_files(av[ac - 1], 'a');
	start = scan_cmd(ac, av, env, 2);
	i = start;
	if (start == 1)
		i ++;
	buffer = open("buffer", O_RDONLY | O_CREAT | O_TRUNC, 436);
	dup2(buffer, 0);
	if (start == 1)
		dup2(infile, 0);
	while (++i < ac - 2 && ac - start >= 4)
		run_cmd_redir(av[i], env);
	dup2(outfile, 1);
	last_cmd(av[ac - 2], env, infile, outfile);
	if (start == 1)
		return (0);
	return (1);
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
	transfer = open("transfer", O_RDONLY | O_CREAT, 436);
	dup2(transfer, 0);
	if (start == 1)
		dup2(infile, 0);
	while (++i < ac - 2 && ac - start >= 4)
		run_cmd_redir(av[i], env);
	dup2(outfile, 1);
	last_cmd(av[ac - 2], env, infile, outfile);
	if (start != 1)
		return (1);
}
