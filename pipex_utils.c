/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 16:09:34 by shechong          #+#    #+#             */
/*   Updated: 2023/07/20 13:42:48 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	*sub;
	char	*join;
	char	*pathjoin;

	while (*env && ft_strncmp(*env, "PATH=", 5) != 0)
		env++;
	if (!*env)
		return (cmd);
	path = *env + 5;
	while (path && *path)
	{
		sub = ft_substr(path, 0, (size_t)(ft_strchr(path, ':') - path));
		join = ft_strjoin(sub, "/");
		pathjoin = ft_strjoin(join, cmd);
		if (access(pathjoin, F_OK) == 0)
			return (f(join, f(sub, (void *)pathjoin)));
		f(pathjoin, f(join, f(sub, NULL)));
		path = ft_strchr(path, ':');
		if (path)
			path ++;
	}
	return ("\0");
}

void	run_cmd(char *cmd, char **env)
{
	char	**args;
	char	*command;
	int		pid;

	args = ft_split(cmd, ' ');
	command = get_path(args[0], env);
	pid = fork();
	if (pid != 0)
	{
		wait(0);
		fa((void *)args, NULL);
		if (command[0] != '\0')
			free(command);
		exit(0);
	}
	else if (pid == 0)
	{
		if (access(command, F_OK) == 0)
			execve((const char *)command, (char *const *)args, NULL);
		fa((void *)args, NULL);
		exit(1);
	}
}

void	run_cmd_redir(char *cmd, char **env)
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
		run_cmd(cmd, env);
	}
	return ;
}

int	open_files(char *filename, char mode)
{
	int	ret_file;

	if (mode == 'i')
		ret_file = open(filename, O_RDONLY);
	else if (mode == 'o')
		ret_file = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (mode == 'a')
		ret_file = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (ret_file == -1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		perror("");
	}
	return (ret_file);
}

int	scan_cmd(int ac, char **av, char **env, int min)
{
	int		i;
	int		start;
	char	*command;
	char	**cmd_split;
	char	**ret;

	i = ac - 1;
	start = 1;
	while (--i > min)
	{
		cmd_split = ft_split(av[i], ' ');
		command = get_path(cmd_split[0], env);
		if (access(command, F_OK) != 0)
		{
			if (start == 1)
				start = i;
			write(2, "bash: ", 6);
			ft_putstr_fd(cmd_split[0], 2);
			ft_putstr_fd(": command not found\n", 2);
		}
		fa((void **)cmd_split, NULL);
		if (command[0] != '\0')
			free(command);
	}
	return (start);
}
