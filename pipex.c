/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 16:09:39 by shechong          #+#    #+#             */
/*   Updated: 2023/07/20 12:11:05 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	last_cmd(char *cmd, char **env, int infile, int outfile)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		if (outfile > -1)
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

int	main(int ac, char **av, char **env)
{
	int	infile;
	int	outfile;
	int	transfer;
	int	start;

	if (ac != 5)
		return (write(2, "Invalid argument count\n", 24));
	start = 1;
	infile = open_files(av[1], 'i');
	outfile = open_files(av[4], 'o');
	transfer = open("transfer", O_RDONLY | O_CREAT, 436);
	if (outfile > 0)
		start = scan_cmd(ac, av, env, 1);
	dup2(transfer, 0);
	if (start == 1)
		dup2(infile, 0);
	run_cmd_redir(av[2], env);
	dup2(outfile, 1);
	if (outfile == -1)
		dup2(transfer, 1);
	last_cmd(av[3], env, infile, outfile);
	return (0);
}
